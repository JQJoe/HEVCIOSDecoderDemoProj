//
//  f265wrap.h
//  ffmpegIOSDemo
//
//  Created by zhangjunhai on 15/7/8.
//  Copyright (c) 2015年 yy company. All rights reserved.
//

#ifndef __ffmpegIOSDemo__f265wrap__
#define __ffmpegIOSDemo__f265wrap__

#ifdef __cplusplus
extern "C"{
#endif
    
#include <stdio.h>
#include "libavcodec/avcodec.h"
#include "libavutil/pixfmt.h"
    
#ifdef  __cplusplus
}
#endif


#ifdef __cplusplus
extern "C"{
#endif
    
    typedef struct{
        
        AVCodecContext* f265_context;
        AVFrame* frame_buffer;
        AVPacket* avpkt;
        
    }f265_handle_wrapper;
    
    void    f265_init(void);
    void*   f265_open(void);
    int     f265_decode(f265_handle_wrapper* handle, unsigned char* nal_buf,
                        int nal_length, int* pic_ready);
    void    f265_close(void* handle);
    
#ifdef  __cplusplus
}
#endif

#endif /* defined(__ffmpegIOSDemo__f265wrap__) */
