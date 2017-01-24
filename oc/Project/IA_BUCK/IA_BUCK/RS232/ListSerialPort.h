//
//  ListSerialPort.h
//  IA_BUCK
//
//  Created by IvanGan on 16/4/26.
//  Copyright © 2016年 mess. All rights reserved.
//

#import <Foundation/Foundation.h>
#include <IOKit/IOTypes.h>

@interface ListSerialPort : NSObject

+(NSMutableArray *)ListSerialPort;

+ (void)addAllSerialPortsToArray:(NSMutableArray * )array;
+ (NSString *)getNextSerialPort:(io_iterator_t) serialPortIterator;

@end
