//
//  ScriptEngine.h
//  iTMP
//
//  Created by Ryan on 12-5-8.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//
#pragma once

#include <lua.hpp>
#include <string>
#include <iterator>
#include <iostream>
#include <algorithm>


class CScriptEngine {
public:
    CScriptEngine();
    virtual ~CScriptEngine();
    
public:
    virtual int Init();
	int LoadString(char * szText);
	int LoadFile(const char * szFile);
	int DoString(const char * szText);
    int DoFile(const char * szFile);
    
//    int pcall(lua_State* luastate,const char* sztable,const char* szfun,array* paramter,array *output);
    int LoadString(lua_State* luaState,char * szText);
	int LoadFile(lua_State* luaState,const char * szFile);
    int DoString(lua_State* luaState,const char * szText);
    int DoFile(lua_State* luaState,const char * szFile);

	void Reslease();
    lua_State * GetLuaState() const;
protected:
    virtual void RegisterAll();
public:
	lua_State * m_pLuaState;
};