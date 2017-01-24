//
//  SerialportEx.h
//  iTMP
//
//  Created by Ryan on 12-2-8.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//
#pragma once

#import <Foundation/Foundation.h>
#import "AMSerialPort.h"
#import "AMSerialPortAdditions.h"
#import "AMSerialPortList.h"
#include "RegexKitLite.h"

@protocol SerialView <NSObject>

//-(void)InsertText:(NSString *)str;
-(void)insertText:(id)str;
-(void)InsertData:(NSData *)data;
@end

@interface  SerialportEx:AMSerialPort{
@private
    NSString * m_strSerialWait;     //待监测的字符串，可以采用正则表达式
    NSString * m_strDetected;       //根据待监测的字符串，监测到的符合的的结果
    NSCondition * m_DetectCondition;
    NSLock * lock;
//    NSMutableString * strBuf;
    id<SerialView> m_SerialView;
    BOOL bDetectFlags;              //字符是否监测到标志
    NSLock * m_BufferLock;
    
    SEL m_SerialCallBack;
    id  m_CallbackTarget;
    
//for debug
}
//Hook entry
-(int)AttachView:(id<SerialView>)view;
-(void)SetCallback:(SEL)Callback Target:(id)object;

//string detect
-(int)SetDetectString:(NSString *)strDectect;
-(int)WaitDetect;
-(int)WaitDetect:(int)iTimeOut;
-(int)ResetCondition;

-(int)PortSetting:(NSString *)strSetting;

-(NSString *)QueryString:(NSString *)strcmd;
-(NSString *)ReadStringValue;
-(NSString *)ReadStringValue:(int)timeout;
-(BOOL)WriteStringValue:(NSString *)strVal;
-(NSData *)GetInBuffer;
-(void)ClearInBuffer;

@property (readwrite,copy) NSString * m_strSerialWait;      //Wait string condition.
@property (readwrite,copy) NSString * m_strDetected;       //根据待监测的字符串，监测到的符合的的结果
@end
