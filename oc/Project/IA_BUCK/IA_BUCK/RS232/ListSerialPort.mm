//
//  ListSerialPort.m
//  IA_BUCK
//
//  Created by IvanGan on 16/4/26.
//  Copyright © 2016年 mess. All rights reserved.
//

#import "ListSerialPort.h"

#include <IOKit/IOKitLib.h>
#include <IOKit/serial/IOSerialKeys.h>
#include <IOKit/serial/ioss.h>
#include <IOKit/IOBSD.h>

@implementation ListSerialPort

+ (void)addAllSerialPortsToArray:(NSMutableArray * )array
{
    kern_return_t kernResult;
    CFMutableDictionaryRef classesToMatch;
    io_iterator_t serialPortIterator;
    NSString * serialPort;
    // Serial devices are instances of class IOSerialBSDClient
    classesToMatch = IOServiceMatching(kIOSerialBSDServiceValue);
    if (classesToMatch != NULL) {
        CFDictionarySetValue(classesToMatch, CFSTR(kIOSerialBSDTypeKey), CFSTR(kIOSerialBSDAllTypes));
        
        // This function decrements the refcount of the dictionary passed it
        kernResult = IOServiceGetMatchingServices(kIOMasterPortDefault, classesToMatch, &serialPortIterator);
        if (kernResult == KERN_SUCCESS) {
            while ((serialPort = [self getNextSerialPort:serialPortIterator]) != nil) {
                [array addObject:serialPort];
            }
            (void)IOObjectRelease(serialPortIterator);
        }
        else {
            NSLog(@"IOServiceGetMatchingServices returned %d", kernResult);
        }
    }
    else {
        NSLog(@"IOServiceMatching returned a NULL dictionary.");
    }
}

+ (NSString *)getNextSerialPort:(io_iterator_t) serialPortIterator
{
    io_object_t serialService = IOIteratorNext(serialPortIterator);
    if (serialService != 0) {
        NSString * str;
        CFStringRef modemName = (CFStringRef)IORegistryEntryCreateCFProperty(serialService, CFSTR(kIOTTYDeviceKey), kCFAllocatorDefault, 0);
        CFStringRef bsdPath = (CFStringRef)IORegistryEntryCreateCFProperty(serialService, CFSTR(kIOCalloutDeviceKey), kCFAllocatorDefault, 0);
        CFStringRef serviceType = (CFStringRef)IORegistryEntryCreateCFProperty(serialService, CFSTR(kIOSerialBSDTypeKey), kCFAllocatorDefault, 0);
        if (modemName && bsdPath) {
            NSLog(@"list: %@",bsdPath);
            str = [NSString stringWithString:(__bridge NSString *)bsdPath];
        }
        if (modemName)   CFRelease(modemName);
        if (bsdPath)     CFRelease(bsdPath);
        if (serviceType) CFRelease(serviceType);
        
        // We have sucked this service dry of information so release it now.
        (void)IOObjectRelease(serialService);
        return str;
    }
    return nil;
}

+ (NSMutableArray *)ListSerialPort
{
    NSMutableArray * arr = [NSMutableArray array];
    [self addAllSerialPortsToArray:arr];
    return arr;
}

@end
