//
//  luaOperation.m
//  CoreLib
//
//  Created by intelligent on 13-7-18.
//
//

#import "luaOperation.h"
#include "Debugger.h"

@implementation luaOperation
@synthesize dicResult,strTestItem;
-(id)initWithLua:(lua_State *)L
{
    self = [super init];
    if (self)
    {
        m_pState = L;
        dicResult = [NSMutableDictionary new];
        self.strTestItem=@"";
    }
    return self;
}

-(void)dealloc
{
    [dicResult release];
    [super dealloc];
}

-(BOOL)isConcurrent
{
    return YES;
}

-(void)MsgBox:(NSString *)msg
{
    NSRunAlertPanel(@"Debugger Error", @"%@", @"OK", nil, nil, msg);
}

-(void)main
{
    @try {
        [dicResult removeAllObjects];
        lua_getglobal(m_pState, "__DoStep");
        if (lua_type(m_pState, -1)!=LUA_TFUNCTION)
        {
            @throw [NSException exceptionWithName:@"Debugger Error" reason:@"Call __DoStep failed!" userInfo:nil];
        }
        
        NSString * item = [NSString stringWithFormat:@"return %@",self.strTestItem];
        int err = luaL_dostring(m_pState, [item UTF8String]);
        if (err)
        {
            @throw [NSException exceptionWithName:@"Debugger Error" reason:[NSString stringWithUTF8String:lua_tostring(m_pState, -1)] userInfo:nil];
        }
        
        err = lua_pcall(m_pState, 1, 3, 0);
        if (err)
        {
            @throw [NSException exceptionWithName:@"Debugger Error" reason:[NSString stringWithUTF8String:lua_tostring(m_pState, -1)] userInfo:nil];
        }
        
        //-3 : value
        //-2 : state
        //-1 : display
        
        const char * pvalue = lua_tostring(m_pState, -3);
        lua_Integer pstate = lua_tointeger(m_pState, -2);
        const char * pdisplay = lua_tostring(m_pState, -1);
        if (!pdisplay)
        {
            pdisplay="";
        }
        printf("value: %s,state:%ld,display:%s\r\n", pvalue, pstate, pdisplay);
        [dicResult setValue:[NSString stringWithUTF8String:pvalue] forKey:kDebuggerValue];
        [dicResult setValue:[NSString stringWithUTF8String:pdisplay] forKey:kDebuggerPrompt];
        [dicResult setValue:[NSNumber numberWithInteger:NSInteger(pstate)] forKey:kDebuggerState];
    }
    @catch (NSException *exception) {
        [dicResult removeAllObjects];
        [self performSelectorOnMainThread:@selector(MsgBox:) withObject:[exception reason] waitUntilDone:YES];
    }
    @finally {
    }

}
@end
