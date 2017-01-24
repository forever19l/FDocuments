//
//  DataModule.h
//  CoreLib
//
//  Created by Ryan on 12-5-27.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#include "Common.h"

@protocol DataModule <NSObject>
@required
-(NSString *)ModuleName;           //模块名称说明
-(int)RegisterModule:(id)sender;     //登记模块
@optional
-(int)DoConfig:(id)sender;
-(int)DoAction:(id)sender;

//UI Update function
-(int)OnTestStart:(id)sender parameter:(id)par;     //测试开始
-(int)OnTestStop:(id)sender parameter:(id)par;      //测试停止
-(int)OnTestPasue:(id)sender parameter:(id)par;     //测试暂停
-(int)OnTestResume:(id)sender parameter:(id)par;    //测试继续
-(int)OnTestFinish:(id)sender parameter:(id)par;    //测试完成
-(int)OnItemStart:(id)sender parameter:(id)par;     //测试项目开始
-(int)OnItemFinish:(id)sender parameter:(id)par;    //测试项目完成

@end
