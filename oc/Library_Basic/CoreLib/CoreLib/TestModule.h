//
//  TestModule.h
//  CoreLib
//
//  Created by Ryan on 12-11-15.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#include "Common.h"
@protocol TestModule <NSObject>
@required
-(NSString *)ModuleName;           //模块名称说明
-(NSString *)Version;
-(NSImage *)Image;
-(int)Load;
-(int)UnLoad;
@optional

@end
