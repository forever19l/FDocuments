//
//  SimpleComCfgDelegate.cpp
//  SimpleCom
//
//  Created by ZL Meng 2016-08-16.
//  Copyright (c) 2016年 ZL Meng. All rights reserved.
//


#import "SimpleComCfgDelegate.h"
#include "SimpleComWinDelegate.h"


@implementation SimpleComCfgWinDelegate

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
    
}

- (void)windowDidBecomeMain:(NSNotification *)notification
{
    SimpleComWinDelegate * main = (SimpleComWinDelegate *)[winSimpleCom delegate];
    
    [self InitialCtls:[main dicConfiguration]];
    
}


-(int)InitialCtls:(NSMutableDictionary *)dicConfiguration
{
    return 0;
}

-(int)SaveConfig
{
    SimpleComWinDelegate * main = (SimpleComWinDelegate *)[winSimpleCom delegate];
    [[main dicConfiguration] writeToFile:[main strCfgFile] atomically:YES];
    return 0;
}


-(IBAction)btOK:(id)sender
{
    [NSApp stopModalWithCode:1];        //OK
    [winConfig orderOut:nil];//隐藏登录窗口
    [winSimpleCom makeKeyAndOrderFront:nil];
    [self SaveConfig];
}
-(IBAction)btCancel:(id)sender
{
    [NSApp stopModalWithCode:0];        //Cancel
    [winConfig orderOut:nil];//隐藏登录窗口
    [winSimpleCom makeKeyAndOrderFront:nil];
}

-(BOOL) windowShouldClose:(NSWindow *)sender
{
    /*
    [winConfig orderOut:nil];
    [NSApp endSheet:winConfig returnCode:1];
    return NO;*/
    
    [NSApp stopModalWithCode:0];        //Cancel
    [winConfig orderOut:nil];//隐藏登录窗口
    [winSimpleCom makeKeyAndOrderFront:nil];
    return NO;
}

-(IBAction)btRescan:(id)sender
{
}
@end
