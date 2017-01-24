#!/bin/sh

#  toLua.sh
#  FCTDemo
#
#  Created by Louis on 13-8-14.
#  Copyright (c) 2013年 Louis. All rights reserved.

#tolua=/Users/ryan/Project/LUA/tolua++-1.0.93/bin/tolua++
tolua=/usr/local/lib/tolua++

#tolua=/usr/bin/code/lua/tolua++

$tolua -o CIANetFixture_lua.mm CIANetFixture_lua.pkg

$tolua -o CIANetFixture_Object.mm CIANetFixture_Object.pkg
