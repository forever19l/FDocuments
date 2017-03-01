//
//  SimpleComCfgDelegate.h
//  SimpleCom
//
//  Created by ZL Meng 2016-08-16.
//  Copyright (c) 2016年 ZL Meng. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface SimpleComCfgWinDelegate : NSObject{
    
@private
    IBOutlet NSWindow * winConfig;
    IBOutlet NSWindow * winSimpleCom;
    
    //NSMutableArray * m_arrCmdHistory;
}

-(IBAction)btOK:(id)sender;
-(IBAction)btCancel:(id)sender;

-(int)InitialCtls:(NSMutableDictionary *)dicConfiguration;
-(int)SaveConfig;

@end

