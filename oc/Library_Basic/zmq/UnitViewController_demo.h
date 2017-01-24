//
//  UnitViewController.h
//  DUT
//
//  Created by Ryan on 11/3/15.
//  Copyright © 2015 ___Intelligent Automation___. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include "DutController.h"

@protocol DataInterface <NSObject>
-(void)OnData:(void *)bytes length:(long)len;
@end

@interface UnitViewController<DataInterface> : NSViewController{
    IBOutlet NSComboBox * comboxCmd;
    IBOutlet NSTextView * txtBuffer;
    int mID;
    
    NSMutableDictionary * m_dicConfiguration;
    
    CDUTController * pDUT;
}
-(IBAction)btSend:(id)sender;
-(id)initialwithID:(int)ID;
-(void)OnData:(void *)bytes length:(long)len;
-(int)InitialPort:(NSDictionary *)diConfig;

@end
