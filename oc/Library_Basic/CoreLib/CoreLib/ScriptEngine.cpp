//
//  ScriptEngine.cpp
//  iTMP
//
//  Created by Ryan on 12-5-8.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#include "ScriptEngine.h"
#include "tolua++.h"


#pragma comment(lib,"libtolua++.a")
CScriptEngine::CScriptEngine(void)
{
	//Lua Initial
    m_pLuaState =luaL_newstate();
	luaL_openlibs(m_pLuaState);
	//open(m_pLuaState);
	//RegisterAll();
    tolua_open(m_pLuaState);
}

CScriptEngine::~CScriptEngine(void)
{
    printf("CScriptEngine::~CScriptEngine(void)\r\n");
	lua_close(m_pLuaState);
}

int CScriptEngine::Init()
{
	RegisterAll();
	return 0;
}

int CScriptEngine::LoadFile(const char *szFile)
{
    return luaL_loadfile(m_pLuaState, szFile);
}

int CScriptEngine::DoFile(const char * szFile)
{
    
	return luaL_dofile(m_pLuaState,szFile);
}

int CScriptEngine::DoString(const char * szText)
{
	return luaL_dostring(m_pLuaState,szText);
}


int CScriptEngine::LoadFile(lua_State* luaState,const char *szFile)
{
    return luaL_loadfile(luaState, szFile);
}

int CScriptEngine::DoFile(lua_State* luaState,const char * szFile)
{
    
	return luaL_dofile(luaState,szFile);
}

int CScriptEngine::DoString(lua_State* luaState,const char * szText)
{
	return luaL_dostring(luaState,szText);
}



void CScriptEngine::RegisterAll()
{
	
}
lua_State * CScriptEngine::GetLuaState() const
{
    return m_pLuaState;
}

void CScriptEngine::Reslease()
{
}
