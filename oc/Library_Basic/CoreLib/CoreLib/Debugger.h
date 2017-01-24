//
//  IDebugger.h
//  CoreLib
//
//  Created by intelligent on 13-7-17.
//
//

#import <Foundation/Foundation.h>

#pragma once


#define kDebuggerInfor          @"Debugger_Infor"
#define kDebuggerUUt            @"ID"
#define kDebuggerValue          @"value"
#define kDebuggerPrompt         @"prompt"
#define kDebuggerState          @"state"

#define kDebuggerSE             @"ScriptEngine"
#define kDebuggerSlot           @"UnitIndex"
#define kDebuggerItem           @"Item"

#define kDebuggerResult0        @"result0"
#define kDebuggerResult1        @"result1"
#define kDebuggerResult2        @"result2"
#define kDebuggerResult3        @"result3"
#define kDebuggerResult4        @"result4"
#define kDebuggerResult5        @"result5"
#define kDebuggerResult6        @"result6"
#define kDebuggerResult7        @"result7"
#define kDebuggerResult8        @"result8"

#define kDebuggerEnable0        @"enable_uut0"
#define kDebuggerEnable1        @"enable_uut1"
#define kDebuggerEnable2        @"enable_uut2"
#define kDebuggerEnable3        @"enable_uut3"
#define kDebuggerEnable4        @"enable_uut4"
#define kDebuggerEnable5        @"enable_uut5"
#define kDebuggerEnable6        @"enable_uut6"
#define kDebuggerEnable7        @"enable_uut7"
#define kDebuggerEnable8        @"enable_uut8"

@protocol IDebugger <NSObject>
@required
-(id)StepOver:(id)sender;
-(id)StepInto:(id)sender;
-(void)AddBreakPoint:(id)sender;
-(void)RemoveBreakPoint:(id)sender;
-(id)GetGlobalVariable;
-(id)GetLocalVariable;
@optional
@end
