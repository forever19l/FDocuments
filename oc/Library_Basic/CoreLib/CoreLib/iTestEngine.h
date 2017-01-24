//
//  iTestEngine.h
//  CoreLib
//
//  Created by Ryan on 12-6-27.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface iTestEngine : NSObject{
@private
    NSMutableDictionary * dicConfiguration;
}
-(int) StartTest:(id)sender;			//Do start test;
-(int) StopTest:(id)sender;             //Do stop test;
-(int) PauseTest:(id)sender;			//Do pause test;
-(int) ResumeTest:(id)sender;			//Do Continue test;
@end
