/*
** Lua binding: CIANetFixture_Object
** Generated automatically by tolua++-1.0.92 on Wed Aug 24 11:04:59 2016.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_CIANetFixture_Object_open (lua_State* tolua_S);

#include "IANetFixture_global.h"

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"CIANetFixture");
}

/* get function: IANetFixture */
#ifndef TOLUA_DISABLE_tolua_get_IANETFIXTURE_ptr
static int tolua_get_IANETFIXTURE_ptr(lua_State* tolua_S)
{
   tolua_pushusertype(tolua_S,(void*)IANetFixture,"CIANetFixture");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: IANetFixture */
#ifndef TOLUA_DISABLE_tolua_set_IANETFIXTURE_ptr
static int tolua_set_IANETFIXTURE_ptr(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isusertype(tolua_S,2,"CIANetFixture",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  IANetFixture = ((CIANetFixture*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_CIANetFixture_Object_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,1);
 tolua_beginmodule(tolua_S,NULL);
  tolua_variable(tolua_S,"IANETFIXTURE",tolua_get_IANETFIXTURE_ptr,tolua_set_IANETFIXTURE_ptr);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_CIANetFixture_Object (lua_State* tolua_S) {
 return tolua_CIANetFixture_Object_open(tolua_S);
};
#endif

