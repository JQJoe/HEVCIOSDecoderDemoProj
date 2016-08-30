//
//  timerelated.c
//  ffmpegIOSDemo
//
//  Created by zhangjunhai on 15/7/15.
//  Copyright (c) 2015年 yy company. All rights reserved.
//

#include "timerelated.h"

//Raw mach_absolute_times going in,difference in seconds out
double subtractTimes( uint64_t endTime, uint64_t startTime )
{
    uint64_t difference = endTime - startTime;
    static double conversion = 0.0;
    if( conversion == 0.0 )
    {
        mach_timebase_info_data_t info;
        kern_return_t err =mach_timebase_info( &info );
        //Convert the timebase into seconds
        if( err == 0  )
            conversion= 1e-9 * (double) info.numer / (double) info.denom;
    }
    return conversion * (double)difference;
}