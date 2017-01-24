//
//  DataProcessor.h
//  CoreLib
//
//  Created by Ryan on 12-4-30.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//
//  数据处理模块的接口协议
//  所有处理数据的模块(UI,Report,Log...)必须继承该protocol

#import <Foundation/Foundation.h>


@protocol DataProcessor <NSObject>
@required
-(NSString *)ModuleName;               //模块名称描述
-(int)RegitsterModule:(id)sender;       //模块注册
-(int)DisposeData:(id)sender;           //数据处理
@end
