//
//  luaDebugger.h
//  CoreLib
//
//  Created by intelligent on 13-7-17.
//
//

#import <Cocoa/Cocoa.h>
#include "Debugger.h"
#include "TestEngine.h"

#include "luaOperation.h"

#pragma once

@interface luaDebugger : NSObject<IDebugger>{
    TestEngine * m_pTestEngine;
}
-(id)initWithEngine:(TestEngine *)Engine;

@end
