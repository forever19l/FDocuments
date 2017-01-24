//
//  iTestEngine.m
//  CoreLib
//
//  Created by Ryan on 12-6-27.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import "iTestEngine.h"

@implementation iTestEngine
- (id)init
{
    self = [super init];
    if (self) {
        // Initialization code here.
        dicConfiguration = [[NSMutableDictionary alloc]init];
    }
    
    return self;
}

- (void)dealloc
{
    [super dealloc];
    [dicConfiguration release];
}
-(int)StartTest:(id)sender
{
    return 0;
}
-(int)StopTest:(id)sender
{
    return 0;
}
-(int)PauseTest:(id)sender
{
    return 0;
}
-(int)ResumeTest:(id)sender
{
    return 0;
}
@end
