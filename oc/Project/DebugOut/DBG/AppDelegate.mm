//
//  AppDelegate.m
//  DUT
//
//  Created by Ryan on 11/2/15.
//  Copyright © 2015 ___Intelligent Automation___. All rights reserved.
//

#import "AppDelegate.h"
#import "UnitViewController.h"
#import <MMTabBarView/UnitFakeModel.h>

#include "ConfigurationWndDelegate.h"


@interface AppDelegate ()

//@property (weak) IBOutlet NSWindow *window;
@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
    for (NSViewController * v in arrUnit)
    {
        [v release];
    }
    [arrUnit release];
}

#pragma mark ---- tab bar config ----
-(int)LoadDefine
{
    NSBundle * bundle = [NSBundle bundleForClass:[self class]];
    NSString * path = [bundle pathForResource:@"define" ofType:@"plist"];
    m_dicDefine = [[NSMutableDictionary alloc] initWithContentsOfFile:path];
    if (!m_dicDefine) {
        [m_dicDefine setValue:@"utils" forKey:kProjectName];
        [m_dicDefine setValue:@"6" forKey:kSoltsNumber];
    }
    return 0;
}

-(void)awakeFromNib
{
    [self LoadDefine];
    [self LoadConfiguration];
    arrUnit = [NSMutableArray new];
    [tabdbgBar setStyleNamed:@"Safari"];
    [tabdbgBar setOnlyShowCloseOnHover:YES];
    [tabdbgBar setCanCloseOnlyTab:YES];
    [tabdbgBar setDisableTabClose:YES];
    [tabdbgBar setAllowsBackgroundTabClosing:YES];
    [tabdbgBar setHideForSingleTab:YES];
    [tabdbgBar setShowAddTabButton:YES];
    [tabdbgBar setSizeButtonsToFit:NO];
    
    [tabdbgBar setTearOffStyle:MMTabBarTearOffAlphaWindow];
    [tabdbgBar setUseOverflowMenu:YES];
    [tabdbgBar setAutomaticallyAnimates:YES];
    [tabdbgBar setAllowsScrubbing:YES];
    [tabdbgBar setButtonOptimumWidth:200];
    
    [self CreateUnit:[[m_dicDefine valueForKey:kSoltsNumber] intValue]];
}

-(void)CreateUnit:(int)count
{
    for (int i=0; i<count; i++) {
        [self addNewTabWithTitle:[NSString stringWithFormat:@"UUT%d",i+1] withIndex:i];
    }
    [tabdbgView selectFirstTabViewItem:nil];
}

-(void)SetUpUnit:(int)index withUnit:(UnitDBGViewController *)controller
{

    NSString * key2 = [NSString stringWithFormat:@"%@%d",kSubscribe,index];
    
    NSDictionary * dic = [NSDictionary dictionaryWithObjectsAndKeys:[m_dicConfiguration valueForKey:key2],kSubscribe, nil];
    if ([[[dic keyEnumerator]allObjects]count]==0) {
        dic = [NSDictionary dictionaryWithObjectsAndKeys:@"tcp:0.0.0.0:0",kSubscribe, nil];
    }
    [controller InitialPort:dic];
}

- (void)addNewTabWithTitle:(NSString *)aTitle withIndex:(int)index
{
    UnitDBGViewController * c = [[UnitDBGViewController alloc] initialwithID:index];
    [self SetUpUnit:index withUnit:c];

    [arrUnit addObject:c];
    
    UnitFakeModel *newModel = [[UnitFakeModel alloc] init];
    [newModel setTitle:aTitle];
    NSTabViewItem *newItem = [[NSTabViewItem alloc] initWithIdentifier:newModel];
    [newItem setLabel:aTitle];
    [tabdbgView addTabViewItem:newItem];
    [tabdbgView selectTabViewItem:newItem];
    [newItem setView:c.view];
//    [c.view setFrame:[view frame]];
    [newModel release];
    [newItem release];
    //[c release];
}

-(IBAction)btConfiguration:(id)sender
{
    [windbgMain beginSheet:windbgConfiguration completionHandler:^(NSModalResponse returnCode) {
        switch (returnCode) {
            case NSModalResponseOK:
                [self SaveConfiguration:m_dicConfiguration];
                for (int i=0;i<[arrUnit count];i++)
                {
                    [self SetUpUnit:i withUnit:[arrUnit objectAtIndex:i]];
                }
                break;
            case NSModalResponseCancel:
                break;
            default:
                break;
        }
    }];
}

-(void )LoadConfiguration
{
    //Load configuration in here
    m_dicConfiguration = [NSMutableDictionary new];
//    NSString * config_path = [NSString stringWithFormat:@"/Vault/%@_config/dut_config.plist",[m_dicDefine valueForKey:kProjectName]];
    NSString * p = [[[[NSBundle mainBundle]bundlePath]stringByDeletingLastPathComponent]stringByAppendingPathComponent:@"TM_config/DebugPanle"];
    if(![[NSFileManager defaultManager]fileExistsAtPath:p])
        [[NSFileManager defaultManager]createDirectoryAtPath:p withIntermediateDirectories:YES attributes:NULL error:NULL];
    NSString * config_path = [NSString stringWithFormat:@"%@/dut_config.plist",p];
    
    if (m_dicConfiguration) {
        [m_dicConfiguration release];
    }
    m_dicConfiguration = [[NSMutableDictionary alloc] initWithContentsOfFile:config_path];
    
    if (!m_dicConfiguration)
    {
        m_dicConfiguration = [NSMutableDictionary new];
        int chn = [[m_dicDefine valueForKey:kSoltsNumber] intValue];
        for (int i=0;i<chn;i++)
        {
            NSString * sub = [NSString stringWithFormat:@"tcp://127.0.0.1:%d",6800+i ];
            [m_dicConfiguration setValue:sub forKey:[NSString stringWithFormat:@"%@%d",kSubscribe,i]];
        }
    }
}

-(void)SaveConfiguration:(NSMutableDictionary *)dic
{
    NSString * config_path = [NSString stringWithFormat:@"/Vault/%@_config/dut_config.plist",[m_dicDefine valueForKey:kProjectName]];
    NSError * err;
    if (![[NSFileManager defaultManager] createDirectoryAtPath:[config_path stringByDeletingLastPathComponent] withIntermediateDirectories:YES attributes:nullptr error:&err])
    {
        NSRunAlertPanel(@"Save Configuration", @"Create config path failed,with file path:%@ with error: %@", @"OK", nil, nil,config_path,[err description]);
    }
    if (![dic writeToFile:config_path atomically:YES])
    {
        NSRunAlertPanel(@"Save Configuration", @"Write configuration file failed,with file path:%@", @"OK", nil, nil,config_path);
    }
}

- (void)windowWillBeginSheet:(NSNotification *)notification
{
    [(ConfigurationWndDelegate *)[windbgConfiguration delegate] InitCtrls:m_dicConfiguration withSolts:[[m_dicDefine valueForKey:kSoltsNumber] intValue]];
}
@end
