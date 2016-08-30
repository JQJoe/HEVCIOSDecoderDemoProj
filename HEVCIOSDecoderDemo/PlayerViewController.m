//
//  PlayerViewController.m
//  ffmpegIOSDemo
//
//  Created by zhangjunhai on 15/7/10.
//  Copyright (c) 2015年 yy company. All rights reserved.
//

#import "PlayerViewController.h"

@interface PlayerViewController ()
@property   (nonatomic) NSUInteger  filesize;
@property (nonatomic) unsigned int framenum;
@property (nonatomic) VideoDecode* Vdecode0;
@property (nonatomic) unsigned char *srcdata;
@property (nonatomic) f265_handle_wrapper *handle;

- (void)Paraminit;
- (void) mediaplayer;
- (unsigned char*) videoread;
@end

@implementation PlayerViewController

#define videoname @"ring_lent_720p_600kbps.hevc"

#pragma mark - ThreadFunc
- (void)mediaplayer{
    
    BOOL FrameStartFlag = false;
    int Fconsumed = 0;
    NSInteger framelen;
    self.srcdata = [self videoread];
    unsigned char *framedata;
    unsigned char *rgbdata = (unsigned char *)malloc(1280*1280*3);//used to store the image
    NSLog(@"mediaplayer thread start!");
    
    [self.Vdecode0 decode_init];
    self.handle = [self.Vdecode0 decode_open];
    
    while (self.Vdecode0.consumed < self.filesize){
        
        NSLog(@"Frame %d decoded!", self.framenum);
        FrameStartFlag = [self.Vdecode0 ExtractFrameData: self.srcdata :framedata
                                                        :&framelen : self.Vdecode0.consumed
                                                        : self.filesize];
        if (FrameStartFlag) {
            Fconsumed = [self.Vdecode0 decode_play: self.handle :framedata :framelen];
            if (self.Vdecode0.pic_ready  && Fconsumed > 0) {
                //yuvtorgb
                I420ToRGB565(self.handle->frame_buffer->data[0], self.handle->frame_buffer->linesize[0],
                             self.handle->frame_buffer->data[1], self.handle->frame_buffer->linesize[1],
                             self.handle->frame_buffer->data[2], self.handle->frame_buffer->linesize[2],
                             rgbdata, ((int)self.Vdecode0.framesize.width) << 1,
                             self.Vdecode0.framesize.width, self.Vdecode0.framesize.height);
                //add rend code
                
                self.framenum++;
            }
        }
        self.Vdecode0.consumed += framelen;    //得到已经解码后长度
    }
}

#pragma mark - UISetFunc
- (void)Paraminit
{
    self.framenum = 0;
    self.filesize = 0;
    
}

- (unsigned char*)videoread
{
    //construct the full name of the vedio store path
    NSArray *path = NSSearchPathForDirectoriesInDomains(NSDocumentationDirectory,
                                                        NSUserDomainMask, YES);
    NSString *docpath = [path objectAtIndex:0];
    NSString *videopath = [docpath stringByAppendingString:videoname];
    //Get data from the path
    NSData *myData = [NSData dataWithContentsOfFile:videopath];
    self.filesize = myData.length;
    return (unsigned char*)[myData bytes];
}

//UI set func
- (void)viewDidLoad {
    [super viewDidLoad];
    [self Paraminit];
    // Do any additional setup after loading the view.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
