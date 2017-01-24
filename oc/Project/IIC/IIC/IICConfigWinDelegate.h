//
//  ConfigurationWndDelegate.h
//  IIC
//
//  Created by FOREVER on 1/3/16.
//  Copyright © 2016  FOREVER.Z. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface IICConfigWinDelegate : NSObject{
@private
    IBOutlet NSWindow * winIICConfig;
    IBOutlet NSWindow * winIICFixture;
    
    IBOutlet NSComboBox * comboxIICComPort;
    
    IBOutlet NSTextField * txtIICSetting;
}

-(IBAction)btIICOK:(id)sender;
-(IBAction)btIICCancel:(id)sender;
-(IBAction)btIICRescan:(id)sender;

-(int)InitialCtls:(NSMutableDictionary *)dicConfiguration;
-(int)SaveConfig;

@end
