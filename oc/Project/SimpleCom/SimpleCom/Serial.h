//
//  Serial.h
//  Serial
//
//  Created by Liu Liang on 14-1-5.
//  Copyright (c) 2014年 Liu Liang. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <VISA/VISA.h>

@interface Serial : NSObject
{
    ViSession defaultRM;
    ViSession instr;
    ViUInt32 writeCount;
    ViUInt32 retCount;
}
- (id)initWithInstr:(char*)instr_name rate:(int)rate bits:(int)bits parity:(int)parity stop:(int)stop;
- (void)clear;
- (void)setTimeout:(int)timemout;
- (void)setDetectChar:(char)c;
- (int)send:(char*)cmd;
- (NSString*)recv;
- (void)dealloc;
@end

