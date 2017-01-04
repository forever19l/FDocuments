
#include "interface.h"

int tolua_global_open(lua_State* tolua_S);

int __cdecl luaopen_CameraTest2(lua_State * L)
{
	printf("< Load lib2Lua > ...\r\n");
	tolua_global_open(L);
	printf("< Load lib2Lua > Finished\r\n");
	return 0;
}
