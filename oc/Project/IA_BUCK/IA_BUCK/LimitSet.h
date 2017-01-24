//
//  LimitSet.h
//  IA_BUCK
//
//  Created by IvanGan on 16/4/25.
//  Copyright © 2016年 mess. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

@interface LimitSet : NSObject
{
    IBOutlet NSWindow * wndLimit;
    IBOutlet NSWindow * wndMain;
    IBOutlet NSTextField * txtName1;
    IBOutlet NSTextField * txtName2;
    IBOutlet NSTextField * txtName3;
    
    IBOutlet NSTextField * txtUpper1;
    IBOutlet NSTextField * txtUpper2;
    IBOutlet NSTextField * txtUpper3;
    
    IBOutlet NSTextField * txtLower1;
    IBOutlet NSTextField * txtLower2;
    IBOutlet NSTextField * txtLower3;
    
    IBOutlet NSTextField * txtUnit1;
    IBOutlet NSTextField * txtUnit2;
    IBOutlet NSTextField * txtUnit3;
    
    NSArray * item;
}

- (IBAction)btnOK:(id)sender;
- (IBAction)btnCancel:(id)sender;
@end
