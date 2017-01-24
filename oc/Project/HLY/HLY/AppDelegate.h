//
//  AppDelegate.h
//  DUT
//
//  Created by Ryan on 11/2/15.
//  Copyright © 2015 ___Intelligent Automation___. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "SerialportEx.h"
#import "SerialPortView.h"
#import "uixTypeDef.h"
#include "HLYConfigWinDelegate.h"
#include "Fixture.h"


#define KEY_SPHLY       @"spHLY"
#define KEY_SETTING_HLY @"settingHLY"


@interface HLYController : NSObject<NSApplicationDelegate>
{
    IBOutlet SerialPortView* _HLYView;      //串口消息显示
    IBOutlet NSWindow * winHLYConfig;       //串口config配置
    IBOutlet NSWindow * winHLYMain;         //主界面设定

    IBOutlet NSTextField * HLY_DAC1;
    IBOutlet NSTextField * HLY_DAC2;
    IBOutlet NSTextView * HLY_ISETHK;
    IBOutlet NSTextView * HLY_ISETHE;
    IBOutlet NSTextView * HLY_IREADHK;
    IBOutlet NSTextView * HLY_IREADHE;
    IBOutlet NSTextView * HLY_VALUE;
    
    IBOutlet NSButton * DACSET1;            //
    IBOutlet NSButton * DACSET2;            //
    IBOutlet NSButton * IAREAD;
    IBOutlet NSButton * BACKREAD;
    
    SerialportEx * m_pSerialPort;           //串口的object
    
    NSMutableDictionary * diconfiguration; //保存config的字典
    NSString * strCfgFile;                  //config文件路径
    
    SEL m_TestManagerCallBack;
    id m_CallbackTarget;
    
    NSMutableString* m_FixtureID;
    NSMutableString* m_FixtureVer;
    
    NSFileHandle * m_handle;                //for debug
@public
    NSString * dac1;
    NSString * dac2;
    NSString * ia1;
    NSString * ia2;
    NSString * back1;
    NSString * back2;
    
@private
    NSString * filepath;
    NSString * fileName;
}
@property (assign) NSMutableDictionary * dicConfiguration;
@property (copy)   NSString * strCfgFile;

-(void)SetCallback:(SEL)Callback Target:(id)object;
-(void)showPanel:(id)sender;
-(void)clearallview;
-(BOOL)isPureFloat:(NSString*)string;

-(IBAction)btHLYConfig:(id)sender;

-(IBAction)DACSET1:(id)sender;
-(IBAction)DACSET2:(id)sender;
-(IBAction)IAREAD:(id)sender;
-(IBAction)BACKREAD:(id)sender;


@end
