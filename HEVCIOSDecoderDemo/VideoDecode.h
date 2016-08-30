//
//  VideoDecode.h
//  ffmpegIOSDemo
//
//  Created by zhangjunhai on 15/7/9.
//  Copyright (c) 2015年 yy company. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "f265wrap.h"
#import "ittiamhevcwrap.h"

#define ittiam_test
#ifdef  ittiam_test
typedef ittiam_handle_wrapper decoder_handle_wrapper;
typedef I265FRAMETYPE PicDataType;
#else
typedef f265_handle_wrapper decoder_handle_wrapper;
typedef AVFrame PicDataType;
#endif

@interface VideoDecode : NSObject

@property   (atomic) CGSize framesize;
@property   (atomic) BOOL pic_ready;
@property   (atomic) int consumed;

- (VideoDecode*)    init;
- (void)    decode_init;
- (void *)  decode_open;
- (void)    decode_close: (decoder_handle_wrapper *)handle;
- (bool)    decode_header: (decoder_handle_wrapper *)handle :(unsigned char*)nal_buf :(int)nal_length;
- (int)     decode_play: (decoder_handle_wrapper *)handle :(unsigned char*)nal_buf :(int)nal_length;
- (BOOL)    ExtractFrameData: (unsigned char *)data :(int *)nalstart
                            :(int *)nallen :(int)usedlen :(int)filesize;
@end
