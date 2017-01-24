//
//  LimitSet.m
//  IA_BUCK
//
//  Created by IvanGan on 16/4/25.
//  Copyright © 2016年 mess. All rights reserved.
//

#import "LimitSet.h"
extern NSArray * keyArr;
extern NSMutableDictionary * dicConfig;
extern NSMutableDictionary * dicResult;
extern NSString * strPath;

#import "UICommon.h"
@implementation LimitSet

- (void)awakeFromNib
{
    item = [NSArray arrayWithObjects:txtName1,txtUpper1,txtLower1,txtUnit1,txtName2,txtUpper2,txtLower2,txtUnit2,txtName3,txtUpper3,txtLower3,txtUnit3, nil];
    NSArray * itemKey = [NSArray arrayWithObjects:keyNAME,keyUPPER,keyLOWER,keyUNIT, nil];
    int i = 0;
    for(NSString * key in keyArr)
    {
        for(NSString * ikey in itemKey)
        {
            [[item objectAtIndex:i]setStringValue:[[dicConfig objectForKey:key] objectForKey:ikey]];
            i++;
        }
    }
}

- (IBAction)btnOK:(id)sender
{
    item = [NSArray arrayWithObjects:txtName1,txtUpper1,txtLower1,txtUnit1,txtName2,txtUpper2,txtLower2,txtUnit2,txtName3,txtUpper3,txtLower3,txtUnit3, nil];
    NSArray * itemKey = [NSArray arrayWithObjects:keyNAME,keyUPPER,keyLOWER,keyUNIT, nil];
    int i = 0;
    for(NSString * key in keyArr)
    {
        NSMutableDictionary * tmp = [NSMutableDictionary dictionary];
        for(NSString * ikey in itemKey)
        {
            [tmp setObject:[[item objectAtIndex:i] stringValue] forKey:ikey];
            i++;
        }
        [dicConfig setObject:tmp forKey:key];
        [tmp setObject:@"" forKey:keyRESULT];
        [dicResult setObject:tmp forKey:key];
    }
    [wndLimit orderOut:sender];
//    [dicConfig writeToFile:strPath atomically:YES];
    [wndMain endSheet:wndLimit returnCode:NSModalResponseOK];
}

- (IBAction)btnCancel:(id)sender
{
    [wndMain endSheet:wndLimit returnCode:NSModalResponseCancel];
}

@end
