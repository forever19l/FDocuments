//
//  luaDebugger.m
//  CoreLib
//
//  Created by intelligent on 13-7-17.
//
//

#import "luaDebugger.h"
#include "ScriptEngine.h"

@implementation luaDebugger

-(id)initWithEngine:(TestEngine *)Engine
{
    self = [super init];
    if (self)
    {
        m_pTestEngine = [Engine retain];
    }
    return self;
}

-(void)dealloc
{
    [m_pTestEngine release];
    [super dealloc];
}

-(void)MsgBox:(NSString *)msg
{
    NSRunAlertPanel(@"Debugger Error", @"%@", @"OK", nil, nil, msg);
}
    
-(NSDictionary *)DoStep:(lua_State *)L withItem:(NSString *)item
{
    @try {
        lua_getglobal(L, "__DoStep");
        if (lua_type(L, -1)!=LUA_TFUNCTION)
        {
            @throw [NSException exceptionWithName:@"Debugger Error" reason:@"Call __DoStep failed!" userInfo:nil];
        }
        
        item = [NSString stringWithFormat:@"return %@",item];
        int err = luaL_dostring(L, [item UTF8String]);
        if (err)
        {
            @throw [NSException exceptionWithName:@"Debugger Error" reason:[NSString stringWithUTF8String:lua_tostring(L, -1)] userInfo:nil];
        }
        
        err = lua_pcall(L, 1, 3, 0);
        if (err)
        {
            @throw [NSException exceptionWithName:@"Debugger Error" reason:[NSString stringWithUTF8String:lua_tostring(L, -1)] userInfo:nil];
        }
        
        //-3 : value
        //-2 : state
        //-1 : display
        
        const char * pvalue = lua_tostring(L, -3);
        lua_Integer pstate = lua_tointeger(L, -2);
        const char * pdisplay = lua_tostring(L, -1);
        if (!pdisplay)
        {
            pdisplay="";
        }
        
        NSDictionary * dic = [NSDictionary dictionaryWithObjectsAndKeys:[NSString stringWithUTF8String:pvalue], kDebuggerValue, [NSString stringWithUTF8String:pdisplay], kDebuggerPrompt, [NSNumber numberWithInteger:NSInteger(pstate)], kDebuggerState, nil];
        return dic;
    }
    @catch (NSException *exception) {
        NSDictionary * dic = [NSDictionary dictionaryWithObjectsAndKeys:[NSString stringWithUTF8String:"Error"],kDebuggerValue,[NSString stringWithUTF8String:"Error"],kDebuggerPrompt,[NSNumber numberWithInt:-1],kDebuggerState, nil];
        
        [self performSelectorOnMainThread:@selector(MsgBox:) withObject:[exception reason] waitUntilDone:YES];
        return dic;
    }
    @finally {
    }
}

-(id)StepOver:(id)sender
{
    @try {
        CScriptEngine * pScriptEngine = (CScriptEngine *)[[sender valueForKey:kDebuggerSE] longValue];
        NSString * strItem = [sender valueForKey:kDebuggerItem];
        return [self DoStep:pScriptEngine->m_pLuaState withItem:strItem];
    }
    @catch (NSException *exception) {
    }
    @finally {
    }
    
}
-(id)StepInto:(id)sender
{
    @try {
#if 1
        NSString * strItem = [sender valueForKey:kDebuggerItem];
        int ID = [[sender valueForKey:kDebuggerSlot] intValue];
        CScriptEngine * pScriptEngine = (CScriptEngine *)[m_pTestEngine GetScripEngine:ID];
        return [self DoStep:pScriptEngine->m_pLuaState withItem:strItem];
#else
        NSString * strItem = [sender valueForKey:kDebuggerItem];
        NSString * key[]={kDebuggerEnable0,kDebuggerEnable1,kDebuggerEnable2,kDebuggerEnable3};
        NSOperationQueue * queue = [[NSOperationQueue alloc] init];
        luaOperation * operation[4];
        for (int i=0;i<=3;i++)
        {
            CScriptEngine * pse = (CScriptEngine *)[m_pTestEngine GetScripEngine:i];
            operation[i] = [[[luaOperation alloc]initWithLua:pse->m_pLuaState] autorelease];
            operation[i].strTestItem = strItem;
            if ([[sender valueForKey:key[i]] boolValue])
            {
                [queue addOperation:operation[i]];
            }
        }
        while (1) {
            BOOL bFinished=YES;
            for (NSOperation * op in [queue operations])
            {
                //if (![op isFinished])
                if ([op isExecuting])
                {
                    bFinished=NO;
                    break;
                }
            }
            if (bFinished)
            {
                break;
            }
            //[NSThread sleepForTimeInterval:0.1];
            [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate date]];
        }
        //NSDictionary * dic = [NSDictionary dictionaryWithObjectsAndKeys:Operation0.dicResult,kDebuggerResult0, nil];
        NSMutableDictionary * dic = [NSMutableDictionary dictionary];
        NSString * str[]={kDebuggerResult0,kDebuggerResult1,kDebuggerResult2,kDebuggerResult3};
        for (int i=0; i<=3; i++) {
            [dic setValue:operation[i].dicResult forKey:str[i]];
        }
        [queue release];
        return dic;
#endif
    }
    @catch (NSException *exception) {
    }
    @finally {   
    }
}
-(void)AddBreakPoint:(id)sender
{
}
-(void)RemoveBreakPoint:(id)sender
{
}
-(id)GetGlobalVariable
{
    return nil;
}
-(id)GetLocalVariable
{
    return nil;
}
@end
