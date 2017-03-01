//
//  SimpleComFactory.m
//  SimpleCom
//
//  Created by ZL-Pro on 16/8/15.
//  Copyright (c) 2016年 ZL-Pro. All rights reserved.
//

#import "SimpleComFactory.h"

@implementation SimpleComFactory
-(NSString*)CheckResourceName
{
    @throw [NSException exceptionWithName:@"Overrid error" reason:@"you should overrid getCheckResourceName function first!" userInfo:nil];
    return nil;
}
-(id)initwithName:(SimpleComSetting*)simpleComSetting
{
    @throw [NSException exceptionWithName:@"Overrid error" reason:@"you should overrid initwithName function first!" userInfo:nil];
    return nil;
}
-(SimpleComSetting*)GetSimpleComSetting
{
    @throw [NSException exceptionWithName:@"Overrid error" reason:@"you should overrid GetSimpleComSetting function first!" userInfo:nil];
    return nil;
}
-(void)SetSimpleComSetting:(SimpleComSetting*)simpleComSetting
{
    @throw [NSException exceptionWithName:@"Overrid error" reason:@"you should overrid SetSimpleComSetting function first!" userInfo:nil];
    return;
}
-(BOOL)OpenSimpleCom
{
    @throw [NSException exceptionWithName:@"Overrid error" reason:@"you should overrid OpenSimpleCom function first!" userInfo:nil];
    return YES;
}
-(bool)WriteString:(const char *)szCMD
{
    @throw [NSException exceptionWithName:@"Overrid error" reason:@"you should overrid WriteString function first!" userInfo:nil];
    return YES;
}
-(const char *)ReadStringRegex:(const char*)Regex Timeout:(int)timeout
{
    @throw [NSException exceptionWithName:@"Overrid error" reason:@"you should overrid ReadStringRegex function first!" userInfo:nil];
    return NULL;
}
-(const char*)ReadString
{
    @throw [NSException exceptionWithName:@"Overrid error" reason:@"you should overrid ReadString function first!" userInfo:nil];
    return NULL;
}
-(int)SetDetectString:(const char *)regex
{
    @throw [NSException exceptionWithName:@"Overrid error" reason:@"you should overrid SetDetectString function first!" userInfo:nil];
    return 0;
}
-(int)WaitForString:(int) iTimeout
{
    @throw [NSException exceptionWithName:@"Overrid error" reason:@"you should overrid WaitForString function first!" userInfo:nil];
    return 0;
}
-(void)ClearBuffer
{
    @throw [NSException exceptionWithName:@"Overrid error" reason:@"you should overrid ClearBuffer function first!" userInfo:nil];
}
-(BOOL)isOpen
{
    @throw [NSException exceptionWithName:@"Overrid error" reason:@"you should overrid isOpen function first!" userInfo:nil];
}
@end
