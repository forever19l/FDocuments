//
//  luaOperation.h
//  CoreLib
//
//  Created by intelligent on 13-7-18.
//
//

#import <Foundation/Foundation.h>
#include <lua.hpp>
@interface luaOperation : NSOperation{
    NSString * strTestItem;
    NSMutableDictionary * dicResult;
    lua_State * m_pState;
}
-(id)initWithLua:(lua_State *)L;
@property (readonly)NSDictionary * dicResult;
@property (copy)NSString * strTestItem;
@end
