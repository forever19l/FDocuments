//
//  DriverModule.h
//  CoreLib
//
//  Created by Ryan on 12-4-30.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//
//
// 每一个设备模块(bundle)的PricipClass必须实现该protocol
// principalClass的init函数应该完成nib/xib文件的加载,并设置好各个windows的属性等

#import <Foundation/Foundation.h>
#include "Common.h"
#define CMD_SAVE_LOG     0x00        //Save log


@protocol DriverModule <NSObject>
@required
-(NSString *)ModuleName;           //模块名称说明
-(int)RegisterModule:(id)sender;     //登记模块
-(int)SelfTest:(id)sender;          //模块自我测试
-(int)Load:(id)sender;
-(int)Unload:(id)sender;
@optional
-(int)DoConfig:(id)sender;
-(int)DoAction:(id)sender;
-(int)DoCmd:(int)cmd arg:(id)arg;
@end
