
//
//  tag.cpp
//  CoreLib
//
//  Created by intelligent on 13-7-16.
//
//

#include "tag.h"
#include "SerialPortView.h"

static int AddClassFunction(lua_State * L,int index,NSMutableArray * arrList)
{
    lua_pushnil(L);
    NSMutableDictionary * dic = [NSMutableDictionary new];
    while (lua_next(L, index-1)) {
        const char * key =lua_tostring(L, -2);
        const char * value = lua_tostring(L, -1);
        //printf("%s=%s\r\n",key,value);
        [dic setValue:[NSString stringWithUTF8String:value] forKey:[NSString stringWithUTF8String:key]];
        lua_pop(L, 1);
    }
    [arrList addObject:dic];
    [dic release];
    return 0;
}

static int ListClassFunction(lua_State * L,int index,NSMutableArray * arrList)
{
    lua_pushnil(L);
    if (!lua_next(L, index-1))
    {
        //lua_pop(L, 2);
        return -1;
    }
    //printf("key : %s, Value : %s\r\n",lua_tostring(L, -2),lua_tostring(L, -1));
    //[self AddKeyItem:L stack:-1];
    AddClassFunction(L, -1,arrList);
    lua_pop(L, 2);
    int __index=1;
    while (1) {
        lua_pushinteger(L, __index++);
        int ret = lua_next(L, index-1);
        if (!ret)
        {
            //lua_pop(L, 2);
            break;
        }
        //printf("key : %s, Value : %s\r\n",lua_tostring(L, -2),lua_tostring(L, -1));
        //[self AddKeyItem:L stack:-1];
        AddClassFunction(L, -1,arrList);
        lua_pop(L, 2);
    }
    return 0;
}

int ListLuaFile(const char * filepath,const char * table)
{
    NSMutableString * strContents = [[NSMutableString alloc]init];
    NSMutableArray * arrClassFunction = [NSMutableArray new];
    @try {
        NSBundle * bundle = [NSBundle bundleForClass:[SerialPortView class]];
        const char * parser_file = [[bundle pathForResource:@"luatag" ofType:@"lua"] UTF8String];
        lua_State * L=luaL_newstate();
        luaL_openlibs(L);
        //luaL_dostring(L, LUA_TABLE);
        int err=0;
        err=luaL_dofile(L, parser_file);
        if (err)
        {
            NSString * strError = [NSString stringWithUTF8String:lua_tostring(L, -1)];
            @throw [NSException exceptionWithName:@"Lua Error" reason:strError userInfo:nil];
        }
        lua_getglobal(L, "Start");
        lua_pushstring(L, filepath);
        err = lua_pcall(L, 1, 3, 0);    //start parser
        if (err)
        {
            NSString * strError = [NSString stringWithUTF8String:lua_tostring(L, -1)];
            @throw [NSException exceptionWithName:@"Lua Error" reason:strError userInfo:nil];
        }
        //-3 : g_ClassFunctionName
        //-2 : g_GolbalFunctionName
        //-1 : g_GlobalVariablName
        printf("%d,%d,%d",lua_type(L, -1),lua_type(L, -2),lua_type(L, -3));
        ListClassFunction(L, -3,arrClassFunction);
        
        //append to tag
        
        for (NSDictionary * dic in arrClassFunction)
        {
            NSString * name = [dic valueForKey:@"name"];
            if (table)
            {
                name=[NSString stringWithFormat:@"%s.%@",table,name];
            }
            NSString * proto = [dic valueForKey:@"proto"];
            NSString * line = [NSString stringWithFormat:@"%@\t%s\t/^%@/\r\n",name,filepath,proto];
            [strContents appendString:line];
        }
        NSString * strtag = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@tag_file];
        NSFileHandle * file = [NSFileHandle fileHandleForWritingAtPath:strtag];
        [file seekToEndOfFile];
        
        [file writeData:[strContents dataUsingEncoding:NSASCIIStringEncoding]];
        
        [file closeFile];
        
        lua_close(L);
        
    }
    @catch (NSException *exception) {
    }
    @finally {
        [strContents release];
        [arrClassFunction release];
    }

    return 0;
}
int AddTags(NSDictionary * dic)
{
    //NSString * strType = [dic valueForKey:@kTagType];
    NSString * strValue = [dic valueForKey:@kTagValue];
    if (!strValue) return -1;
    NSString * strtag = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@tag_file];
    NSFileHandle * file = [NSFileHandle fileHandleForWritingAtPath:strtag];
    [file seekToEndOfFile];
    
    [file writeData:[strValue dataUsingEncoding:NSASCIIStringEncoding]];
    
    [file closeFile];
    return 0;
}