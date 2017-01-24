/*
** Lua binding: CIANetFixture_lua
** Generated automatically by tolua++-1.0.92 on Wed Aug 24 11:04:59 2016.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_CIANetFixture_lua_open (lua_State* tolua_S);

#include "CIANetFixture.h"

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"ushort");
 tolua_usertype(tolua_S,"CIANetFixture");
}

/* method: Read of class  CIANetFixture */
#ifndef TOLUA_DISABLE_tolua_CIANetFixture_lua_CIANetFixture_Read00
static int tolua_CIANetFixture_lua_CIANetFixture_Read00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CIANetFixture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CIANetFixture* self = (CIANetFixture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Read'", NULL);
#endif
  {
   self->Read();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Read'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Write of class  CIANetFixture */
#ifndef TOLUA_DISABLE_tolua_CIANetFixture_lua_CIANetFixture_Write00
static int tolua_CIANetFixture_lua_CIANetFixture_Write00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CIANetFixture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CIANetFixture* self = (CIANetFixture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Write'", NULL);
#endif
  {
   self->Write();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Write'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CancelTest of class  CIANetFixture */
#ifndef TOLUA_DISABLE_tolua_CIANetFixture_lua_CIANetFixture_CancelTest00
static int tolua_CIANetFixture_lua_CIANetFixture_CancelTest00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CIANetFixture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CIANetFixture* self = (CIANetFixture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CancelTest'", NULL);
#endif
  {
   self->CancelTest();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CancelTest'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: StartTest of class  CIANetFixture */
#ifndef TOLUA_DISABLE_tolua_CIANetFixture_lua_CIANetFixture_StartTest00
static int tolua_CIANetFixture_lua_CIANetFixture_StartTest00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CIANetFixture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CIANetFixture* self = (CIANetFixture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'StartTest'", NULL);
#endif
  {
   self->StartTest();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'StartTest'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTestPlatNum of class  CIANetFixture */
#ifndef TOLUA_DISABLE_tolua_CIANetFixture_lua_CIANetFixture_getTestPlatNum00
static int tolua_CIANetFixture_lua_CIANetFixture_getTestPlatNum00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CIANetFixture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CIANetFixture* self = (CIANetFixture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTestPlatNum'", NULL);
#endif
  {
   int tolua_ret = (int)  self->getTestPlatNum();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTestPlatNum'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTestDutNum of class  CIANetFixture */
#ifndef TOLUA_DISABLE_tolua_CIANetFixture_lua_CIANetFixture_getTestDutNum00
static int tolua_CIANetFixture_lua_CIANetFixture_getTestDutNum00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CIANetFixture",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CIANetFixture* self = (CIANetFixture*)  tolua_tousertype(tolua_S,1,0);
  int plat = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTestDutNum'", NULL);
#endif
  {
   int tolua_ret = (int)  self->getTestDutNum(plat);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTestDutNum'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GrabOnce of class  CIANetFixture */
#ifndef TOLUA_DISABLE_tolua_CIANetFixture_lua_CIANetFixture_GrabOnce00
static int tolua_CIANetFixture_lua_CIANetFixture_GrabOnce00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CIANetFixture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CIANetFixture* self = (CIANetFixture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GrabOnce'", NULL);
#endif
  {
   long tolua_ret = (long)  self->GrabOnce();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GrabOnce'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CalculateServoPoint of class  CIANetFixture */
#ifndef TOLUA_DISABLE_tolua_CIANetFixture_lua_CIANetFixture_CalculateServoPoint00
static int tolua_CIANetFixture_lua_CIANetFixture_CalculateServoPoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CIANetFixture",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CIANetFixture* self = (CIANetFixture*)  tolua_tousertype(tolua_S,1,0);
  int platnum = ((int)  tolua_tonumber(tolua_S,2,0));
  int dutnum = ((int)  tolua_tonumber(tolua_S,3,0));
  int modelname = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CalculateServoPoint'", NULL);
#endif
  {
   int tolua_ret = (int)  self->CalculateServoPoint(platnum,dutnum,modelname);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CalculateServoPoint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CalTestPointX of class  CIANetFixture */
#ifndef TOLUA_DISABLE_tolua_CIANetFixture_lua_CIANetFixture_CalTestPointX00
static int tolua_CIANetFixture_lua_CIANetFixture_CalTestPointX00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CIANetFixture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CIANetFixture* self = (CIANetFixture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CalTestPointX'", NULL);
#endif
  {
   float tolua_ret = (float)  self->CalTestPointX();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CalTestPointX'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CalTestPointY of class  CIANetFixture */
#ifndef TOLUA_DISABLE_tolua_CIANetFixture_lua_CIANetFixture_CalTestPointY00
static int tolua_CIANetFixture_lua_CIANetFixture_CalTestPointY00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CIANetFixture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CIANetFixture* self = (CIANetFixture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CalTestPointY'", NULL);
#endif
  {
   float tolua_ret = (float)  self->CalTestPointY();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CalTestPointY'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetDUTTypeFromCamera of class  CIANetFixture */
#ifndef TOLUA_DISABLE_tolua_CIANetFixture_lua_CIANetFixture_GetDUTTypeFromCamera00
static int tolua_CIANetFixture_lua_CIANetFixture_GetDUTTypeFromCamera00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CIANetFixture",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CIANetFixture* self = (CIANetFixture*)  tolua_tousertype(tolua_S,1,0);
  int PlatNum = ((int)  tolua_tonumber(tolua_S,2,0));
  int HandNum = ((int)  tolua_tonumber(tolua_S,3,0));
  int modelname = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetDUTTypeFromCamera'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetDUTTypeFromCamera(PlatNum,HandNum,modelname);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetDUTTypeFromCamera'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReadPosition of class  CIANetFixture */
#ifndef TOLUA_DISABLE_tolua_CIANetFixture_lua_CIANetFixture_ReadPosition00
static int tolua_CIANetFixture_lua_CIANetFixture_ReadPosition00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CIANetFixture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CIANetFixture* self = (CIANetFixture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadPosition'", NULL);
#endif
  {
   self->ReadPosition();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadPosition'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTestPoint of class  CIANetFixture */
#ifndef TOLUA_DISABLE_tolua_CIANetFixture_lua_CIANetFixture_getTestPoint00
static int tolua_CIANetFixture_lua_CIANetFixture_getTestPoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CIANetFixture",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CIANetFixture* self = (CIANetFixture*)  tolua_tousertype(tolua_S,1,0);
  int platnum = ((int)  tolua_tonumber(tolua_S,2,0));
  int dutnum = ((int)  tolua_tonumber(tolua_S,3,0));
  string key = ((string)  tolua_tocppstring(tolua_S,4,0));
  string xyz = ((string)  tolua_tocppstring(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTestPoint'", NULL);
#endif
  {
   double tolua_ret = (double)  self->getTestPoint(platnum,dutnum,key,xyz);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTestPoint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTestPointcal of class  CIANetFixture */
#ifndef TOLUA_DISABLE_tolua_CIANetFixture_lua_CIANetFixture_getTestPointcal00
static int tolua_CIANetFixture_lua_CIANetFixture_getTestPointcal00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CIANetFixture",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CIANetFixture* self = (CIANetFixture*)  tolua_tousertype(tolua_S,1,0);
  int platnum = ((int)  tolua_tonumber(tolua_S,2,0));
  int dutnum = ((int)  tolua_tonumber(tolua_S,3,0));
  string key = ((string)  tolua_tocppstring(tolua_S,4,0));
  string xyz = ((string)  tolua_tocppstring(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTestPointcal'", NULL);
#endif
  {
   double tolua_ret = (double)  self->getTestPointcal(platnum,dutnum,key,xyz);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTestPointcal'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRotatePoint of class  CIANetFixture */
#ifndef TOLUA_DISABLE_tolua_CIANetFixture_lua_CIANetFixture_getRotatePoint00
static int tolua_CIANetFixture_lua_CIANetFixture_getRotatePoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CIANetFixture",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CIANetFixture* self = (CIANetFixture*)  tolua_tousertype(tolua_S,1,0);
  int platnum = ((int)  tolua_tonumber(tolua_S,2,0));
  int dutnum = ((int)  tolua_tonumber(tolua_S,3,0));
  int key = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRotatePoint'", NULL);
#endif
  {
   double tolua_ret = (double)  self->getRotatePoint(platnum,dutnum,key);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRotatePoint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getCameraPoint of class  CIANetFixture */
#ifndef TOLUA_DISABLE_tolua_CIANetFixture_lua_CIANetFixture_getCameraPoint00
static int tolua_CIANetFixture_lua_CIANetFixture_getCameraPoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CIANetFixture",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CIANetFixture* self = (CIANetFixture*)  tolua_tousertype(tolua_S,1,0);
  int plantnum = ((int)  tolua_tonumber(tolua_S,2,0));
  int dutnum = ((int)  tolua_tonumber(tolua_S,3,0));
  string key = ((string)  tolua_tocppstring(tolua_S,4,0));
  string xyz = ((string)  tolua_tocppstring(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCameraPoint'", NULL);
#endif
  {
   double tolua_ret = (double)  self->getCameraPoint(plantnum,dutnum,key,xyz);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCameraPoint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ClearLoadData of class  CIANetFixture */
#ifndef TOLUA_DISABLE_tolua_CIANetFixture_lua_CIANetFixture_ClearLoadData00
static int tolua_CIANetFixture_lua_CIANetFixture_ClearLoadData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CIANetFixture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CIANetFixture* self = (CIANetFixture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ClearLoadData'", NULL);
#endif
  {
   self->ClearLoadData();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ClearLoadData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getLoadCellMarketData of class  CIANetFixture */
#ifndef TOLUA_DISABLE_tolua_CIANetFixture_lua_CIANetFixture_getLoadCellMarketData00
static int tolua_CIANetFixture_lua_CIANetFixture_getLoadCellMarketData00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CIANetFixture",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CIANetFixture* self = (CIANetFixture*)  tolua_tousertype(tolua_S,1,0);
  int Index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLoadCellMarketData'", NULL);
#endif
  {
   double tolua_ret = (double)  self->getLoadCellMarketData(Index);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLoadCellMarketData'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ShowJoystick of class  CIANetFixture */
#ifndef TOLUA_DISABLE_tolua_CIANetFixture_lua_CIANetFixture_ShowJoystick00
static int tolua_CIANetFixture_lua_CIANetFixture_ShowJoystick00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CIANetFixture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CIANetFixture* self = (CIANetFixture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ShowJoystick'", NULL);
#endif
  {
   int tolua_ret = (int)  self->ShowJoystick();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ShowJoystick'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SendCmdPLC of class  CIANetFixture */
#ifndef TOLUA_DISABLE_tolua_CIANetFixture_lua_CIANetFixture_SendCmdPLC00
static int tolua_CIANetFixture_lua_CIANetFixture_SendCmdPLC00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CIANetFixture",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"ushort",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"ushort",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"ushort",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"ushort",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,6,&tolua_err) || !tolua_isusertype(tolua_S,6,"ushort",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CIANetFixture* self = (CIANetFixture*)  tolua_tousertype(tolua_S,1,0);
  ushort cmd_count = *((ushort*)  tolua_tousertype(tolua_S,2,0));
  ushort cmd_type = *((ushort*)  tolua_tousertype(tolua_S,3,0));
  ushort cmd_id = *((ushort*)  tolua_tousertype(tolua_S,4,0));
  ushort cmd_v1 = *((ushort*)  tolua_tousertype(tolua_S,5,0));
  ushort cmd_v2 = *((ushort*)  tolua_tousertype(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SendCmdPLC'", NULL);
#endif
  {
   long tolua_ret = (long)  self->SendCmdPLC(cmd_count,cmd_type,cmd_id,cmd_v1,cmd_v2);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SendCmdPLC'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SendCMDJoystick of class  CIANetFixture */
#ifndef TOLUA_DISABLE_tolua_CIANetFixture_lua_CIANetFixture_SendCMDJoystick00
static int tolua_CIANetFixture_lua_CIANetFixture_SendCMDJoystick00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CIANetFixture",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"ushort",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"ushort",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CIANetFixture* self = (CIANetFixture*)  tolua_tousertype(tolua_S,1,0);
  int downORrotate = ((int)  tolua_tonumber(tolua_S,2,0));
  ushort d1 = *((ushort*)  tolua_tousertype(tolua_S,3,0));
  ushort d2 = *((ushort*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SendCMDJoystick'", NULL);
#endif
  {
   long tolua_ret = (long)  self->SendCMDJoystick(downORrotate,d1,d2);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SendCMDJoystick'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SendCmdLoadcell of class  CIANetFixture */
#ifndef TOLUA_DISABLE_tolua_CIANetFixture_lua_CIANetFixture_SendCmdLoadcell00
static int tolua_CIANetFixture_lua_CIANetFixture_SendCmdLoadcell00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CIANetFixture",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CIANetFixture* self = (CIANetFixture*)  tolua_tousertype(tolua_S,1,0);
  int KeyIndex = ((int)  tolua_tonumber(tolua_S,2,0));
  char* localCmd = ((char*)  tolua_tostring(tolua_S,3,0));
  int PlcCmdTimeout = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SendCmdLoadcell'", NULL);
#endif
  {
   long tolua_ret = (long)  self->SendCmdLoadcell(KeyIndex,localCmd,PlcCmdTimeout);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SendCmdLoadcell'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ZipRawdata of class  CIANetFixture */
#ifndef TOLUA_DISABLE_tolua_CIANetFixture_lua_CIANetFixture_ZipRawdata00
static int tolua_CIANetFixture_lua_CIANetFixture_ZipRawdata00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CIANetFixture",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CIANetFixture* self = (CIANetFixture*)  tolua_tousertype(tolua_S,1,0);
  const char* zippathfile = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ZipRawdata'", NULL);
#endif
  {
   int tolua_ret = (int)  self->ZipRawdata(zippathfile);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ZipRawdata'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ClearTmpDatafile of class  CIANetFixture */
#ifndef TOLUA_DISABLE_tolua_CIANetFixture_lua_CIANetFixture_ClearTmpDatafile00
static int tolua_CIANetFixture_lua_CIANetFixture_ClearTmpDatafile00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CIANetFixture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CIANetFixture* self = (CIANetFixture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ClearTmpDatafile'", NULL);
#endif
  {
   int tolua_ret = (int)  self->ClearTmpDatafile();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ClearTmpDatafile'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SF_Login of class  CIANetFixture */
#ifndef TOLUA_DISABLE_tolua_CIANetFixture_lua_CIANetFixture_SF_Login00
static int tolua_CIANetFixture_lua_CIANetFixture_SF_Login00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CIANetFixture",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CIANetFixture* self = (CIANetFixture*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SF_Login'", NULL);
#endif
  {
   int tolua_ret = (int)  self->SF_Login();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SF_Login'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SF_CheckRoute of class  CIANetFixture */
#ifndef TOLUA_DISABLE_tolua_CIANetFixture_lua_CIANetFixture_SF_CheckRoute00
static int tolua_CIANetFixture_lua_CIANetFixture_SF_CheckRoute00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CIANetFixture",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CIANetFixture* self = (CIANetFixture*)  tolua_tousertype(tolua_S,1,0);
  string web = ((string)  tolua_tocppstring(tolua_S,2,0));
  string sn = ((string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SF_CheckRoute'", NULL);
#endif
  {
   int tolua_ret = (int)  self->SF_CheckRoute(web,sn);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SF_CheckRoute'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SF_SaveJson of class  CIANetFixture */
#ifndef TOLUA_DISABLE_tolua_CIANetFixture_lua_CIANetFixture_SF_SaveJson00
static int tolua_CIANetFixture_lua_CIANetFixture_SF_SaveJson00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CIANetFixture",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CIANetFixture* self = (CIANetFixture*)  tolua_tousertype(tolua_S,1,0);
  string value = ((string)  tolua_tocppstring(tolua_S,2,0));
  string key = ((string)  tolua_tocppstring(tolua_S,3,0));
  string name = ((string)  tolua_tocppstring(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SF_SaveJson'", NULL);
#endif
  {
   int tolua_ret = (int)  self->SF_SaveJson(value,key,name);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SF_SaveJson'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SF_SendJson of class  CIANetFixture */
#ifndef TOLUA_DISABLE_tolua_CIANetFixture_lua_CIANetFixture_SF_SendJson00
static int tolua_CIANetFixture_lua_CIANetFixture_SF_SendJson00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CIANetFixture",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CIANetFixture* self = (CIANetFixture*)  tolua_tousertype(tolua_S,1,0);
  string web = ((string)  tolua_tocppstring(tolua_S,2,0));
  string sn = ((string)  tolua_tocppstring(tolua_S,3,0));
  string date = ((string)  tolua_tocppstring(tolua_S,4,0));
  int result = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SF_SendJson'", NULL);
#endif
  {
   int tolua_ret = (int)  self->SF_SendJson(web,sn,date,result);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SF_SendJson'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_CIANetFixture_lua_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_cclass(tolua_S,"CIANetFixture","CIANetFixture","",NULL);
  tolua_beginmodule(tolua_S,"CIANetFixture");
   tolua_function(tolua_S,"Read",tolua_CIANetFixture_lua_CIANetFixture_Read00);
   tolua_function(tolua_S,"Write",tolua_CIANetFixture_lua_CIANetFixture_Write00);
   tolua_function(tolua_S,"CancelTest",tolua_CIANetFixture_lua_CIANetFixture_CancelTest00);
   tolua_function(tolua_S,"StartTest",tolua_CIANetFixture_lua_CIANetFixture_StartTest00);
   tolua_function(tolua_S,"getTestPlatNum",tolua_CIANetFixture_lua_CIANetFixture_getTestPlatNum00);
   tolua_function(tolua_S,"getTestDutNum",tolua_CIANetFixture_lua_CIANetFixture_getTestDutNum00);
   tolua_function(tolua_S,"GrabOnce",tolua_CIANetFixture_lua_CIANetFixture_GrabOnce00);
   tolua_function(tolua_S,"CalculateServoPoint",tolua_CIANetFixture_lua_CIANetFixture_CalculateServoPoint00);
   tolua_function(tolua_S,"CalTestPointX",tolua_CIANetFixture_lua_CIANetFixture_CalTestPointX00);
   tolua_function(tolua_S,"CalTestPointY",tolua_CIANetFixture_lua_CIANetFixture_CalTestPointY00);
   tolua_function(tolua_S,"GetDUTTypeFromCamera",tolua_CIANetFixture_lua_CIANetFixture_GetDUTTypeFromCamera00);
   tolua_function(tolua_S,"ReadPosition",tolua_CIANetFixture_lua_CIANetFixture_ReadPosition00);
   tolua_function(tolua_S,"getTestPoint",tolua_CIANetFixture_lua_CIANetFixture_getTestPoint00);
   tolua_function(tolua_S,"getTestPointcal",tolua_CIANetFixture_lua_CIANetFixture_getTestPointcal00);
   tolua_function(tolua_S,"getRotatePoint",tolua_CIANetFixture_lua_CIANetFixture_getRotatePoint00);
   tolua_function(tolua_S,"getCameraPoint",tolua_CIANetFixture_lua_CIANetFixture_getCameraPoint00);
   tolua_function(tolua_S,"ClearLoadData",tolua_CIANetFixture_lua_CIANetFixture_ClearLoadData00);
   tolua_function(tolua_S,"getLoadCellMarketData",tolua_CIANetFixture_lua_CIANetFixture_getLoadCellMarketData00);
   tolua_function(tolua_S,"ShowJoystick",tolua_CIANetFixture_lua_CIANetFixture_ShowJoystick00);
   tolua_function(tolua_S,"SendCmdPLC",tolua_CIANetFixture_lua_CIANetFixture_SendCmdPLC00);
   tolua_function(tolua_S,"SendCMDJoystick",tolua_CIANetFixture_lua_CIANetFixture_SendCMDJoystick00);
   tolua_function(tolua_S,"SendCmdLoadcell",tolua_CIANetFixture_lua_CIANetFixture_SendCmdLoadcell00);
   tolua_function(tolua_S,"ZipRawdata",tolua_CIANetFixture_lua_CIANetFixture_ZipRawdata00);
   tolua_function(tolua_S,"ClearTmpDatafile",tolua_CIANetFixture_lua_CIANetFixture_ClearTmpDatafile00);
   tolua_function(tolua_S,"SF_Login",tolua_CIANetFixture_lua_CIANetFixture_SF_Login00);
   tolua_function(tolua_S,"SF_CheckRoute",tolua_CIANetFixture_lua_CIANetFixture_SF_CheckRoute00);
   tolua_function(tolua_S,"SF_SaveJson",tolua_CIANetFixture_lua_CIANetFixture_SF_SaveJson00);
   tolua_function(tolua_S,"SF_SendJson",tolua_CIANetFixture_lua_CIANetFixture_SF_SendJson00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_CIANetFixture_lua (lua_State* tolua_S) {
 return tolua_CIANetFixture_lua_open(tolua_S);
};
#endif

