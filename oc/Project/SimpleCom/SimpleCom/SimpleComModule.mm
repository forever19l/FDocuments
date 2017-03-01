//
//  SimpleComModule.h
//  SimpleCom
//
//  Created by ZL-Pro on 16/8/13.
//  Copyright (c) 2016年 ZL-Pro. All rights reserved.
//

#import "SimpleComModule.h"

static BOOL m_isDebugOut = NO;

@implementation SimpleComSetting

+(BOOL)isDebugOut
{
    return m_isDebugOut;
}

+(void)setDebugOut:(BOOL)state
{
    m_isDebugOut = state;
}

-(id)getconnectStatusView
{
    return m_connectStatusView;
}
-(void)setconnectStatusView:(id)view
{
    id old=m_connectStatusView;
    m_connectStatusView=[view retain];
    [old release];
}
-(id)getLogView
{
    return m_showlogView;
}
-(void)SetLogView:(id)view
{
    id old=m_showlogView;
    m_showlogView=[view retain];
    [old release];
}
-(NSString*)getInterfaceName
{
    return m_InterfaceName;
}
-(eInstrInterfaceType)getType
{
    return m_Type;
}
-(NSString*)getSetting
{
    return m_Setting;
}
-(NSInteger)isSaveLog
{
    return m_isSaveLog;
}
-(NSInteger)isSendLogtoPDCA
{
    return m_isSendLogtoPDCA;
}
-(NSInteger)isEcho
{
    return m_isEcho;
}
//-(NSInteger)isFixture
//{
//    return m_isFixture;
//}
//-(NSInteger)isStartCallLua
//{
//    return m_isStartCallLua;
//}
//-(NSInteger)isStopCallLua
//{
//    return m_isStopCallLua;
//}
//-(NSInteger)isScanCallLua
//{
//    return m_isScanCallLua;
//}
//-(NSString*)getStartCallLua
//{
//    return m_StartCallLua;
//}
//-(NSString*)getStopCallLua
//{
//    return m_StopCallLua;
//}
//-(NSString*)getScanCallLua
//{
//    return m_ScanCallLua;
//}
//-(NSArray*)getOtherCallLua
//{
//    return m_OtherCallLua;
//}

-(void)setInterfaceName:(NSString*)str
{
    m_InterfaceName=str;
}
-(void)setType:(eInstrInterfaceType)str
{
    m_Type=str;
}
-(void)setSetting:(NSString*)str
{
    m_Setting=str;
}
-(void)setisSaveLog:(NSInteger)state
{
    m_isSaveLog=state;
}
-(void)setisSendLogtoPDCA:(NSInteger)state
{
    m_isSendLogtoPDCA=state;
}

-(void)setisEcho:(NSInteger)state
{
    m_isEcho=state;
}
//-(void)setisFixture:(NSInteger)state
//{
//    m_isFixture=state;
//}
//-(void)setisStartCallLua:(NSInteger)state
//{
//    m_isStartCallLua=state;
//}
//-(void)setisStopCallLua:(NSInteger)state
//{
//    m_isStopCallLua=state;
//}
//-(void)setisScanCallLua:(NSInteger)state
//{
//    m_isScanCallLua=state;
//}
//-(void)setStartCallLua:(NSString*)str
//{
//    m_StartCallLua=str;
//}
//-(void)setStopCallLua:(NSString*)str
//{
//    m_StopCallLua=str;
//}
//-(void)setScanCallLua:(NSString*)str
//{
//    m_ScanCallLua=str;
//}
//-(void)setOtherCallLuabyString:(NSString*)str
//{
//    m_OtherCallLua=[[str componentsSeparatedByString:@"\n"] retain];
//}
//-(void)setOtherCallLua:(NSArray*)str
//{
//    m_OtherCallLua=[str retain];
//}

@end
