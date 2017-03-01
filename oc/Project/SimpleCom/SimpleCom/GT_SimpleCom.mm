//
//  GT_SimpleCom.cpp
//  SimpleCom
//
//  Created by Liu Liang on 13-12-23.
//  Copyright (c) 2013年 Liu Liang. All rights reserved.
//

#include "GT_SimpleCom.h"
#include "tolua++.h"
#import <CoreLib/ScriptEngine.h>
#import <CoreLib/tag.h>

TOLUA_API int  tolua_CSimpleCom_lua_open (lua_State* tolua_S);
TOLUA_API int  tolua_CSimpleCom_Object_open (lua_State* tolua_S);

@implementation GT_SimpleCom
-(id)init
{
    self = [super init] ;
    
    return self ;
}

- (void)dealloc
{
    [super dealloc];
}


#pragma mark-DriverModule interface
-(NSString *)ModuleName           //模块名称说明
{
    return @"GT SimpleCom" ;
}

-(int)RegisterModule:(id)sender
{
    
    int err;
    NSMutableDictionary * dic = (NSMutableDictionary *)sender;
    lua_State * lua = (lua_State *)[[dic objectForKey:@"lua"] longValue];
    //Register FCT class
    tolua_CSimpleCom_lua_open(lua);
    tolua_CSimpleCom_Object_open(lua);
    
    //Register Script
    NSBundle * bundle = [NSBundle bundleForClass:[self class]];
    //    NSLog(@"......%@",[bundle resourcePath]) ;
    NSString * str = [NSString stringWithFormat:@"package.path = package.path..';'..'%@'..'/?.lua'",[bundle resourcePath]];
    pTestEngine = [dic objectForKey:@"TestEngine"];
    //
    //    [pTestEngine RegisterString:[str UTF8String]];
    //    [pTestEngine RegisterString:"IAnetfixture = require \"uart\""];
    int fixId = [[dic objectForKey:@"id"] intValue];
    
    CScriptEngine * se = (CScriptEngine *)[pTestEngine GetScripEngine:fixId];
    se->DoString([str UTF8String]);
    err = se->DoString("simplecom = require \"simplecom\"");
    
    if (err)
    {
        NSRunAlertPanel(@"Load Error", @"%s", @"OK", nil, nil,lua_tostring(se->m_pLuaState, -1));
    }
    
    if (fixId==0)
    {
        NSString * strscript = [[NSBundle bundleForClass:[self class]] pathForResource:@"SimpleCom" ofType:@"lua"];
        ListLuaFile([strscript UTF8String],"simplecom");
    }
    return 0;
}


-(int)SelfTest:(id)sender          //模块自我测试
{
    return 0 ;
}

-(int)Load:(id)sender
{
    if ([NSBundle loadNibNamed:@"SimpleComDebugPanel" owner:self]==FALSE)
        return -1 ;
    
    return 0 ;
}

-(int)Unload:(id)sender
{
    return 0 ;
}

@end