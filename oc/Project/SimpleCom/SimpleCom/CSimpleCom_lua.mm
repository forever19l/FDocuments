/*
** Lua binding: CSimpleCom_lua
** Generated automatically by tolua++-1.0.92 on Tue Nov  1 16:55:46 2016.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_CSimpleCom_lua_open (lua_State* tolua_S);

#include "SimpleCom.h"

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"CSimpleCom");
}

/* method: WriteString of class  CSimpleCom */
#ifndef TOLUA_DISABLE_tolua_CSimpleCom_lua_CSimpleCom_WriteString00
static int tolua_CSimpleCom_lua_CSimpleCom_WriteString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CSimpleCom",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CSimpleCom* self = (CSimpleCom*)  tolua_tousertype(tolua_S,1,0);
  const char* szDeviceName = ((const char*)  tolua_tostring(tolua_S,2,0));
  const char* szCMD = ((const char*)  tolua_tostring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'WriteString'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->WriteString(szDeviceName,szCMD);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'WriteString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddLabelString of class  CSimpleCom */
#ifndef TOLUA_DISABLE_tolua_CSimpleCom_lua_CSimpleCom_AddLabelString00
static int tolua_CSimpleCom_lua_CSimpleCom_AddLabelString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CSimpleCom",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CSimpleCom* self = (CSimpleCom*)  tolua_tousertype(tolua_S,1,0);
  const char* szDeviceName = ((const char*)  tolua_tostring(tolua_S,2,0));
  const char* LabelStr = ((const char*)  tolua_tostring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddLabelString'", NULL);
#endif
  {
   self->AddLabelString(szDeviceName,LabelStr);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddLabelString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReadString of class  CSimpleCom */
#ifndef TOLUA_DISABLE_tolua_CSimpleCom_lua_CSimpleCom_ReadString00
static int tolua_CSimpleCom_lua_CSimpleCom_ReadString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CSimpleCom",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CSimpleCom* self = (CSimpleCom*)  tolua_tousertype(tolua_S,1,0);
  const char* szDeviceName = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadString'", NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->ReadString(szDeviceName);
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReadStringRegex of class  CSimpleCom */
#ifndef TOLUA_DISABLE_tolua_CSimpleCom_lua_CSimpleCom_ReadStringRegex00
static int tolua_CSimpleCom_lua_CSimpleCom_ReadStringRegex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CSimpleCom",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CSimpleCom* self = (CSimpleCom*)  tolua_tousertype(tolua_S,1,0);
  const char* szDeviceName = ((const char*)  tolua_tostring(tolua_S,2,0));
  const char* Regex = ((const char*)  tolua_tostring(tolua_S,3,0));
  int timeout = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadStringRegex'", NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->ReadStringRegex(szDeviceName,Regex,timeout);
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadStringRegex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetDetectString of class  CSimpleCom */
#ifndef TOLUA_DISABLE_tolua_CSimpleCom_lua_CSimpleCom_SetDetectString00
static int tolua_CSimpleCom_lua_CSimpleCom_SetDetectString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CSimpleCom",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CSimpleCom* self = (CSimpleCom*)  tolua_tousertype(tolua_S,1,0);
  const char* szDeviceName = ((const char*)  tolua_tostring(tolua_S,2,0));
  const char* regex = ((const char*)  tolua_tostring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetDetectString'", NULL);
#endif
  {
   int tolua_ret = (int)  self->SetDetectString(szDeviceName,regex);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetDetectString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: WaitForString of class  CSimpleCom */
#ifndef TOLUA_DISABLE_tolua_CSimpleCom_lua_CSimpleCom_WaitForString00
static int tolua_CSimpleCom_lua_CSimpleCom_WaitForString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CSimpleCom",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CSimpleCom* self = (CSimpleCom*)  tolua_tousertype(tolua_S,1,0);
  const char* szDeviceName = ((const char*)  tolua_tostring(tolua_S,2,0));
  int iTimeout = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'WaitForString'", NULL);
#endif
  {
   int tolua_ret = (int)  self->WaitForString(szDeviceName,iTimeout);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'WaitForString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ClearBuffer of class  CSimpleCom */
#ifndef TOLUA_DISABLE_tolua_CSimpleCom_lua_CSimpleCom_ClearBuffer00
static int tolua_CSimpleCom_lua_CSimpleCom_ClearBuffer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CSimpleCom",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CSimpleCom* self = (CSimpleCom*)  tolua_tousertype(tolua_S,1,0);
  char* szDeviceName = ((char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ClearBuffer'", NULL);
#endif
  {
   self->ClearBuffer(szDeviceName);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ClearBuffer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_CSimpleCom_lua_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_cclass(tolua_S,"CSimpleCom","CSimpleCom","",NULL);
  tolua_beginmodule(tolua_S,"CSimpleCom");
   tolua_function(tolua_S,"WriteString",tolua_CSimpleCom_lua_CSimpleCom_WriteString00);
   tolua_function(tolua_S,"AddLabelString",tolua_CSimpleCom_lua_CSimpleCom_AddLabelString00);
   tolua_function(tolua_S,"ReadString",tolua_CSimpleCom_lua_CSimpleCom_ReadString00);
   tolua_function(tolua_S,"ReadStringRegex",tolua_CSimpleCom_lua_CSimpleCom_ReadStringRegex00);
   tolua_function(tolua_S,"SetDetectString",tolua_CSimpleCom_lua_CSimpleCom_SetDetectString00);
   tolua_function(tolua_S,"WaitForString",tolua_CSimpleCom_lua_CSimpleCom_WaitForString00);
   tolua_function(tolua_S,"ClearBuffer",tolua_CSimpleCom_lua_CSimpleCom_ClearBuffer00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_CSimpleCom_lua (lua_State* tolua_S) {
 return tolua_CSimpleCom_lua_open(tolua_S);
};
#endif

