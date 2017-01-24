//
//  TestEngine.h
//  CoreLib
//
//  Created by Ryan on 12-11-5.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once
#import <Foundation/Foundation.h>

#include "ScriptEngine.h"
#include "TestContext.h"

#include "Debugger.h"

#define kEngineUUTID   "UUT_ID"

#define kEngineUUT0Enable   "UUT0_Enable"
#define kEngineUUT1Enable   "UUT1_Enable"
#define kEngineUUT2Enable   "UUT2_Enable"
#define kEngineUUT3Enable   "UUT3_Enable"
#define kEngineUUT4Enable   "UUT4_Enable"
#define kEngineUUT5Enable   "UUT5_Enable"
#define kEngineUUT6Enable   "UUT6_Enable"
#define kEngineUUT7Enable   "UUT7_Enable"
#define kEngineUUT8Enable   "UUT8_Enable"

#define kEngineTestStatus   "EngineStatus"

#define kEngineProfilePath      "Engine_Profile"

#define kNotificationReloadProfile  "Engine_ReloadProfile"  

enum __ENGINE_STATUS {
    NO_NEED_WAIT=-1,
    IDLE = 0,
    RUNNING = 1,
    ABORT=2,
    };



@interface TestEngine : NSObject{
@private
    NSMutableDictionary * dicConfiguration;
    NSMutableArray * m_arrScripts;
    id<IDebugger> m_Debugger;
}
-(int) StartTest;			//Do start test;
-(int) StopTest;             //Do stop test;
-(int) PauseTest;			//Do pause test;
-(int) ResumeTest;			//Do Continue test;
-(int) IsTesting:(int)index;    //-1,all,0,1,2,3 each unit;
-(int) WorkingMdoules;          //Return engine working modules now.
-(int) Cores;                   //Return engine cores
-(int) RegisterModule:(id)module;    //Register Driver Module

-(int) RegisterInitTestViews:(NSView *)subview HiddenTestItem:(BOOL)bItem Origin:(int)iorigin;

-(int) RegisterScript:(NSString *)pathscript;
-(int) RegisterString:(const char *)string;
-(int) ReloadProfile:(id)sender;
-(void *) GetScripEngine:(int)index;
-(void *) GetTestContext:(int)index;
-(id<IDebugger>)GetDebugger;

//@property (retain) NSMutableDictionary * dicConfiguration;
@end
