//
//  timerelated.h
//  ffmpegIOSDemo
//
//  Created by zhangjunhai on 15/7/15.
//  Copyright (c) 2015年 yy company. All rights reserved.
//

#ifndef __ffmpegIOSDemo__timerelated__
#define __ffmpegIOSDemo__timerelated__

#ifdef __cplusplus
extern "C"{
#endif
    
#include <stdio.h>
#include <stdint.h>
#include <mach/mach_time.h>
    
#ifdef  __cplusplus
}
#endif

#ifdef __cplusplus
extern "C"{
#endif
    
//将计数差返回为对应的秒数
double subtractTimes( uint64_t endTime, uint64_t startTime );

#ifdef  __cplusplus
}
#endif
    
#endif /* defined(__ffmpegIOSDemo__timerelated__) */
