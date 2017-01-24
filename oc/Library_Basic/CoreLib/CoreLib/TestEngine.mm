//
//  TestEngine.m
//  CoreLib
//
//  Created by Ryan on 12-11-5.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import "TestEngine.h"
#include "luaDebugger.h"

@implementation TestEngine

//@synthesize dicConfiguration;
-(id)init
{
    self = [super init];
    if (self)
    {
        m_arrScripts = [NSMutableArray new];
        dicConfiguration = [NSMutableDictionary new];
        m_Debugger = [[luaDebugger alloc] initWithEngine:self];
    }
    return  self;
}

-(void)dealloc
{
    [m_arrScripts release];
    [dicConfiguration release];
    [m_Debugger release];
    [super dealloc];
}

-(NSString *)description
{
    //return [self.dicConfiguration description];
    return [CTestContext::m_dicConfiguration description];
}

-(int) RegisterInitTestViews:(NSView *)subview HiddenTestItem:(BOOL)bItem Origin:(int)iorigin
{
    @throw [NSException exceptionWithName:@"Overrid error" reason:@"you should overrid RegisterTestViews Test function first!" userInfo:nil];
    return 0;
}

-(int) StartTest;			//Do start test;
{
    @throw [NSException exceptionWithName:@"Overrid error" reason:@"you should overrid Start Test function first!" userInfo:nil];
    return 0;
}
-(int) StopTest;             //Do stop test;
{
    @throw [NSException exceptionWithName:@"Overrid error" reason:@"you should overrid Stop Test function first!" userInfo:nil];
    return 0;
}

-(int) PauseTest;			//Do pause test;
{
    @throw [NSException exceptionWithName:@"Overrid error" reason:@"you should overrid Pause Test function first!" userInfo:nil];
    return 0;
}
-(int) ResumeTest;			//Do Continue test;
{
    @throw [NSException exceptionWithName:@"Overrid error" reason:@"you should overrid Resume Test function first!" userInfo:nil];
    return 0;
}
-(int)WorkingMdoules
{
    @throw [NSException exceptionWithName:@"Overrid error" reason:@"you should overrid  WorkingMdoules function first!" userInfo:nil];
    return 0;
}

-(int)Cores
{
    @throw [NSException exceptionWithName:@"Overrid error" reason:@"you should overrid  Cores function first!" userInfo:nil];
    return 0;
}
-(int) IsTesting:(int)index;    //-1,all,0,1,2,3 each unit;
{
    @throw [NSException exceptionWithName:@"Overrid error" reason:@"you should overrid IsTesting function first!" userInfo:nil];
    return 0;
}
-(int)RegisterModule:(id)module
{
    @throw [NSException exceptionWithName:@"Overrid error" reason:@"you should overrid RegisterModule function first!" userInfo:nil];
    return 0;
}

-(int)RegisterScript:(NSString *)pathscript
{
    [m_arrScripts addObject:pathscript];
    return 0;
}

-(int)RegisterString:(const char *)string
{
    return 0;
}
-(int)ReloadProfile:(id)sender
{
    return 0;
}

-(void *)GetScripEngine:(int)index
{
    return NULL;
}

-(void *)GetTestContext:(int)index
{
    return NULL;
}

-(id)GetDebugger
{
    return m_Debugger;
}
@end
