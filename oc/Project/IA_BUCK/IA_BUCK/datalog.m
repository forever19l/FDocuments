//
//  datalog.m
//  IA_BUCK
//
//  Created by IvanGan on 16/4/25.
//  Copyright © 2016年 mess. All rights reserved.
//

#import "datalog.h"
#import "UICommon.h"

#define FIRSTLINE @"Intelligent,V1.0,%@"//station name
#define SECONDLINE @"Serial Number,Station ID,Site ID,PASS/FAIL,ErrorMessage,Fail Item,Test Start Time,Total Test Time,%@"//item
#define THIRDLINE @"upper Limited,,,,,,,,%@"//limit
#define FOURTHLINE @"lower Limited,,,,,,,,%@"//limit
#define FIFTHLINE @"unit,,,,,,,,%@"//limit
#define CONTENT @"%@,%@,%@,%@,%@,%@,%@,%@,%@\n"//content

@implementation datalog

- (id)init
{
    self = [super init];
    return self;
}

- (void)dealloc
{
    [super dealloc];
}

- (void)saveLog:(NSMutableDictionary *)dic :(NSArray*)key
{
    NSDateFormatter * fmt = [[NSDateFormatter alloc]init];
    [fmt setDateFormat:@"yyyy-MM-dd"];
    NSString * date = [fmt stringFromDate:[NSDate date]];
    [fmt release];
    NSString * filePath = [NSString stringWithFormat:@"%@/%@.csv",[dic objectForKey:keyCSV ],[NSString stringWithFormat:@"%@-%@",[dic objectForKey:keySN],date]];
    if(![[NSFileManager defaultManager]fileExistsAtPath:filePath])
    {
        NSString * first = [NSString stringWithFormat:FIRSTLINE,[dic objectForKey:keySTATION]];
        NSMutableString * itemname = [NSMutableString string];
        NSMutableString * upper = [NSMutableString string];
        NSMutableString * lower = [NSMutableString string];
        NSMutableString * unit = [NSMutableString string];
        for(int i=0;i<[key count]; i++)
        {
            NSDictionary * item = [dic objectForKey:[key objectAtIndex:i]];
            [itemname appendString:[NSString stringWithFormat:@"%@,",[item objectForKey:keyNAME]]];
            [upper appendString:[NSString stringWithFormat:@"%@,",[item objectForKey:keyUPPER]]];
            [lower appendString:[NSString stringWithFormat:@"%@,",[item objectForKey:keyLOWER]]];
            [unit appendString:[NSString stringWithFormat:@"%@,",[item objectForKey:keyUNIT]]];
        }
        NSString * second = [NSString stringWithFormat:SECONDLINE,itemname];
        NSString * third = [NSString stringWithFormat:THIRDLINE,upper];
        NSString * fourth = [NSString stringWithFormat:FOURTHLINE,lower];
        NSString * fifth = [NSString stringWithFormat:FIFTHLINE,unit];
        NSString * content = [NSString stringWithFormat:@"%@\n%@\n%@\n%@\n%@\n",first,second,third,fourth,fifth];
        [content writeToFile:filePath atomically:YES encoding:NSUTF8StringEncoding error:nil];
    }
    NSMutableString * value = [NSMutableString string];
    for(int i=0;i<[key count]; i++)
    {
        NSDictionary * item = [dic objectForKey:[key objectAtIndex:i]];
        [value appendString:[NSString stringWithFormat:@"%@,",[item objectForKey:keyRESULT]]];
    }
    NSString * cont = [NSString stringWithFormat:CONTENT,[dic objectForKey:keySN],[dic objectForKey:keySTATIONID],[dic objectForKey:keySITEID],[dic objectForKey:keySTATUS],[dic objectForKey:keyERRORMSG],[dic objectForKey:keyFAILITEM],[dic objectForKey:keySTARTTIME],[dic objectForKey:keyTOTALTIME],value];
    
    NSFileHandle * file = [NSFileHandle fileHandleForWritingAtPath:filePath];
    [file seekToEndOfFile];
    [file writeData:[cont dataUsingEncoding:NSUTF8StringEncoding]];
    [file closeFile];
}

@end

