//
//  AppDelegate.h
//  DUT
//
//  Created by Ryan on 11/2/15.
//  Copyright © 2015 ___Intelligent Automation___. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <MMTabBarView/MMTabBarView.h>


#define kProjectName        @"project_name"
#define kSoltsNumber         @"slots_number"

#define kSubscribe        @"Subscribe"

@interface AppDelegate : NSObject <NSApplicationDelegate>{
    IBOutlet MMTabBarView           *tabdbgBar;
    IBOutlet NSTabView              *tabdbgView;
    NSMutableArray * arrUnit;
    
    IBOutlet NSWindow * windbgConfiguration;
    IBOutlet NSWindow    * windbgMain;
    
    int m_Slots;
    
    NSMutableDictionary * m_dicDefine;
    NSMutableDictionary * m_dicConfiguration;
}

@end
