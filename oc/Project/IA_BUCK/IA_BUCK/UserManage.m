//
//  UserManage.m
//  IA_BUCK
//
//  Created by IvanGan on 16/4/25.
//  Copyright © 2016年 mess. All rights reserved.
//

#import "UserManage.h"
#import "UICommon.h"
extern int iAuthor;

@implementation UserManage

- (id)init
{
    self = [super init];
    user = @"admin";
    password = @"admin";
    return self;
}

- (void)dealloc
{
    [super dealloc];
}

- (void)awakeFromNib
{

}

- (IBAction)btnCancel:(id)sender
{
    [winMain endSheet:wndLog returnCode:NSModalResponseCancel];
}

- (IBAction)btnOk:(id)sender
{
    if([txtUser.stringValue isEqualToString:user] && [txtPassword.stringValue isEqualToString:password])
    {
        iAuthor = 2;
        [winMain endSheet:wndLog returnCode:NSModalResponseOK];
    }
    else
    {
        NSDictionary * userInfo = [NSDictionary dictionaryWithObjectsAndKeys:@"Invalide Password !",NSLocalizedDescriptionKey, nil];
        NSError * error = [NSError errorWithDomain:kERRORDOMAIN code:-1 userInfo:userInfo];
        NSAlert * alert = [NSAlert alertWithError:error];
        [alert runModal];
    }
        
}

@end
