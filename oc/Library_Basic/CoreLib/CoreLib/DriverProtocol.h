//
//  DriverProtocol.h
//  CoreLib
//
//  Created by Ryan on 12-4-30.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//
// 每一个设备模块(bundle)的PricipClass必须实现该protocol
// principalClass的init函数应该完成nib/xib文件的加载,并设置好各个windowsd的属性等

#import <Foundation/Foundation.h>
#include <lua.hpp>


@protocol DriverProtocol <NSObject>
@required
-(NSString *)Description;           //模块名称说明
-(int)ReisterModule:(id)sender;     //登记模块
@optional
-(int)DoConfig:(id)sender;
@end
