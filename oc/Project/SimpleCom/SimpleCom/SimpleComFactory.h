//
//  SimpleComFactory.h
//  SimpleCom
//
//  Created by ZL-Pro on 16/8/15.
//  Copyright (c) 2016年 ZL-Pro. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreLib/TestEngine.h>
#import <CoreLib/SerialportEx.h>
#import <CoreLib/ScriptEngine.h>
#import <CoreLib/SerialPortView.h>
#import "SimpleComModule.h"

@interface SimpleComFactory : NSObject<SimpleComModule>
{
    
}
-(NSString*)CheckResourceName;
-(id)initwithName:(SimpleComSetting*)simpleComSetting;
-(void)SetSimpleComSetting:(SimpleComSetting*)simpleComSetting;
-(SimpleComSetting*)GetSimpleComSetting;
-(BOOL)OpenSimpleCom;
-(bool)WriteString:(const char *)szCMD;
-(const char*)ReadString;
-(const char *)ReadStringRegex:(const char*)Regex Timeout:(int)timeout;
-(int)SetDetectString:(const char *)regex;
-(int)WaitForString:(int) iTimeout;
-(void)ClearBuffer;
-(BOOL)isOpen;
@end
