//
//  SerialportEx.m
//  iTMP
//
//  Created by Ryan on 12-2-8.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#import "SerialportEx.h"


@implementation SerialportEx

@synthesize m_strSerialWait;
@synthesize m_strDetected;
- (id)init
{
    self = [super init];
    if (self) {
        m_DetectCondition = [[NSCondition alloc]init];
        self.m_strDetected=@"\n";
        self.m_strSerialWait=@"";
    }
    
    return self;
}

- (id)init:(NSString *)path withName:(NSString *)name type:(NSString *)type
{
    self = [super init:path withName:name type:type];
    if (self) {
        m_DetectCondition = [[NSCondition alloc] init];
        self.m_strDetected=@"\n";
        self.m_strSerialWait=@"";
        [self setDelegate:self];
    }
    return self;
}

-(NSFileHandle *)open
{
    [self setDelegate:self];
    NSFileHandle * f = [super open];
    
    if (f>=0)
    {
        [NSThread detachNewThreadSelector:@selector(readDataInBackgroundThread) toTarget:self withObject:nil];
    }
    return f;
}
    

- (void)dealloc
{
//    [strBuf release];
    [m_DetectCondition release];
    [super dealloc];
}

-(int)SetDetectString:(NSString *)strDectect
{
    self.m_strSerialWait = strDectect;
//    m_strSerialWait = [NSString stringWithString:strDectect];
//    [strBuf setString:@""];     //Clear buffer
    [self ClearInBuffer];

    return 0;
}

-(int)WaitDetect
{
#if 0
    if (bDetectFlags == YES) return 0;
    [m_DetectCondition lock];
    [m_DetectCondition wait];       //待测试如果先已监测到信号，后再等待是否会被阻塞
    [m_DetectCondition unlock];
#else
    return [self WaitDetect:30000];      //defautl: 30s;
#endif
}

-(int)WaitDetect:(int)iTimeOut
{
    BOOL bTrigger= NO;
    NSTimeInterval starttime = [[NSDate date]timeIntervalSince1970];
    double tm = (double)iTimeOut/1000.0;
    while (1) {
        NSTimeInterval now = [[NSDate date]timeIntervalSince1970];
        if ((now-starttime)>=tm)
        {
            return -1;       //time out
        }
        
        if ([[NSThread currentThread] isCancelled])
        {
            return -2;
        }
        
        NSString * str;
        
        @synchronized(self.dataInput)
        {
            str = [[NSString alloc]initWithData:self.dataInput encoding:NSASCIIStringEncoding];
        }
        
        NSRange range = [str rangeOfString:self.m_strSerialWait];
        
        [str release];
        if (range.location!=NSNotFound)
        {
            break;
        }
        
        [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate date]];
        [NSThread sleepForTimeInterval:0.01];
    }
    return bTrigger;        //YES: Get Signal NO:TimeOut
}

-(int)ResetCondition
{
    [m_DetectCondition lock];
    [m_DetectCondition broadcast];
    [m_DetectCondition unlock];
    m_strSerialWait=@"";
    return 0;
}
-(int)PortSetting:(NSString *)strSetting
{
     NSArray * arr = [[strSetting uppercaseString] componentsSeparatedByString:@","];
    if  ([arr count]!=4) return -1;
    [self  setSpeed:[[arr objectAtIndex:0] intValue]];
    NSString * str = [arr objectAtIndex:1];
    AMSerialParity p;
    if ([str isEqualToString:@"E"]) {
        p = kAMSerialParityEven;
    }
    else if([str isEqualToString:@"O"])
    {
        p = kAMSerialParityOdd;
    }
    else
    {
        p=kAMSerialParityNone;
    }
    [self setParity:p];
    [self setDataBits:[[arr objectAtIndex:2]intValue]];
    [self setStopBits:[[arr objectAtIndex:3] intValue]];
    return 0;    
}


-(NSString *)QueryString:(NSString *)strcmd
{
    return nil;
//    [strBuf setString:@""];
    [self writeString:strcmd usingEncoding:NSASCIIStringEncoding error:nil];
    [NSThread sleepForTimeInterval:1];
    NSString * strdata = @"";
    return strdata;
}

-(BOOL)WriteStringValue:(NSString *)strVal
{
//    [strBuf setString:@""];
    return [self writeString:strVal usingEncoding:NSASCIIStringEncoding error:nil];
}

-(NSString *)ReadStringValue
{
    return [self ReadStringValue:5000];     //default timeout = 5s;
}

-(NSString *)ReadStringValue:(int)timeout
{
    NSTimeInterval starttime = [[NSDate date]timeIntervalSince1970];
    NSString * strData=nil;;
    double tm = (double)timeout/1000.0;
    while (1) {
        NSTimeInterval now = [[NSDate date]timeIntervalSince1970];
        if ((now-starttime)>=tm)
        {
            return nil;       //time out
        }
        
        @synchronized(self.dataInput)
        {
            strData = [[NSString alloc]initWithData:self.dataInput encoding:NSASCIIStringEncoding];
            if (strData && [strData length]>0)
            {
                [self.dataInput setLength:0];
                break;
            }else
            {
                [strData release];
            }
        }
        [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate distantFuture]];
        [NSThread sleepForTimeInterval:0.01];
    }
    NSString *strRET=[NSString stringWithString:strData];
    [strData release];
    return strRET;
}

-(void)ClearInBuffer
{
    BOOL haveData = YES;
    while (haveData)
    {
        @synchronized(self.dataInput)
        {
            haveData = ([self.dataInput length] > 0);
            [self.dataInput setLength:0];
        }
        
        if (haveData) [NSThread sleepForTimeInterval:0.025];
    }

}
-(NSData *)GetInBuffer
{
    NSData * data;
    
    @synchronized(self.dataInput)
    {
        data = [NSData dataWithData:self.dataInput];
        [self.dataInput setLength:0];
    }
    return data;
}

-(int)AttachView:(id<SerialView>)view
{
    m_SerialView = view;
    return 0;
}

-(void)SetCallback:(SEL)Callback Target:(id)object
{
    m_SerialCallBack = Callback;
    m_CallbackTarget = object;
}

- (void)serialPortReadData:(NSDictionary *)dataDictionary
{
	// this method is called if data arrives 
	// @"data" is the actual data, @"serialPort" is the sending port
	NSData *data = [dataDictionary objectForKey:@"data"];
	if (data && [data length] > 0) {
        [m_SerialView InsertData:data];
        
        if (m_CallbackTarget)
        {
            [m_CallbackTarget performSelector:m_SerialCallBack withObject:data];
        }
        
		// continue listeningn
	} else { // port closed
		[m_SerialView insertText:@"port closed\r"];
	}
    return;
}
@end
