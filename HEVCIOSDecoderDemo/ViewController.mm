//
//  ViewController.m
//  ffmpegIOSDemo
//
//  Created by zhangjunhai on 15/7/8.
//  Copyright (c) 2015年 yy company. All rights reserved.
//

#import "ViewController.h"
#include <sys/time.h>

static struct timeval timeStart;

@interface ViewController ()
{
    AAPLEAGLLayer *vediolayer;
}
@property (nonatomic) CVPixelBufferRef pixelBuffer;
@property (nonatomic) unsigned char* framedata;
@property (strong, nonatomic) NSThread* Playthread;
@property (nonatomic) int  filesize;
@property (nonatomic) unsigned int framenum;
@property (nonatomic) VideoDecode* Vdecode0;
@property (nonatomic) unsigned char *srcdata;
@property (nonatomic) NSString *videofile;
@property (nonatomic) decoder_handle_wrapper *handle;
@property (weak, nonatomic) IBOutlet UILabel *ParamLabel;
@property (weak, nonatomic) IBOutlet UIView *VedioView;

- (void) Paraminit;
- (void) mediaplayer;
- (unsigned char*) videoread:(NSString *)videoname;
- (CVPixelBufferRef) pixelBufferFromPictureData:(PicDataType *)frame_buffer :(CGSize)Picsize;
- (CVPixelBufferRef)allocPixelBuffer:(uint32_t)width height:(uint32_t)height;

@end

@implementation ViewController

//#define TestPerf


#pragma mark - ThreadFunc
//解码渲染控制线程
- (void)mediaplayer{
NSArray *Varray = [NSArray arrayWithObjects:
                   @"movie",
                   nil];

    
    int Fconsumed = 0;
    int framelen = 0;
    int framestart = -1;
    int64_t ms_used;
    NSInteger Nvideo = [Varray count];//获取视频数目
    self.Vdecode0 = [[VideoDecode alloc] init];
    for (NSInteger i = 0; i < Nvideo; i++)
    {
    _videofile = [Varray objectAtIndex:i];
    self.ParamLabel.text =_videofile;
    self.srcdata = [self videoread:_videofile];
    self.framedata = _srcdata;
    self.Vdecode0.consumed = 0;
    self.framenum = 0;
    NSLog(@"mediaplayer thread start!");
    double framerate = 0.0;
    double real_time = 0.0;
    clock_t clock_start, clock_end, clock_used;
    struct timeval tv_start, tv_end;
    
    gettimeofday(&tv_start, NULL);
    clock_start =clock();
        [self.Vdecode0 decode_init];
        self.handle = static_cast<decoder_handle_wrapper*>([self.Vdecode0 decode_open]);
        bool isOK = [self.Vdecode0 decode_header: self.handle :self.srcdata :200];
    if(!isOK)
        NSLog(@"FrameDecode error!");
    
    while (self.Vdecode0.consumed < self.filesize && self.PlayStatus){
        [self.Vdecode0 ExtractFrameData:self.srcdata :&framestart
                                       :&framelen :self.Vdecode0.consumed
                                       :self.filesize];
        self.framedata = self.srcdata + framestart;
        Fconsumed = [self.Vdecode0 decode_play: self.handle :self.framedata :framelen];
        if (framelen > 0) {
            if (self.Vdecode0.pic_ready) {
                if (self.framenum == 0) {
                    gettimeofday(&timeStart, NULL);}
                NSLog(@"Frame %d decoded!", self.framenum++);
                //self.framenum++;
                //add rend code
#ifndef TestPerf
                vediolayer.pixelBuffer = [self pixelBufferFromPictureData:
                                          self.handle->frame_buffer :self.Vdecode0.framesize];
#endif//TestPerf
                
            }
            self.Vdecode0.consumed += framelen;    //得到已经解码后长度
        }
        else
            break;
    }
    clock_end = clock();
    gettimeofday(&tv_end, NULL);
    clock_used = clock_end - clock_start;
    ms_used = (int64_t)(clock_used * 1000.0 / CLOCKS_PER_SEC);
    real_time = (tv_end.tv_sec + (tv_end.tv_usec / 1000000.0)) - (tv_start.tv_sec + (tv_start.tv_usec / 1000000.0));
    printf("%d frame decoded\n"
           "\ttime\tfps\n"
           "CPU\t%lldms\t%.2f\n"
           "Real\t%.3fs\t%.2f.\n",
           self.framenum,
           ms_used, self.framenum * 1000.0 / ms_used,
           real_time, self.framenum / real_time);
    framerate = (double)self.framenum/real_time;

    NSLog(@"%@\n",_videofile);
    NSLog(@"The decoder fps = %.1f",framerate);
}
    [self.Vdecode0 decode_close: self.handle];
    NSLog(@"Test finished all!");
}

#pragma mark - RendPrepareFunc
//得到经过NV转化后渲染使用的CVPixelBufferRef类型数据
- (CVPixelBufferRef)pixelBufferFromPictureData:(PicDataType *)frame_buffer :(CGSize)Picsize
{
    CVPixelBufferRef pb = [self allocPixelBuffer:Picsize.width height:Picsize.height];
    
    CVPixelBufferLockBaseAddress(pb, 0);
    
    void *baseAddress = CVPixelBufferGetBaseAddress(pb);
    CVPlanarPixelBufferInfo_YCbCrBiPlanar *pi = static_cast<CVPlanarPixelBufferInfo_YCbCrBiPlanar*>(baseAddress);
    size_t yPlaneOffset = CFSwapInt32(pi->componentInfoY.offset);
    size_t cbcrPlaneOffset = CFSwapInt32(pi->componentInfoCbCr.offset);
    uint32_t yPlanePitch = CFSwapInt32(pi->componentInfoY.rowBytes);
    uint32_t cbcrPlanePitch = CFSwapInt32(pi->componentInfoCbCr.rowBytes);
    
    unsigned char *dest = (unsigned char*)baseAddress;
    
    libyuv::I420ToNV12(frame_buffer->data[0], frame_buffer->linesize[0],
                       frame_buffer->data[1], frame_buffer->linesize[1],
                       frame_buffer->data[2], frame_buffer->linesize[2],
                       dest + yPlaneOffset, yPlanePitch,
                       dest + cbcrPlaneOffset, cbcrPlanePitch,
                       Picsize.width, Picsize.height);
    
    CVPixelBufferUnlockBaseAddress(pb, 0);
    
    return pb;
}
//初始化需要显示的数据
- (CVPixelBufferRef)allocPixelBuffer:(uint32_t)width height:(uint32_t)height
{
    if(_pixelBuffer) {
        uint32_t w = (uint32_t)CVPixelBufferGetWidth(_pixelBuffer);
        uint32_t h = (uint32_t)CVPixelBufferGetHeight(_pixelBuffer);
        
        if(w != width || h != height) {
            CVPixelBufferRelease(_pixelBuffer);
            _pixelBuffer = NULL;
        }
    }
    
    if(_pixelBuffer == NULL) {
        const void *keys[] =   {
            kCVPixelBufferOpenGLESCompatibilityKey,
            kCVPixelBufferIOSurfacePropertiesKey
        };
        const void *values[] = {
            (__bridge const void *)([NSNumber numberWithBool:YES]),
            (__bridge const void *)([NSDictionary dictionary])
        };
        
        CFDictionaryRef optionsDictionary = CFDictionaryCreate(NULL, keys, values, 2, NULL, NULL);
        CVReturn err = CVPixelBufferCreate(kCFAllocatorDefault,
                                           width,
                                           height,
                                           kCVPixelFormatType_420YpCbCr8BiPlanarFullRange,
                                           optionsDictionary,
                                           &_pixelBuffer);
        
        if(err != 0) {
            NSLog(@"CVPixelBufferCreate failed error=%d", err);
        }
        
        CFRelease(optionsDictionary);
    }
    
    return _pixelBuffer;
}

#pragma mark - UISetFunc
- (void)Paraminit
{
    self.framenum = 0;
    self.filesize = 0;
    self.PlayStatus = FALSE;    //初始化按键状态为无效
}

- (unsigned char*)videoread:(NSString *)videoname
{
    //construct the full name of the vedio store path
    NSString *h265filepath = [[NSBundle mainBundle] pathForResource:videoname ofType:@"hevc"];
    NSFileHandle *H265file = [NSFileHandle fileHandleForReadingAtPath:h265filepath];
    NSData *myData = [H265file readDataToEndOfFile];
    self.filesize = (int)myData.length;
    NSLog(@"filesize is: %d", (int)self.filesize);
    return (unsigned char*)[myData bytes];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    [self Paraminit];
    self.ParamLabel.text = _videofile;
#ifndef TestPerf
    CGRect layerbounds = CGRectMake(self.VedioView.bounds.origin.x, self.VedioView.bounds.origin.y,
                                    3*self.VedioView.bounds.size.height/4,
                                    3*self.VedioView.bounds.size.width/4);
    vediolayer = [[AAPLEAGLLayer alloc] initWithFrame:layerbounds];
    [self.VedioView.layer addSublayer:vediolayer];
#endif
}

#pragma mark  - ButtonFunc
- (IBAction)StartBtn:(UIButton *)sender {
     self.ParamLabel.text =_videofile;
//设置播放状态为播放
    if (!self.PlayStatus) {
        self.PlayStatus = TRUE;
        dispatch_async(dispatch_get_global_queue(0, 0), ^{
            [self mediaplayer];});
    }
}

- (IBAction)StopBtn:(UIButton *)sender {
    
    self.ParamLabel.text = @" ";
    if (self.PlayStatus) {
        self.PlayStatus = FALSE;
    }
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
