//
//  SerialportFactory.h
//  SimpleCom
//
//  Created by ZL-Pro on 16/8/13.
//  Copyright (c) 2016年 ZL-Pro. All rights reserved.
//SimpleComFactory

#import <CoreLib/TestEngine.h>
#import <CoreLib/SerialportEx.h>
#import <CoreLib/ScriptEngine.h>
#import <CoreLib/SerialPortView.h>
#import "SimpleComFactory.h"

@interface SerialportFactory : SimpleComFactory
{
    NSMutableData * m_recive_dataInput;
    SimpleComSetting *m_simpleComSetting;
    SerialportEx *m_mPboardComm;
    NSMutableString *m_UartLog;
    
//    char m_name[255];
//    char m_szSetting[255];
}
-(BOOL)CheckResourceName:(NSString*)Resource;
-(id)initwithName:(SimpleComSetting*)simpleComSetting;
-(void)SetSimpleComSetting:(SimpleComSetting*)simpleComSetting;
-(SimpleComSetting*)GetSimpleComSetting;
-(BOOL)OpenSimpleCom;
-(BOOL)isOpen;
-(void)closeSimpleCom;

-(bool)WriteString:(const char *)szCMD;
-(void)AddLabelString:(const char *)LabelStr;
-(const char*)ReadString;
-(const char *)ReadStringRegex:(const char*)Regex Timeout:(int)timeout;
-(int)SetDetectString:(const char *)regex;
-(int)WaitForString:(int) iTimeout;
-(void)ClearBuffer;
@end
