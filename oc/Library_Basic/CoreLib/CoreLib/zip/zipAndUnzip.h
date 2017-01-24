//
//  zipAndUnzip.h
//  ZipTest
//
//  Created by wangyinan on 14-1-24.
//  Copyright (c) 2014年 wangyinan. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface zipAndUnzip : NSObject

+ (NSData *)gzipInflate:(NSData*)data;
+ (NSData *)gzipDeflate:(NSData*)data;

@end
