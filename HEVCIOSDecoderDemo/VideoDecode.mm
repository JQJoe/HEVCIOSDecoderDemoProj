//
//  VideoDecode.m
//  ffmpegIOSDemo
//
//  Created by zhangjunhai on 15/7/9.
//  Copyright (c) 2015年 yy company. All rights reserved.
//

#import "VideoDecode.h"

@implementation VideoDecode

- (VideoDecode*)init
{
    return self;
}

- (BOOL)ExtractFrameData:(unsigned char *)data :(int *)nalstart
                        :(int *)nallen :(int)usedlen :(int)filesize
{
    BOOL FrameStartFound = false;
    int naltype = 0xff;
    *nalstart = -1;
    
    //NSLog(@"In ExtractFrameData");
    for (int i=usedlen+3; i<filesize-2; i++) {
        //NSLog(@"Data[%d] = %d", i,data[i+4]);
        if (data[i]==1) {
            if ((data[i-3] | data[i-2] | data[i-1]) == 0) {
                naltype = (data[i+4]>>1) & 0x3f;
                if(naltype<=63){
                    if (FrameStartFound) {
                        *nallen = i-(*nalstart)-3;
                        //NSLog(@"Pos %d: length = %d naltype = %d", *nalstart, *nallen, naltype);
                        return FrameStartFound;
                    }
                    else{
                        FrameStartFound = true;
                        *nalstart = i-3;
                        *nallen = filesize - (*nalstart);
                    }
                }
            }
        }
    }
    return  FrameStartFound;
}

#pragma mark - DecodeMethod
#ifdef ittiam_test
- (void)decode_init
{
    ittiamhevc_init();
    self.consumed = 0;
    self.pic_ready = false;
    self.framesize = CGSizeZero;   //默认分辨率为（0，0）
    //NSLog(@"decode_init");
}

- (void *)decode_open
{
    //NSLog(@"decode_open");
    return ittiamhevc_open();
}

- (bool)decode_header: (decoder_handle_wrapper *)handle :(unsigned char*)nal_buf :(int)nal_length
{
    int isOK = -1;
    
    isOK = ittiamhevc_decode_header(handle, nal_buf, nal_length);
    if(isOK == -1)
        return false;
    
    return true;
}

- (int)decode_play: (decoder_handle_wrapper *)handle :(unsigned char*)nal_buf :(int)nal_length
{
    int picture_ready = 0;
    int fconsumed = 0;
    
    fconsumed = ittiamhevc_decode_frame(handle, nal_buf, nal_length, &picture_ready);
    
    if (fconsumed>0 && picture_ready!=0) {
        self.framesize = CGSizeMake(handle->picwidth,handle->picheight);
    }
    
    self.pic_ready = picture_ready ? true : false;
    
    return fconsumed;
}

- (void)decode_close: (decoder_handle_wrapper *)handle
{
    //NSLog(@"decode_close");
    if (handle) {
        ittiamhevc_close(handle);
    }
}

#else
- (void)decode_init
{
    f265_init();
    self.consumed = 0;
    self.pic_ready = false;
    self.framesize = CGSizeZero;   //默认分辨率为（0，0）
    //NSLog(@"decode_init");
}

- (bool)decode_header: (decoder_handle_wrapper *)handle :(unsigned char*)nal_buf :(int)nal_length
{
    return true;
}

- (void *)decode_open
{
    //NSLog(@"decode_open");
    return f265_open();
}

- (void)decode_close:(decoder_handle_wrapper *)handle
{
    NSLog(@"decode_close");
    if (handle) {
        f265_close(handle);
    }
}

- (int)decode_play: (decoder_handle_wrapper *)handle :(unsigned char*)nal_buf :(int)nal_length
{
    int picture_ready = 0;
    int fconsumed = 0;
    fconsumed = f265_decode(handle, nal_buf, nal_length, &picture_ready);
    
    if (fconsumed>0 && picture_ready!=0) {
        self.framesize = CGSizeMake(handle->f265_context->width,handle->f265_context->height);
    }
    
    self.pic_ready = picture_ready ? true : false;
    
    return fconsumed;
}
#endif
@end
