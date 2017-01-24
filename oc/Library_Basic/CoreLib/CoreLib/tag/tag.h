//
//  tag.h
//  CoreLib
//
//  Created by intelligent on 13-7-16.
//
//

#ifndef __CoreLib__tag__
#define __CoreLib__tag__


#include <iostream>
#include <lua.hpp>
#include <Foundation/Foundation.h>

#define tag_file    "tmtags"

#define kTagType    "tag_type"
#define kTagValue   "tag_type"


int ListLuaFile(const char * filepath,const char * table=NULL);
int AddTags(NSDictionary * dic);
NSDictionary * GetTag();



#endif /* defined(__CoreLib__tag__) */
