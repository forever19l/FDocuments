//
//  ConfigurationWndDelegate.m
//  IIC
//
//  Created by Liang on 1-3-16.
//  Copyright (c) 2016年 FOREVER.Z. All rights reserved.
//

#import "IICConfigWinDelegate.h"
#import "AppDelegate.h"

@implementation IICConfigWinDelegate

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
    [self btIICRescan:self];
}


-(BOOL) windowShouldClose:(NSWindow *)sender
{
    
    [NSApp stopModalWithCode:0];        //Cancel
    [winIICConfig orderOut:nil];    //隐藏登录窗口
    [winIICFixture makeKeyAndOrderFront:nil];
    return NO;
}


- (void)windowDidBecomeMain:(NSNotification *)notification
{
    IICController* main = (IICController*)[winIICFixture delegate];
    
    [self InitialCtls:[main dicConfiguration]];
}


-(int)InitialCtls:(NSMutableDictionary *)dicConfiguration
{
    [comboxIICComPort setStringValue:[dicConfiguration valueForKey:KEY_SPIIC]];
    
    [txtIICSetting setStringValue:[dicConfiguration valueForKey:KEY_SETTING_IIC]];
    
    [self btIICRescan:self];
    return 0;
}

-(int)SaveConfig
{
    IICController * main = (IICController *)[winIICFixture delegate];
    
    NSString* str = [comboxIICComPort stringValue];
    
    [[main dicConfiguration]setValue:str forKey:KEY_SPIIC];
    
    
    [[main dicConfiguration]setValue:[txtIICSetting stringValue] forKey:KEY_SETTING_IIC];
    
    [[main dicConfiguration] writeToFile:[main strCfgFile] atomically:YES];
    return 0;
}

-(IBAction)btIICOK:(id)sender
{
    [winIICConfig orderOut:nil];
    [self SaveConfig];
    [NSApp endSheet:winIICConfig returnCode:1];
    return;
    [NSApp stopModalWithCode:1];        //OK
    [winIICConfig orderOut:nil];        //隐藏登录窗口
    [winIICFixture makeKeyAndOrderFront:nil];
    [self SaveConfig];
}

-(IBAction)btIICCancel:(id)sender
{
    [NSApp endSheet:winIICConfig returnCode:0];
    [winIICConfig orderOut:nil];
    return;
    [NSApp stopModalWithCode:0];        //Cancel
    [winIICConfig orderOut:nil];        //隐藏登录窗口
    [winIICFixture makeKeyAndOrderFront:nil];
}

-(IBAction)btIICRescan:(id)sender
{
    [comboxIICComPort removeAllItems];
    NSEnumerator *enumerator = [AMSerialPortList portEnumerator];
    AMSerialPort *aPort;
    while (aPort = [enumerator nextObject]) {
        [comboxIICComPort addItemWithObjectValue:[aPort bsdPath]];
    }
}

@end
