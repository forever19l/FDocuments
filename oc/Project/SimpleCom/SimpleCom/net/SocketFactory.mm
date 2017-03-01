//
//  IASocket.mm
//  IANetFixture
//
//  Created by Yuekie on 7/5/16.
//  Copyright (c) 2016 Louis. All rights reserved.
//

#import "SocketFactory.h"
#include "pthread.h"
#import <CoreLib/Common.h>
#import <CoreLib/TestContext.h>
#import "SimpleCom_Global.h"
#import <CoreLib/Common.h>
#include <CoreLib/PathManager.h>
#import "RegexKitLite.h"
#import <RegexKit/RegexKit.h>

@implementation SocketFactory

-(id)init
{
    self = [super init];
    if (self)
    {
        pthread_mutex_init(&m_socketmutex, NULL);
        pthread_mutex_init(&m_SendCmdmutex,NULL);
        m_Socketinputdata=[[NSMutableData alloc]init];
        m_UartLog = [[NSMutableString alloc] init];
        m_socket = nil;
        m_simpleComSetting=nil;
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(SaveLogNotification:) name:kNotificationSaveLog object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(EmptyLogNotification:) name:kNotificationOnEngineStart object:nil];
    }
    
    return self;
}

-(BOOL)CheckResourceName:(NSString*)Resource
{
    return [Resource isEqualToString:[m_simpleComSetting getSetting]];
}
-(id)initwithName:(SimpleComSetting*)simpleComSetting
{
    self = [super init];
    if (self)
    {
        pthread_mutex_init(&m_socketmutex, NULL);
        pthread_mutex_init(&m_SendCmdmutex,NULL);
        m_Socketinputdata=[[NSMutableData alloc]init];
        m_UartLog = [[NSMutableString alloc] init];
        m_socket = nil;
        m_simpleComSetting=[simpleComSetting retain];
        m_statusview=[m_simpleComSetting getconnectStatusView];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(SaveLogNotification:) name:kNotificationSaveLog object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(EmptyLogNotification:) name:kNotificationOnEngineStart object:nil];
    }
    return self;
}
-(void)EmptyLogNotification:(NSNotification *)nf
{
    [m_UartLog setString:@""];
    SerialPortView *mTVOutput=[m_simpleComSetting getLogView];
    [[mTVOutput textStorage] replaceCharactersInRange:NSMakeRange(0, [[mTVOutput textStorage] length]) withString:@""];
}

-(void)SaveLogNotification:(NSNotification *)nf
{
    if (![m_simpleComSetting isSaveLog]) {
        return;
    }
    NSDictionary * dic = [nf userInfo];
    int index = [[dic objectForKey:KEY_FIXTURE_ID] intValue];
    NSString * strpath = [dic objectForKey:KEY_NF_UARTPATH];
    if (strpath)
    {
        NSString* path = [[strpath lastPathComponent] stringByReplacingOccurrencesOfString:@"uart" withString:[m_simpleComSetting getInterfaceName]];
        strpath = [[strpath stringByDeletingLastPathComponent]stringByAppendingString:[NSString stringWithFormat:@"/%@",path]];
        NSLog(@"Save DUT Log : %@ at index: %d",strpath,index);
        [self SaveLog:strpath index:index];
    }
}
-(int)SaveLog:(NSString *)filepath index:(int)uID
{
    
    [[NSFileManager defaultManager] createDirectoryAtPath:[filepath stringByDeletingLastPathComponent] withIntermediateDirectories:YES attributes:nil error:nil];
    NSError * err;
    //SerialPortView *mTVOutput=[m_simpleComSetting getLogView];
    BOOL b =  [m_UartLog writeToFile:filepath atomically:YES encoding:NSUTF8StringEncoding error:&err];
    if (!b)
    {
        NSString * strError = [NSString stringWithFormat:@"Save DUT log failed!\r\n%@",[err description]];
        NSRunAlertPanel(@"Save DUT", @"%@", @"OK", nil, nil, strError);
        return -1;
    }
    if (![m_simpleComSetting isSendLogtoPDCA]) {
        return 0;
    }
    if ([[CTestContext::m_dicConfiguration valueForKey:kConfigPuddingBlobUart] intValue])
    {
        CScriptEngine * pEngine = (CScriptEngine *)[pTestEngine GetScripEngine:0];
        char szAddBlobstring[255];
        memset(szAddBlobstring, 0, 255);
        sprintf(szAddBlobstring, "DataLogger:AddBlob(\"%s\");",[filepath UTF8String] );
        int err = pEngine->DoString(szAddBlobstring);
        if (err)
        {
            NSRunAlertPanel(@"Add Uartlog", @"AddBlob fail %@,with message :\r\n%s", @"OK", nil, nil,filepath,lua_tostring(pEngine->m_pLuaState, -1));
        }
    }
    return b;
}
-(void)SetSimpleComSetting:(SimpleComSetting*)simpleComSetting
{
    SimpleComSetting *old=m_simpleComSetting;
    m_simpleComSetting=[simpleComSetting retain];
    m_statusview=[m_simpleComSetting getconnectStatusView];
    [old release];
}
-(SimpleComSetting*)GetSimpleComSetting
{
    return m_simpleComSetting;
}
-(BOOL)OpenSimpleCom
{
    NSString * strSetting=[m_simpleComSetting getSetting];
    NSArray * arrSetting=[strSetting componentsSeparatedByString:@";"];
    NSString* strIP=[arrSetting objectAtIndex:0];
    if (strIP==nil || strIP.length==0)
        return NO;
    NSString* strport=[arrSetting objectAtIndex:1];
    if (strport==nil || [strport intValue]<100)
        return NO;
    return [self connectSocket:strIP Port:[strport intValue] timeOut:5];
}

-(bool)WriteString:(const char *)szCMD
{
    if ([self isOpen])
    {
        if (strlen(szCMD)>0)
        {
             if ([m_simpleComSetting isEcho])
             {
                 
                NSTextView *m_testview=[m_simpleComSetting getLogView];
                NSString *str1=[NSString stringWithFormat:@"send :%s",szCMD];
                [self performSelectorOnMainThread:@selector(logOut:) withObject:[NSDictionary dictionaryWithObjectsAndKeys:str1,@"Message",m_testview,@"TextView",nil] waitUntilDone:YES];
            
             }
            return (int)[m_socket write:szCMD maxLength:strlen(szCMD)];
        }
    }
    return 0;
}

-(void)AddLabelString:(const char *)LabelStr
{
    NSString* Str = [NSString stringWithUTF8String:LabelStr];
    [self performSelectorOnMainThread:@selector(logOut:) withObject:[NSDictionary dictionaryWithObjectsAndKeys:Str,@"Message",[m_simpleComSetting getLogView],@"TextView",nil] waitUntilDone:YES];
}
-(const char *)ReadStringRegex:(const char*)Regex Timeout:(int)timeout
{
    NSTimeInterval starttime = [[NSDate date]timeIntervalSince1970];
    NSString * strData;
    NSString * strRegex=[NSString stringWithUTF8String:Regex];
    double tm = (double)timeout/1000.0;
    
    while (1) {
        @synchronized(m_Socketinputdata)
        {
            
            NSRange range=[m_Socketinputdata rangeOfRegex:strRegex];
            if (range.location!=NSNotFound) {
                strData=[[[NSString alloc] initWithData:[m_Socketinputdata subdataWithRange:range] encoding:NSUTF8StringEncoding] autorelease];
                [m_Socketinputdata setLength:0];
                return [[NSString stringWithString:strData] UTF8String];
            }
        }
        NSTimeInterval now = [[NSDate date]timeIntervalSince1970];
        if ((now-starttime)>=tm)
        {
            return nil;       //time out
        }
        if ([[NSThread currentThread] isCancelled])
        {
            return nil;
        }
        [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate distantFuture]];
        [NSThread sleepForTimeInterval:0.01];
    }
    return nil;
}
-(const char*)ReadString
{
    NSData * data;
    @synchronized(m_Socketinputdata)
    {
        data = [NSData dataWithData:m_Socketinputdata];
        [m_Socketinputdata setLength:0];
    }
    //return (const char*)[data bytes];
    NSString *rtnStr = [[[NSString alloc]initWithData:data encoding:NSASCIIStringEncoding] autorelease];
    return rtnStr.UTF8String;
}
-(int)SetDetectString:(const char *)regex
{
    [self ClearBuffer];
    NSData *old=m_strSerialWait;
    m_strSerialWait=[[NSData alloc] initWithBytes:regex length:strlen(regex)];
    [old release];
    return 0;
}
-(int)WaitForString:(int) iTimeout
{
    BOOL bTrigger= NO;
    NSTimeInterval starttime = [[NSDate date]timeIntervalSince1970];
    double tm = (double)iTimeout/1000.0;
    NSRange range;
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
        @synchronized(m_Socketinputdata)
        {
            range= [m_Socketinputdata rangeOfRegex:@""];
            range=[m_Socketinputdata rangeOfData:m_strSerialWait options:NSDataSearchBackwards range:NSMakeRange(0, [m_Socketinputdata length])];
            if (range.location!=NSNotFound)
            {
                break;
            }
            
        }
        [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate date]];
        [NSThread sleepForTimeInterval:0.01];
    }
    return bTrigger;        //YES: Get Signal NO:TimeOut
}
-(void)ClearBuffer
{
    @synchronized(m_Socketinputdata)
    {
        [m_Socketinputdata setLength:0];
    }
}
-(void)dealloc
{
    [m_Socketinputdata release];
    [m_UartLog release];
    [self closeSimpleCom];
    [[NSNotificationCenter defaultCenter] removeObserver:self] ;
    [super dealloc];
}

-(void)closeSimpleCom
{
    if (m_socket)
    {
        [m_socket close];
        [m_socket release];
        m_socket = nil;
    }
}

- (BOOL)connectSocket:(NSString*) host Port:(UInt32) port timeOut:(double)timeout
{
    if (host != nil)
    {
        [self closeSimpleCom];
        m_socket = [[IASocketConnection alloc] initWithHost:[host UTF8String] Port:port];
        [m_socket setDelegate:self];
        [self SetDetectString:"\r\n"];
        return [m_socket open:timeout];
    }
    
    return NO;
}

- (BOOL)isOpen
{
    if (m_socket == nil)
    {
        return NO;
    }
    return [m_socket isOpen];
}

-(void)ShowMsg:(NSString*)msg
{
    [self performSelectorOnMainThread:@selector(fnErrMsg:) withObject:msg waitUntilDone:NO];
}

-(void)fnErrMsg:(id)par
{
//    NSDictionary* dic = [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithInt:0],@"id",par,@"msg",nil];
    NSRunAlertPanel(@"SocketError",par, @"OK", nil, nil);
//    [[NSNotificationCenter defaultCenter] postNotificationName:kNotificationOnTestError object:nil userInfo:dic];
}
-(void)logOut:(NSDictionary*)dic
{
    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
    [dateFormatter setDateFormat:@"yyyy/MM/dd HH:mm:ss.SSS : "];
    int length = 0;
    NSTextView* view = [dic objectForKey:@"TextView"];
    NSString* msg = [dic objectForKey:@"Message"];
    NSAttributedString *theString;
    NSRange theRange;
    NSString * str = [NSString stringWithFormat:@"%@ %@\r",[dateFormatter stringFromDate:[NSDate date]],msg];
    [m_UartLog appendString:str];
    if ([SimpleComSetting isDebugOut])
    {
        theString = [[NSAttributedString alloc] initWithString:str];
        [[view textStorage] appendAttributedString: theString];
        NSTextView *m_testview=[m_simpleComSetting getLogView];
        length = (int)[[m_testview textStorage] length];
        
        if ([[[view textStorage] string] length] > 1000000)
        {
            NSLog(@"The length of log data is too big, much then 1000000 bytes, cleared at %@",str);
            [[view textStorage] replaceCharactersInRange:NSMakeRange(0, [[view textStorage] length]) withString:@"Too much data, cleared to save memory\r\n"];
        }
        
        theRange = NSMakeRange(length, 0);
        [view scrollRangeToVisible:theRange];
        
        if (theString)
        {
            [theString release];
        }
        [view setNeedsDisplay:YES];
    }
    if (dateFormatter)
    {
        [dateFormatter release];
    }

}
- (void)dataready:(NSStream *)aStream handleEvent:(NSStreamEvent)eventCode
{
    NSInteger actuallyRead;
    NSInputStream *instream = (NSInputStream*)aStream;
    memset(m_tmpbuffer, 0, sizeof(m_tmpbuffer));
    actuallyRead = [instream read:(uint8_t *)m_tmpbuffer maxLength:sizeof(m_tmpbuffer)];
    if(actuallyRead<1)
    {
        NSLog(@"read data error(data lenght=%ld)",actuallyRead);
        return;
    }
    @synchronized(m_Socketinputdata)
    {
        [m_Socketinputdata appendBytes:m_tmpbuffer length:actuallyRead];
    }
    m_tmpbuffer[actuallyRead]='\0';
    NSTextView *m_testview=[m_simpleComSetting getLogView];
    NSString * readString = [NSString stringWithFormat:@"recv :%s",(char*)m_tmpbuffer];

    [self performSelectorOnMainThread:@selector(logOut:) withObject:[NSDictionary dictionaryWithObjectsAndKeys:readString,@"Message",m_testview,@"TextView",nil] waitUntilDone:YES];
}

- (void)streamerror:(NSStream *)aStream handleEvent:(NSStreamEvent)eventCode
{
    switch ([aStream streamStatus]) {
        case NSStreamStatusNotOpen:
            [self ShowMsg:@"connect PLC Command error"];
            [self performSelectorOnMainThread:@selector(SocketStatus:) withObject:[NSNumber numberWithBool:NO] waitUntilDone:YES];
            break;
        case NSStreamStatusOpening:
            [self ShowMsg:@"connecting to PLC Command server, please wait a minute"];
            break;
        case NSStreamStatusOpen:
            break;
        case NSStreamStatusReading:
            [self ShowMsg:@"local cell busy"];
            break;
        case NSStreamStatusWriting:
            [self ShowMsg:@"local cell busy"];
            break;
        case NSStreamStatusAtEnd:
            [self closeSimpleCom];
            [self ShowMsg:@"PLC Command Server stop"];
            [self performSelectorOnMainThread:@selector(SocketStatus:) withObject:[NSNumber numberWithBool:NO] waitUntilDone:YES];
            break;
        case NSStreamStatusClosed:
            [self closeSimpleCom];
            [self ShowMsg:@"PLC Command Server stop"];
            [self performSelectorOnMainThread:@selector(SocketStatus:) withObject:[NSNumber numberWithBool:NO] waitUntilDone:YES];
            break;
        case NSStreamStatusError:
            [self closeSimpleCom];
            [self ShowMsg:@"PLC Command Server error"];
            [self performSelectorOnMainThread:@selector(SocketStatus:) withObject:[NSNumber numberWithBool:NO] waitUntilDone:YES];
            break;
        default:
            break;
    }
}

- (void)ConnectTimeout:(NSTimer *)timer Socket:(IASocketConnection*)socket
{
    [self closeSimpleCom];
    [self performSelectorOnMainThread:@selector(SocketStatus:) withObject:[NSNumber numberWithBool:NO] waitUntilDone:YES];
    [self ShowMsg:@"connect to IANetFixture server timeout"];
}

- (void)opencompleted:(NSStream *)aStream handleEvent:(NSStreamEvent)eventCode
{
//     ColorButtonView *m_statusview=[m_simpleComSetting getconnectStatusView];
    [self performSelectorOnMainThread:@selector(SocketStatus:) withObject:[NSNumber numberWithBool:YES] waitUntilDone:YES];
}
-(void)SocketStatus:(NSNumber*)number
{
    if ([number boolValue]) {
        [m_statusview setState:1];
    }else
    {
        [m_statusview setState:0];
    }
}
@end
