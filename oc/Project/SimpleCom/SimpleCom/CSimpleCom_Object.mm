/*
** Lua binding: CSimpleCom_Object
** Generated automatically by tolua++-1.0.92 on Tue Nov  1 16:55:46 2016.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_CSimpleCom_Object_open (lua_State* tolua_S);

#include "SimpleCom_Global.h"

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"CSimpleCom");
}

/* get function: SimpleCom */
#ifndef TOLUA_DISABLE_tolua_get_SIMPLECOM_ptr
static int tolua_get_SIMPLECOM_ptr(lua_State* tolua_S)
{
   tolua_pushusertype(tolua_S,(void*)SimpleCom,"CSimpleCom");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: SimpleCom */
#ifndef TOLUA_DISABLE_tolua_set_SIMPLECOM_ptr
static int tolua_set_SIMPLECOM_ptr(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!tolua_isusertype(tolua_S,2,"CSimpleCom",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  SimpleCom = ((CSimpleCom*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_CSimpleCom_Object_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,1);
 tolua_beginmodule(tolua_S,NULL);
  tolua_variable(tolua_S,"SIMPLECOM",tolua_get_SIMPLECOM_ptr,tolua_set_SIMPLECOM_ptr);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_CSimpleCom_Object (lua_State* tolua_S) {
 return tolua_CSimpleCom_Object_open(tolua_S);
};
#endif

