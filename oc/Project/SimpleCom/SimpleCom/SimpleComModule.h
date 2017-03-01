//
//  SimpleComModule.h
//  SimpleCom
//
//  Created by ZL-Pro on 16/8/13.
//  Copyright (c) 2016年 ZL-Pro. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ColorButtonView.h"
typedef enum{
    eIITNa = 0,
    eIITSerial = 1,
    eIITGpib = 2,
    eIITLan = 3,
    eIITUsb = 4
    
}eInstrInterfaceType;

@interface SimpleComSetting : NSObject
{
@private

    id m_connectStatusView;
    NSView *m_showlogView;
    NSString* m_InterfaceName;
    eInstrInterfaceType m_Type;
    NSString* m_Setting;
    NSInteger m_isSaveLog;
    NSInteger m_isSendLogtoPDCA;
    NSInteger m_isEcho;
//    NSInteger m_isFixture;
//    NSInteger m_isStartCallLua;
//    NSInteger m_isStopCallLua;
//    NSInteger m_isScanCallLua;
//    NSString* m_StartCallLua;
//    NSString* m_StopCallLua;
//    NSString* m_ScanCallLua;
//    NSArray *m_OtherCallLua;
}
-(id)getconnectStatusView;
-(void)setconnectStatusView:(id)view;
-(id)getLogView;
-(void)SetLogView:(id)view;
-(NSString*)getInterfaceName;
-(eInstrInterfaceType)getType;
-(NSString*)getSetting;
-(NSInteger)isSaveLog;
-(NSInteger)isSendLogtoPDCA;
-(NSInteger)isEcho;
//-(NSInteger)isFixture;
//-(NSInteger)isStartCallLua;
//-(NSInteger)isStopCallLua;
//-(NSInteger)isScanCallLua;
//-(NSString*)getStartCallLua;
//-(NSString*)getStopCallLua;
//-(NSString*)getScanCallLua;
//-(NSArray*)getOtherCallLua;

-(void)setInterfaceName:(NSString*)str;
-(void)setType:(eInstrInterfaceType)str;
-(void)setSetting:(NSString*)str;
-(void)setisSaveLog:(NSInteger)state;
-(void)setisSendLogtoPDCA:(NSInteger)state;
-(void)setisEcho:(NSInteger)state;
//-(void)setisFixture:(NSInteger)state;
//-(void)setisStartCallLua:(NSInteger)state;
//-(void)setisStopCallLua:(NSInteger)state;
//-(void)setisScanCallLua:(NSInteger)state;
//-(void)setStartCallLua:(NSString*)str;
//-(void)setStopCallLua:(NSString*)str;
//-(void)setScanCallLua:(NSString*)str;
//-(void)setOtherCallLuabyString:(NSString*)str;
//-(void)setOtherCallLua:(NSArray*)str;
+(BOOL)isDebugOut;
+(void)setDebugOut:(BOOL)state;
@end

@protocol SimpleComModule <NSObject>
@required
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
@optional

@end

