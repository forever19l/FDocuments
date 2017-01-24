//
//  ConfigurationWndDelegate.h
//  DUT
//
//  Created by Ryan on 11/12/15.
//  Copyright © 2015 ___Intelligent Automation___. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface ConfigurationWndDelegate : NSObject{
    IBOutlet NSTableView *  tableView;
    IBOutlet NSButton * btOK;
    IBOutlet NSButton * btCancel;
    
    IBOutlet NSWindow * winConfiguration;
    IBOutlet NSWindow * winMain;
    
    NSMutableDictionary * dicConfiguration;
    int m_Solts;
    
    NSMutableDictionary * dicTableView;
}

-(void)InitCtrls:(NSMutableDictionary *)dic withSolts:(int)number;

@end
