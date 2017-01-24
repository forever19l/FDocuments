//
//  UnitViewController.h
//  DUT
//
//  Created by Ryan on 11/3/15.
//  Copyright © 2015 ___Intelligent Automation___. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include "DBGController.h"

@protocol DataInterface <NSObject>
-(void)OnData:(void *)bytes length:(long)len;
@end

@interface UnitDBGViewController<DataInterface> : NSViewController{
    IBOutlet NSTextView * txtdbgBuffer;
    int mID;
    
    NSMutableDictionary * m_dicConfiguration;
    
    CDBGController * pDBG;
}
-(id)initialwithID:(int)ID;
-(void)OnData:(void *)bytes length:(long)len;
-(int)InitialPort:(NSDictionary *)diConfig;

@end
