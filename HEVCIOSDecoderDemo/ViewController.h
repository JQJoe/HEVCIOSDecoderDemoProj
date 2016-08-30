//
//  ViewController.h
//  ffmpegIOSDemo
//
//  Created by zhangjunhai on 15/7/8.
//  Copyright (c) 2015年 yy company. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <CoreVideo/CVPixelBuffer.h>
#import <Foundation/Foundation.h>
#import "VideoDecode.h"
#import "libyuv.h"
#import "AAPLEAGLLayer.h"
#import "timerelated.h"

@interface ViewController : UIViewController
@property (atomic) BOOL PlayStatus;
@end

