//
//  f265wrap.c
//  ffmpegIOSDemo
//
//  Created by zhangjunhai on 15/7/8.
//  Copyright (c) 2015年 yy company. All rights reserved.
//

#include "f265wrap.h"

//#define multi_thread

//解码器初始化接口函数
void f265_init(void)
{
    avcodec_register_all();
}

//打开解码器
void* f265_open(void)
{
    int openret = 0;
    
    AVCodec *codec = avcodec_find_decoder(AV_CODEC_ID_HEVC);
    AVCodecContext *codec_context = avcodec_alloc_context3(codec);
    AVPacket* avpkt = (AVPacket *)malloc(sizeof(AVPacket));
    av_init_packet(avpkt);
    AVFrame *frame_buffer = av_frame_alloc();
    f265_handle_wrapper *handle = NULL;
#ifdef multi_thread
    codec_context->thread_count = 2;
    codec_context->thread_type = FF_THREAD_FRAME;
#endif
    openret = avcodec_open2(codec_context, codec, NULL);
    if(openret < 0)
    {
        av_frame_free((AVFrame **)&frame_buffer);
        avcodec_close(codec_context);
        avcodec_free_context(&codec_context);
        free(avpkt);
        avpkt = NULL;
        return handle;
    }
    handle = (f265_handle_wrapper *)malloc(sizeof(f265_handle_wrapper));
    handle->f265_context  = codec_context;
    handle->frame_buffer = frame_buffer;
    handle->avpkt = avpkt;
    
    return handle;
}

//关闭解码器
void f265_close(void* handle){
    
    f265_handle_wrapper* Fhandle = (f265_handle_wrapper*)handle;
    if (Fhandle->f265_context!=NULL)
    {
        avcodec_close(Fhandle->f265_context);
        avcodec_free_context(&Fhandle->f265_context);
    }
    if (Fhandle->frame_buffer!=NULL)
    {
        av_frame_free((AVFrame **)&Fhandle->frame_buffer);
    }
    if(Fhandle->avpkt!=NULL)
    {
        free(Fhandle->avpkt);
        Fhandle->avpkt = NULL;
    }
    if(Fhandle!=NULL)
    {
        free(Fhandle);
        Fhandle = NULL;
    }
}

//解码器解码单帧
int f265_decode(f265_handle_wrapper* handle, unsigned char* nal_buf, int nal_length, int* pic_ready)
{
    handle->avpkt->data = nal_buf;
    handle->avpkt->size = nal_length;
    
    int consumed = avcodec_decode_video2(handle->f265_context, handle->frame_buffer, pic_ready, handle->avpkt);
    if(consumed < 0){
        return -1;
    }
    else if(consumed == 0 && *pic_ready == 0){
        return -1;
    }
    else
        return consumed;
}