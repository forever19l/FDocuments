//
//  ComConfig.h
//  IA_BUCK
//
//  Created by IvanGan on 16/4/25.
//  Copyright © 2016年 mess. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

@interface ComConfig : NSObject
{
    IBOutlet NSButton * btnOk;
    IBOutlet NSButton * btnCancel;
    
    IBOutlet NSComboBox * txtPort;
    IBOutlet NSTextField * txtSetting;
    
    IBOutlet NSWindow * winMain;
    IBOutlet NSWindow * winCom;
}

- (IBAction)btnOk:(id)sender;
- (IBAction)btnCancel:(id)sender;
- (IBAction)btnRefresh:(id)sender;

@end
