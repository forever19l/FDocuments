//
//  UserManage.h
//  IA_BUCK
//
//  Created by IvanGan on 16/4/25.
//  Copyright © 2016年 mess. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

@interface UserManage : NSObject
{
    IBOutlet NSWindow * wndLog;
    IBOutlet NSWindow * winMain;
    IBOutlet NSTextField * txtUser;
    IBOutlet NSTextField * txtPassword;
@private
    NSString * user;
    NSString * password;
}

- (IBAction)btnCancel:(id)sender;
- (IBAction)btnOk:(id)sender;

@end
