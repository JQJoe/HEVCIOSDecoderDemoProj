//
//  f265wrap.h
//  ffmpegIOSDemo
//
//  Created by zhangjunhai on 15/7/8.
//  Copyright (c) 2015年 yy company. All rights reserved.
//

#ifndef __ffmpegIOSDemo__ittiamwrap__
#define __ffmpegIOSDemo__ittiamwrap__

#ifdef __cplusplus
extern "C"{
#endif
    
#include <stdio.h>
#include <stdlib.h>
#include "ihevc_typedefs.h"
#include "iv.h"
#include "ivd.h"
#include "ihevcd_cxa.h"
#ifdef  __cplusplus
}
#endif


#ifdef __cplusplus
extern "C"{
#endif
    
    struct I265FRAMETYPE
    {
        UWORD8 *data[3];
        int linesize[3];
    };
    typedef struct I265FRAMETYPE IFRAMETYPE;
    
    struct ITTIAMH265Decoder
    {
        iv_obj_t *handle;
        ivd_out_bufdesc_t frameinfo;
        ivd_video_decode_op_t DecoderPara;
        ivd_ctl_getbufinfo_op_t GetBufPara;
        UWORD8 *bufptr;
        IFRAMETYPE *frame_buffer;
        int picheight;
        int picwidth;
    };
    
    typedef struct ITTIAMH265Decoder ittiam_handle_wrapper;
    
    void    ittiamhevc_init(void);
    void*   ittiamhevc_open(void);
    int     ittiamhevc_decode_header(ittiam_handle_wrapper* handle_wrapper, unsigned char* nal_buf,
                                     int nal_length);
    int     ittiamhevc_decode_frame(ittiam_handle_wrapper* handle_wrapper, unsigned char* nal_buf,
                                    int nal_length, int* pic_ready);
    void    ittiamhevc_close(void* handle_wrapper);
    
#ifdef  __cplusplus
}
#endif

#endif /* defined(__ffmpegIOSDemo__ittiamwrap__) */
