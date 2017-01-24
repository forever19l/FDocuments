//
//  zipAndUnzip.h
//  ZipTest
//
//  Created by wangyinan on 14-1-24.
//  Copyright (c) 2014年 wangyinan. All rights reserved.
//
/* 直接对数据的压缩、解压缩基础类 */
#import <Foundation/Foundation.h>

@interface zipAndUnzip : NSObject

+ (NSData *)gzipInflate:(NSData*)data;//解压
+ (NSData *)gzipDeflate:(NSData*)data;

@end
