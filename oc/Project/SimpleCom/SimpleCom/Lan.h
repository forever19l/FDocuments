//
//  Serial.h
//  Serial
//
//  Created by Liu Liang on 14-1-5.
//  Copyright (c) 2014年 Liu Liang. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <VISA/VISA.h>

#define LAN_RECV_BUFFER_SIZE 0x1000000

@interface Lan : NSObject
{
    ViSession defaultRM;
    ViSession instr;
    ViUInt32 writeCount;
    ViUInt32 retCount;
    
    char RecvBuffer[LAN_RECV_BUFFER_SIZE];
}
- (id)initWithInstr: (char*)ipAddr;
- (void)clear;
- (void)setTimeout:(int)timemout;
- (void)setDetectChar:(char)c;
- (int)send:(char*)cmd;
- (NSString*)recv;
- (void)dealloc;
@end

