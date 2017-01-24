//
//  ConfigurationWndDelegate.h
//  HLY
//
//  Created by FOREVER.Z on 12/4/1016.
//  Copyright © 2016  FOREVER.Z. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface HLYConfigWinDelegate : NSObject{
@private
    IBOutlet NSWindow * winHLYConfig;
    IBOutlet NSWindow * winHLYFixture;
    
    IBOutlet NSComboBox * comboxHLYComPort;
    
    IBOutlet NSTextField * txtHLYSetting;
}

-(IBAction)btHLYOK:(id)sender;
-(IBAction)btHLYCancel:(id)sender;
-(IBAction)btHLYRescan:(id)sender;

-(int)InitialCtls:(NSMutableDictionary *)dicConfiguration;
-(int)SaveConfig;

@end
