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
#include "IICConfigWinDelegate.h"
#include "Fixture.h"


#define KEY_SPIIC       @"spIIC"
#define KEY_SETTING_IIC @"settingIIC"


@interface IICController : NSObject<NSApplicationDelegate>
{
    IBOutlet SerialPortView * _IICView;      //串口消息显示
    IBOutlet NSWindow * winIICConfig;       //串口config配置
    IBOutlet NSWindow * winIICMain;         //主界面设定
//    IBOutlet NSTextView * IICResult;        //搜索对比结果显示
//    IBOutlet NSTextView * IICData;          //TXT消息显示
//    IBOutlet NSTextField * IICpath;         //权限及sn维护框
    
//    IBOutlet NSButton * IICREAD;         //触发搜索比较事件
    IBOutlet NSButton * IICCheckin;
    IBOutlet NSButton * IICCheckout;
    IBOutlet NSButton * IICRESET;
//    IBOutlet NSButton * flagreset;
    
    SerialportEx * m_pSerialPort;           //串口的object

    NSMutableString * m_Readdata;           //txt读到的动态data
    
    NSMutableDictionary * dicConfiguration; //保存config的字典
    NSString * strCfgFile;                  //config文件路径
    
    SEL m_TestManagerCallBack;
    id m_CallbackTarget;
    
    NSMutableString* m_FixtureID;
    NSMutableString* m_FixtureVer;
    
    NSFileHandle * m_handle;                //for debug
    
    
    //detail reset
    IBOutlet SerialPortView * d1;
    IBOutlet SerialPortView * d2;
    IBOutlet SerialPortView * d3;
    IBOutlet SerialPortView * d4;
    IBOutlet SerialPortView * d5;
    IBOutlet SerialPortView * d6;
    IBOutlet SerialPortView * d7;
    IBOutlet SerialPortView * d8;
    IBOutlet SerialPortView * d9;
    IBOutlet SerialPortView * d10;
    IBOutlet SerialPortView * d11;
    IBOutlet SerialPortView * d12;
    IBOutlet SerialPortView * d13;
    IBOutlet SerialPortView * d14;
    IBOutlet SerialPortView * d15;
    IBOutlet SerialPortView * d16;
    IBOutlet SerialPortView * d17;
    IBOutlet SerialPortView * d18;
    IBOutlet SerialPortView * d19;
    IBOutlet SerialPortView * d20;
    IBOutlet SerialPortView * d21;
    IBOutlet SerialPortView * d22;
    IBOutlet SerialPortView * d23;
    IBOutlet SerialPortView * d24;
    IBOutlet SerialPortView * d25;
    IBOutlet SerialPortView * d26;
    IBOutlet SerialPortView * d27;
    IBOutlet SerialPortView * d28;
    IBOutlet SerialPortView * d29;
    IBOutlet NSWindow * winflagreset;
    IBOutlet NSWindow * winUser;

@public
//    NSMutableString *IICACTSN;              //全局操作的sn，添加删除

@private
    NSString * filepath;
    NSString * ShareFolderName;
//    NSString * fileName;
}
@property (assign) NSMutableDictionary * dicConfiguration;
@property (copy)   NSString * strCfgFile;
@property (assign) NSString * SN;
@property (assign) bool DynamicCheck,MFlagCheck;
@property (assign) NSMutableArray * FailItem;

-(void)SetCallback:(SEL)Callback Target:(id)object;
-(void)showPanel:(id)sender;
-(void)clearallview;

-(IBAction)btIICConfig:(id)sender;
//-(IBAction)IICREAD:(id)sender;
//-(IBAction)IICRESET:(id)sender;


@end
