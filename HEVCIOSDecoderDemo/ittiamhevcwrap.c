//
//  f265wrap.c
//  ffmpegIOSDemo
//
//  Created by zhangjunhai on 15/7/8.
//  Copyright (c) 2015年 yy company. All rights reserved.
//

#include "ittiamhevcwrap.h"

//#define multi_thread

void *ihevca_aligned_malloc(void *pv_ctxt, WORD32 alignment, WORD32 i4_size)
{
    (void)pv_ctxt;
    return malloc(i4_size);
}

void ihevca_aligned_free(void *pv_ctxt, void *pv_buf)
{
    (void)pv_ctxt;
    free(pv_buf);
    return;
}

IV_API_CALL_STATUS_T IttiamH265HeaderDecode(ittiam_handle_wrapper *pDecoder, unsigned char* pData, int iLen)
{
    ivd_video_decode_ip_t s_video_decode_ip;
    ivd_video_decode_op_t s_video_decode_op;
    ivd_ctl_set_config_ip_t s_ctl_ip;
    ivd_ctl_set_config_op_t s_ctl_op;
    IV_API_CALL_STATUS_T ret = IV_STATUS_NA;
    s_ctl_ip.u4_disp_wd = 0;                    //must be set, or the data output will wrong
    s_ctl_ip.e_frm_skip_mode = IVD_SKIP_NONE;
    s_ctl_ip.e_frm_out_mode = IVD_DISPLAY_FRAME_OUT;
    s_ctl_ip.e_vid_dec_mode = IVD_DECODE_HEADER;
    s_ctl_ip.e_cmd = IVD_CMD_VIDEO_CTL;
    s_ctl_ip.e_sub_cmd = IVD_CMD_CTL_SETPARAMS;
    s_ctl_ip.u4_size = sizeof(ivd_ctl_set_config_ip_t);
    s_ctl_op.u4_size = sizeof(ivd_ctl_set_config_op_t);
    ret = ihevcd_cxa_api_function(pDecoder->handle, (void *)&s_ctl_ip, (void *)&s_ctl_op);
    if (ret != IV_SUCCESS){
        return ret;
    }
    s_video_decode_ip.e_cmd = IVD_CMD_VIDEO_DECODE;
    s_video_decode_ip.u4_ts = 0;
    s_video_decode_ip.pv_stream_buffer = (UWORD8 *)pData;
    s_video_decode_ip.u4_num_Bytes = iLen;
    s_video_decode_ip.u4_size = sizeof(ivd_video_decode_ip_t);
    s_video_decode_op.u4_size = sizeof(ivd_video_decode_op_t);
    ret = ihevcd_cxa_api_function(pDecoder->handle, (void *)&s_video_decode_ip, (void *)&s_video_decode_op);
    if(ret != IV_SUCCESS){
        return ret;
    }
    pDecoder->picwidth = s_video_decode_op.u4_pic_wd;
    pDecoder->picheight = s_video_decode_op.u4_pic_ht;
    return ret;
}
//should wait for a while after calling IttiamH265HeaderDecode
IV_API_CALL_STATUS_T IttiamH265FrameDecodeInit(ittiam_handle_wrapper *pDecoder)
{
    ivd_ctl_set_config_ip_t s_ctl_ip;
    ivd_ctl_set_config_op_t s_ctl_op;
    IV_API_CALL_STATUS_T ret = IV_STATUS_NA;
    s_ctl_ip.e_frm_skip_mode = IVD_SKIP_NONE;
    s_ctl_ip.u4_disp_wd = 0;
    s_ctl_ip.e_frm_out_mode = IVD_DISPLAY_FRAME_OUT;
    s_ctl_ip.e_vid_dec_mode = IVD_DECODE_FRAME;
    s_ctl_ip.e_cmd = IVD_CMD_VIDEO_CTL;
    s_ctl_ip.e_sub_cmd = IVD_CMD_CTL_SETPARAMS;
    s_ctl_ip.u4_size = sizeof(ivd_ctl_set_config_ip_t);
    s_ctl_op.u4_size = sizeof(ivd_ctl_set_config_op_t);
    ret = ihevcd_cxa_api_function(pDecoder->handle, (void *)&s_ctl_ip, (void *)&s_ctl_op);
    return ret;
}

IV_API_CALL_STATUS_T IttiamH265FrameDecode(ittiam_handle_wrapper *pDecoder, unsigned char* pData, int iLen, const long long pts)
{
    IV_API_CALL_STATUS_T ret = IV_STATUS_NA;
    ivd_video_decode_ip_t s_video_decode_ip;
    ivd_video_decode_op_t s_video_decode_op;
    s_video_decode_ip.e_cmd = IVD_CMD_VIDEO_DECODE;
    s_video_decode_ip.u4_ts = (UWORD32)pts;
    s_video_decode_ip.pv_stream_buffer = pData;
    s_video_decode_ip.u4_num_Bytes = iLen;
    s_video_decode_ip.u4_size = sizeof(ivd_video_decode_ip_t);
    s_video_decode_ip.s_out_buffer = pDecoder->frameinfo;
    s_video_decode_op.u4_size = sizeof(ivd_video_decode_op_t);
    
    ret = ihevcd_cxa_api_function(pDecoder->handle, (void *)&s_video_decode_ip, (void *)&s_video_decode_op);
    pDecoder->DecoderPara = s_video_decode_op;
    return ret;
}

IV_API_CALL_STATUS_T IttiamH265DecodeFlush(iv_obj_t *pdeHandle)
{
    ivd_ctl_flush_ip_t s_ctl_ip;
    ivd_ctl_flush_op_t s_ctl_op;
    IV_API_CALL_STATUS_T ret = IV_STATUS_NA;
    
    s_ctl_ip.e_cmd = IVD_CMD_VIDEO_CTL;
    s_ctl_ip.e_sub_cmd = IVD_CMD_CTL_FLUSH;
    s_ctl_ip.u4_size = sizeof(ivd_ctl_flush_ip_t);
    s_ctl_op.u4_size = sizeof(ivd_ctl_flush_op_t);
    ret = ihevcd_cxa_api_function(pdeHandle, (void *)&s_ctl_ip,
                                  (void *)&s_ctl_op);
    return ret;
}

IV_API_CALL_STATUS_T IttiamH265DecodeDelete(iv_obj_t *pdeHandle)
{
    ivd_delete_ip_t s_delete_dec_ip;
    ivd_delete_op_t s_delete_dec_op;
    
    s_delete_dec_ip.e_cmd = IVD_CMD_DELETE;
    s_delete_dec_ip.u4_size = sizeof(ivd_delete_ip_t);
    s_delete_dec_op.u4_size = sizeof(ivd_delete_op_t);
    IV_API_CALL_STATUS_T ret = ihevcd_cxa_api_function(pdeHandle, (void *)&s_delete_dec_ip,
                                                       (void *)&s_delete_dec_op);
    return ret;
}

IV_API_CALL_STATUS_T set_degrade(iv_obj_t *pdeHandle, UWORD32 type, WORD32 pics)
{
    ihevcd_cxa_ctl_degrade_ip_t s_ctl_ip;
    ihevcd_cxa_ctl_degrade_op_t s_ctl_op;
    void *pv_api_ip, *pv_api_op;
    IV_API_CALL_STATUS_T e_dec_status;
    
    s_ctl_ip.u4_size = sizeof(ihevcd_cxa_ctl_degrade_ip_t);
    s_ctl_ip.i4_degrade_type = type;
    s_ctl_ip.i4_nondegrade_interval = 4;
    s_ctl_ip.i4_degrade_pics = pics;
    
    s_ctl_op.u4_size = sizeof(ihevcd_cxa_ctl_degrade_op_t);
    
    pv_api_ip = (void *)&s_ctl_ip;
    pv_api_op = (void *)&s_ctl_op;
    
    s_ctl_ip.e_cmd = IVD_CMD_VIDEO_CTL;
    s_ctl_ip.e_sub_cmd = (IVD_CONTROL_API_COMMAND_TYPE_T)IHEVCD_CXA_CMD_CTL_DEGRADE;
    
    e_dec_status = ihevcd_cxa_api_function(pdeHandle, pv_api_ip, pv_api_op);
    
    return (IV_SUCCESS);
    
}

//解码器初始化接口函数
void ittiamhevc_init(void)
{
    
}

//打开解码器
void* ittiamhevc_open(void)
{
    IV_API_CALL_STATUS_T ret = IV_STATUS_NA;
    ittiam_handle_wrapper *pDe = (ittiam_handle_wrapper *)malloc(sizeof(ittiam_handle_wrapper));
    //create handle
    ihevcd_cxa_create_ip_t s_create_ip;
    ihevcd_cxa_create_op_t s_create_op;
    void *fxns = (void *)(&ihevcd_cxa_api_function);
    s_create_ip.s_ivd_create_ip_t.e_cmd = IVD_CMD_CREATE;
    s_create_ip.s_ivd_create_ip_t.u4_share_disp_buf = 0;                    //disable share display buffer
    s_create_ip.s_ivd_create_ip_t.e_output_format = IV_YUV_420P;
    s_create_ip.s_ivd_create_ip_t.pf_aligned_alloc = ihevca_aligned_malloc;
    s_create_ip.s_ivd_create_ip_t.pf_aligned_free = ihevca_aligned_free;
    s_create_ip.s_ivd_create_ip_t.pv_mem_ctxt = NULL;
    s_create_ip.s_ivd_create_ip_t.u4_size = sizeof(ihevcd_cxa_create_ip_t);
    s_create_op.s_ivd_create_op_t.u4_size = sizeof(ihevcd_cxa_create_op_t);
    ret = ihevcd_cxa_api_function(NULL, (void *)&s_create_ip,
                                  (void *)&s_create_op);
    if(ret != IV_SUCCESS)
    {
        return NULL;
    }
    pDe->handle = (iv_obj_t*)s_create_op.s_ivd_create_op_t.pv_handle;
    pDe->handle->pv_fxns = fxns;
    pDe->handle->u4_size = sizeof(iv_obj_t);
    //set cores
    ihevcd_cxa_ctl_set_num_cores_ip_t s_ctl_set_cores_ip;
    ihevcd_cxa_ctl_set_num_cores_op_t s_ctl_set_cores_op;
    
    s_ctl_set_cores_ip.e_cmd = IVD_CMD_VIDEO_CTL;
    s_ctl_set_cores_ip.e_sub_cmd = (IVD_CONTROL_API_COMMAND_TYPE_T)IHEVCD_CXA_CMD_CTL_SET_NUM_CORES;
#ifdef multi_thread
    s_ctl_set_cores_ip.u4_num_cores = 2;
#else
    s_ctl_set_cores_ip.u4_num_cores = 1;
#endif
    s_ctl_set_cores_ip.u4_size = sizeof(ihevcd_cxa_ctl_set_num_cores_ip_t);
    s_ctl_set_cores_op.u4_size = sizeof(ihevcd_cxa_ctl_set_num_cores_op_t);
    
    ret = ihevcd_cxa_api_function((iv_obj_t *)(pDe->handle), (void *)&s_ctl_set_cores_ip,
                                  (void *)&s_ctl_set_cores_op);
    if(ret != IV_SUCCESS)
    {
        return NULL;
    }
    pDe->picheight = 0;
    pDe->picwidth = 0;
    pDe->frameinfo.u4_num_bufs = 3;              //yuv buffer number
    pDe->bufptr = NULL;
    pDe->frame_buffer = (IFRAMETYPE *)malloc(sizeof(IFRAMETYPE));
    
    return pDe;
}

//关闭解码器
void ittiamhevc_close(void* handle_wrapper){
    ittiam_handle_wrapper *pDe = (ittiam_handle_wrapper *)handle_wrapper;
    if(!pDe) return ;
    if(!pDe->handle){
        IttiamH265DecodeFlush(pDe->handle);
        IttiamH265DecodeDelete(pDe->handle);
        pDe->handle = NULL;
    }
    if (pDe->bufptr != NULL) {
        free(pDe->bufptr);
    }
    
    if (pDe->frame_buffer != NULL)
        free(pDe->frame_buffer);
    
    free(pDe);
    pDe = NULL;
}

//解码器解码头
int ittiamhevc_decode_header(ittiam_handle_wrapper* handle, unsigned char* nal_buf, int nal_length)
{
    IV_API_CALL_STATUS_T ret = IV_STATUS_NA;
    ittiam_handle_wrapper* pDe = handle;
    
    if (pDe->bufptr != NULL)
        free(pDe->bufptr);
    
    ret = IttiamH265HeaderDecode(pDe, nal_buf, nal_length);
    if (ret != IV_SUCCESS)
    {
        return -1;
    }
    int picsize = pDe->picheight * pDe->picwidth;
    pDe->bufptr = (UWORD8 *)malloc(picsize + (picsize >> 1));
    if (pDe->bufptr == NULL)
    {
        return -1;
    }
    pDe->frameinfo.u4_min_out_buf_size[0] = picsize;
    pDe->frameinfo.u4_min_out_buf_size[1] = picsize >> 2;
    pDe->frameinfo.u4_min_out_buf_size[2] = picsize >> 2;
    pDe->frameinfo.pu1_bufs[0] = pDe->bufptr;
    pDe->frameinfo.pu1_bufs[1] = pDe->frameinfo.pu1_bufs[0] + pDe->frameinfo.u4_min_out_buf_size[0];
    pDe->frameinfo.pu1_bufs[2] = pDe->frameinfo.pu1_bufs[1] + pDe->frameinfo.u4_min_out_buf_size[1];
    ret = IttiamH265FrameDecodeInit(pDe); // init for frame decode, should keep a wait after called header decode
    if (ret != IV_SUCCESS) {
        return -1;
    }
    return 0;
}


//解码器解码单帧
int ittiamhevc_decode_frame(ittiam_handle_wrapper* handle, unsigned char* nal_buf, int nal_length, int* pic_ready)
{
    IV_API_CALL_STATUS_T ret = IV_STATUS_NA;
    ittiam_handle_wrapper* pDe = handle;
    
    ret = IttiamH265FrameDecode(pDe, nal_buf, nal_length, 0);
    if (ret != IV_SUCCESS){
        return 0;
    }
    //    int offsety = pDe->DecoderPara.s_disp_frm_buf.u4_y_ht * pDe->DecoderPara.s_disp_frm_buf.u4_y_strd;
    //    int offsetu = pDe->DecoderPara.s_disp_frm_buf.u4_u_ht * pDe->DecoderPara.s_disp_frm_buf.u4_u_strd;
    //    int offsetv = pDe->DecoderPara.s_disp_frm_buf.u4_v_ht * pDe->DecoderPara.s_disp_frm_buf.u4_v_strd;
    pDe->frame_buffer->linesize[0] = pDe->DecoderPara.s_disp_frm_buf.u4_y_strd;
    pDe->frame_buffer->linesize[1] = pDe->DecoderPara.s_disp_frm_buf.u4_u_strd;
    pDe->frame_buffer->linesize[2] = pDe->DecoderPara.s_disp_frm_buf.u4_v_strd;
    pDe->frame_buffer->data[0] = pDe->DecoderPara.s_disp_frm_buf.pv_y_buf;
    pDe->frame_buffer->data[1] = pDe->DecoderPara.s_disp_frm_buf.pv_u_buf;
    pDe->frame_buffer->data[2] = pDe->DecoderPara.s_disp_frm_buf.pv_v_buf;
    
    *pic_ready = pDe->DecoderPara.u4_output_present;
    
    return pDe->DecoderPara.u4_num_bytes_consumed;
}