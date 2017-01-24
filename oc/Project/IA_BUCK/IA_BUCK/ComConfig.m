//
//  ComConfig.m
//  IA_BUCK
//
//  Created by IvanGan on 16/4/25.
//  Copyright © 2016年 mess. All rights reserved.
//

#import "ComConfig.h"
#import "UICommon.h"
#import "ListSerialPort.h"
extern NSMutableDictionary * dicConfig;

@implementation ComConfig

- (void)awakeFromNib
{
    [txtPort setStringValue:[dicConfig objectForKey:keyPORT ]];
    [txtSetting setStringValue:[dicConfig objectForKey:keySETTING]];
}

- (IBAction)btnCancel:(id)sender
{
    [winMain endSheet:winCom returnCode:NSModalResponseCancel];
}


-(IBAction)btnOk:(id)sender
{
//    NSString * port = [dicConfig objectForKey:keyPORT];
//    NSString * setting = [dicConfig objectForKey:keySETTING];
//    
//    if([port isNotEqualTo:txtPort.stringValue] || [setting isNotEqualTo:txtSetting.stringValue])
//        bComChanged = YES;
//    else
//        bComChanged = NO;
//    
    [dicConfig setObject:txtPort.stringValue forKey:keyPORT];
    [dicConfig setObject:txtSetting.stringValue forKey:keySETTING];
    [winMain endSheet:winCom returnCode:NSModalResponseOK];
}

- (IBAction)btnRefresh:(id)sender
{
    NSMutableArray * list = [NSMutableArray arrayWithArray:[ListSerialPort ListSerialPort]];
    [txtPort addItemsWithObjectValues:list];
}

@end
