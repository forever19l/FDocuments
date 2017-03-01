//
//  SimpleComWinDelegate.h
//  SimpleCom
//
//  Created by Liu Liang on 13-12-23.
//  Copyright (c) 2013年 Liu Liang. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SimpleCom_Global.h"
#import <CoreLib/TestEngine.h>
#import <CoreLib/SerialportEx.h>
#import <CoreLib/ScriptEngine.h>
#import <CoreLib/SerialPortView.h>


@interface SimpleComWinDelegate: NSObject
{
@private
//    NSTabViewItem *m_tabViewItem_Modul;
    
    
    
    
    IBOutlet NSView *m_tabitemview;
    IBOutlet NSTabView *m_tabview;
    IBOutlet NSBox *m_boxSerialport;
    IBOutlet NSBox *m_boxNetword;
    
    IBOutlet NSMenuItem* menuItem ;
    IBOutlet NSWindow* mWinDebugPanel ;
    IBOutlet NSWindow* mWinConfigPanel ;
    IBOutlet NSTextView* mLog;
    
    
    IBOutlet NSComboBox* combSimpleCom;
    IBOutlet NSComboBox* combSimpleComType;

    
    IBOutlet NSTextField* txtSimpleComName;
    
    IBOutlet NSButton *btScanSerialport;
    IBOutlet NSComboBox* combSerialPortDevice;
    IBOutlet NSTextField* txtSettingLable;
    IBOutlet NSTextField* txtSetting;
    IBOutlet NSTextField* txtSerail_log;
    
    IBOutlet NSTextField* txtServerIPLable;
    IBOutlet NSTextField* txtServerIP;
    IBOutlet NSTextField* txtPortLable;
    IBOutlet NSTextField* txtPort;

    IBOutlet NSButton *btIsSendLogtoPDCA;
    IBOutlet NSButton *btIsSaveLog;
    IBOutlet NSButton *btIsEcho;
    IBOutlet NSButton *btCreateInterface;
    
    
    IBOutlet NSButton *btIsFixtrue;
    IBOutlet NSButton *btIsStartCallLua;
    IBOutlet NSButton *btIsStopCallLua;
    IBOutlet NSButton *btIsScanCallLua;
    
    IBOutlet NSTextField* txtStart;
    IBOutlet NSTextField* txtStop;
    IBOutlet NSTextField* txtScan;
    IBOutlet NSTextField* txtOther;

    //here is config view outlet
    NSMutableDictionary *dicConfiguration ;
    NSString * strCfgFile;
    
    int DebugMsgCounter;
    NSMutableArray* arrCommands;
}

@property (assign) NSMutableDictionary * dicConfiguration;
@property (copy) NSString * strCfgFile;

-(void)PrintLog:(NSString*)strMsg;

-(IBAction)btSend:(id)sender;

-(IBAction)menu_ShowDebugPanel:(id)sender;
-(IBAction)menu_ShowConfigPanel:(id)sender;
-(IBAction)bt_IsSendLogtoPDCAClick:(id)sender;

-(IBAction)bt_IsFixtrueClick:(id)sender;
-(IBAction)OnChangeInterfaceType:(id)sender;
-(IBAction)btn_SearchDUTDevice:(id)sender;

-(IBAction)btCreateSimpleCom:(id)sender;
-(IBAction)btSaveConfig:(id)sender;
-(IBAction)btRemoveSimpleCom:(id)sender;
-(IBAction)btRemoveAllSimpleComs:(id)sender;
-(IBAction)chonSelectText:(id)sender;
-(IBAction)btSetDebugOut:(id)sender;

-(IBAction)btSetCheckBox:(id)sender;
@end

