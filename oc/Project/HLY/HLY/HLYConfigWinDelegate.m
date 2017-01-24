//
//  ConfigurationWndDelegate.m
//  HLY
//
//  Created by FOREVER.Z on 12/4/2016.
//  Copyright (c) 2016年 FOREVER.Z. All rights reserved.
//

#import "HLYConfigWinDelegate.h"
#import "AppDelegate.h"

@implementation HLYConfigWinDelegate

- (id)init
{
    self = [super init];
    if (self) {
        // Initialization code here.
    }
    
    return self;
}

- (void)dealloc
{
    [super dealloc];
}

-(void)awakeFromNib
{
    [self btHLYRescan:self];
}


-(BOOL) windowShouldClose:(NSWindow *)sender
{
    
    [NSApp stopModalWithCode:0];        //Cancel
    [winHLYConfig orderOut:nil];    //隐藏登录窗口
    [winHLYFixture makeKeyAndOrderFront:nil];
    return NO;
}


- (void)windowDidBecomeMain:(NSNotification *)notification
{
    HLYController* main = (HLYController*)[winHLYFixture delegate];
    
    [self InitialCtls:[main dicConfiguration]];
}


-(int)InitialCtls:(NSMutableDictionary *)dicConfiguration
{
    [comboxHLYComPort setStringValue:[dicConfiguration valueForKey:KEY_SPHLY]];
    [txtHLYSetting setStringValue:[dicConfiguration valueForKey:KEY_SETTING_HLY]];
    
    [self btHLYRescan:self];
    return 0;
}

-(int)SaveConfig
{
    HLYController * main = (HLYController *)[winHLYFixture delegate];
    
    NSString* str = [comboxHLYComPort stringValue];
    
    [[main dicConfiguration]setValue:str forKey:KEY_SPHLY];
    
    
    [[main dicConfiguration]setValue:[txtHLYSetting stringValue] forKey:KEY_SETTING_HLY];
    
    [[main dicConfiguration] writeToFile:[main strCfgFile] atomically:YES];
    return 0;
}

-(IBAction)btHLYOK:(id)sender
{
    [winHLYConfig orderOut:nil];
    [self SaveConfig];
    [NSApp endSheet:winHLYConfig returnCode:1];
    return;
    [NSApp stopModalWithCode:1];        //OK
    [winHLYConfig orderOut:nil];        //隐藏登录窗口
    [winHLYFixture makeKeyAndOrderFront:nil];
    [self SaveConfig];
}

-(IBAction)btHLYCancel:(id)sender
{
    [NSApp endSheet:winHLYConfig returnCode:0];
    [winHLYConfig orderOut:nil];
    return;
    [NSApp stopModalWithCode:0];        //Cancel
    [winHLYConfig orderOut:nil];        //隐藏登录窗口
    [winHLYFixture makeKeyAndOrderFront:nil];
}

-(IBAction)btHLYRescan:(id)sender
{
    [comboxHLYComPort removeAllItems];
    NSEnumerator *enumerator = [AMSerialPortList portEnumerator];
    AMSerialPort *aPort;
    while (aPort = [enumerator nextObject]) {
        [comboxHLYComPort addItemWithObjectValue:[aPort bsdPath]];
    }
}

@end
