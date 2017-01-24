//
//  AppDelegate.h
//  IA_BUCK
//
//  Created by Haitao on 22/4/16.
//  Copyright © 2016年 mess. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "CRS232.h"
#import "datalog.h"

@interface AppBuckDelegate : NSObject <NSApplicationDelegate>
{
    IBOutlet NSWindow *mainWnd;
    IBOutlet NSTextField *state0;
    IBOutlet NSTextField *state1;
    IBOutlet NSTextField *state2;
    
    IBOutlet NSTextField *value0;
    IBOutlet NSTextField *value1;
    IBOutlet NSTextField *value2;
    IBOutlet NSTextField *calleft;//左校准值（位置02）
    IBOutlet NSTextField *calright;//右校准值（位置1）
    
    IBOutlet NSButton *start;
    IBOutlet NSButton *stop;
    
    IBOutlet NSButton *check0;
    IBOutlet NSButton *check1;
    IBOutlet NSButton *check2;
    IBOutlet NSButton *calbutton;//锁定校准值按钮
    
    IBOutlet NSTextField *TxtSnInput;

    
    IBOutlet NSWindow * wndUser;
    IBOutlet NSWindow * wndCom;
    IBOutlet NSWindow * wndItemEdit;
    
    IBOutlet NSMenuItem * menuLog;
    IBOutlet NSMenuItem * menuCom;
    IBOutlet NSMenuItem * menuItem;
    
    IBOutlet NSTextField * txtPass;
    IBOutlet NSTextField * txtFail;
    IBOutlet NSTextField * txtTotal;
    IBOutlet NSTextField * txtFpy;
    IBOutlet NSTextField * txtTime;
    
    IBOutlet NSWindow * wndDebug;
    IBOutlet NSComboBox * comboCmd;
    IBOutlet NSTextView * txtView;
    
@private

    CRS232 * pRS232;
    NSTimeInterval startTime;
    datalog * dl;
    NSTimer * timer;
}
-(IBAction)BtStart:(id)sender;
-(IBAction)Btstop:(id)sender;

- (IBAction)btnLogIn:(id)sender;
- (IBAction)btnConfig:(id)sender;
- (IBAction)btnItemEdit:(id)sender;

- (IBAction)btnSend:(id)sender;
@end

