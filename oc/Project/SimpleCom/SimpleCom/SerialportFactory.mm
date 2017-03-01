//
//  SerialportFactory.m
//  SimpleCom
//
//  Created by ZL-Pro on 16/8/13.
//  Copyright (c) 2016年 ZL-Pro. All rights reserved.
//

#import "SerialportFactory.h"
#import "SimpleCom_Global.h"
#import <CoreLib/Common.h>
#include <CoreLib/PathManager.h>

@implementation SerialportFactory
- (instancetype)init
{
    self = [super init];
    if (self) {
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(didRemovePorts:) name:AMSerialPortListDidRemovePortsNotification object:nil];
        m_UartLog=[[NSMutableString alloc]init];
        m_recive_dataInput=[[NSMutableData alloc] init];
        m_simpleComSetting=nil;
        m_mPboardComm=nil;
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(SaveLogNotification:) name:kNotificationSaveLog object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(EmptyLogNotification:) name:kNotificationOnEngineStart object:nil];
    }
    return self;
}

- (void)dealloc
{
    if (m_mPboardComm!=nil) {
        [m_mPboardComm close] ;
        [m_mPboardComm release] ;
        m_mPboardComm=nil;
    }
    [m_simpleComSetting release];
    [m_UartLog release];
    [m_recive_dataInput release];
    [[NSNotificationCenter defaultCenter] removeObserver:self] ;
    [super dealloc];
}

- (void)didRemovePorts:(NSNotification *)theNotification
{
    NSArray *arr=(NSArray*)[[theNotification userInfo] objectForKey:AMSerialPortListRemovedPorts];
    for (AMSerialPort *port in arr)
    {
        NSLog(@"port=%@\n",[port name]);
        NSLog(@"m_mPboardComm=%@\n",[m_mPboardComm name]);
        NSRange rang= [[m_mPboardComm name] rangeOfString:[port name] options:NSForcedOrderingSearch];
        if (rang.location!= NSNotFound )
        {
            [[m_simpleComSetting getconnectStatusView] setState:0];
            [m_mPboardComm close] ;
            [m_mPboardComm release] ;
            m_mPboardComm=nil;
        }
    }

}

-(id)initwithName:(SimpleComSetting*)simpleComSetting
{
    self = [super init];
    if (self) {
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(didRemovePorts:) name:AMSerialPortListDidRemovePortsNotification object:nil];
        m_UartLog=[[NSMutableString alloc]init];
        m_recive_dataInput=[[NSMutableData alloc] init];
        m_simpleComSetting=nil;
        m_mPboardComm=nil;
         m_simpleComSetting=[simpleComSetting retain];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(SaveLogNotification:) name:kNotificationSaveLog object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(EmptyLogNotification:) name:kNotificationOnEngineStart object:nil];
    }
    return self;
}

-(BOOL)CheckResourceName:(NSString*)Resource
{
    if (m_simpleComSetting) {
        NSString * strSetting=[m_simpleComSetting getSetting];
        NSRange rang=[strSetting rangeOfString:Resource options:NSForcedOrderingSearch];
        return rang.location!=NSNotFound;
    }
    return NO;
}

-(void)SetSimpleComSetting:(SimpleComSetting*)simpleComSetting
{
    SimpleComSetting *old=m_simpleComSetting;
    m_simpleComSetting=[simpleComSetting retain];
    [old release];
    return;
}

-(SimpleComSetting*)GetSimpleComSetting
{
    return m_simpleComSetting;
}

-(BOOL)isOpen
{
    if (m_mPboardComm)
    {
        return [m_mPboardComm isOpen];
    }
    return NO;
}

-(BOOL)OpenSimpleCom
{
    if (m_mPboardComm!=nil) {
        [m_mPboardComm close] ;
        [m_mPboardComm release] ;
        m_mPboardComm=nil;
    }
    
    NSString * strSetting=[m_simpleComSetting getSetting];
    NSArray * arrSetting=[strSetting componentsSeparatedByString:@";"];
    NSString* strDUTAddr=[arrSetting objectAtIndex:0];
    if (strDUTAddr==nil || strDUTAddr.length==0)
        return NO;
    NSString* strDUTSetting=[arrSetting objectAtIndex:1];
    if (strDUTSetting==nil || strDUTSetting.length==0)
        return NO;
    SerialportEx *serialport = [[[SerialportEx alloc] init:strDUTAddr withName
                                         :strDUTAddr type
                                         :(NSString*)CFSTR(kIOSerialBSDModemType)] autorelease] ;
    [serialport AttachReceiveMode:NO];
    NSFileHandle *handle = [serialport open] ;
    if (!handle)
    {
        [[m_simpleComSetting getconnectStatusView] setState:0];
        [serialport close];
        [serialport release];
         serialport=nil;
        NSAlert *alert = [[[NSAlert alloc] init] autorelease];
        [alert setMessageText:[NSString stringWithUTF8String:"Create interface"]];
        [alert setInformativeText:[NSString stringWithFormat:@"Device Open Fail with name:\r%@",strDUTAddr]];
        
        [alert setAlertStyle:NSWarningAlertStyle];
        [alert runModal];
        return NO;
        
//        NSRunAlertPanel(@"DUT Device",@"%@",@"Prompt", nil, nil,[NSString stringWithFormat:@"Device Open Fail with name:\r%@",strDUTAddr]) ;
    }
    [m_mPboardComm SetDetectString:@"\r\n"];
    m_mPboardComm=[serialport retain];
    [m_mPboardComm PortSetting:strDUTSetting] ;
   [m_mPboardComm SetCallback:@selector(SimplecomSaveUartLog:) Target:self];
//    [m_mPboardComm AttachView:[m_simpleComSetting getLogView]];
    [[m_simpleComSetting getconnectStatusView] setState:1];
    return [m_mPboardComm isOpen];
}

-(void)closeSimpleCom
{
    if (m_mPboardComm)
    {
        [m_mPboardComm close];
        [m_mPboardComm release];
        m_mPboardComm=nil;
    }
}

-(void)SimplecomSaveUartLog:(id)ReadData
{
   
   NSString *StrTemp=[[NSString alloc] initWithData:ReadData encoding:NSASCIIStringEncoding];

    NSString *Str = [NSString stringWithFormat:@"recv :%@",StrTemp];
    [StrTemp release];
    [self performSelectorOnMainThread:@selector(logOut:) withObject:[NSDictionary dictionaryWithObjectsAndKeys:Str,@"Message",[m_simpleComSetting getLogView],@"TextView",nil] waitUntilDone:YES];
}

-(void)EmptyLogNotification:(NSNotification *)nf
{
    SerialPortView *mTVOutput=[m_simpleComSetting getLogView];
    [m_UartLog setString:@""];
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
//    BOOL b =  [[[m_simpleComSetting getLogView] string] writeToFile:filepath atomically:YES encoding:NSUTF8StringEncoding error:&err];
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

-(bool)WriteString:(const char *)szCMD
{
    if ([m_simpleComSetting isEcho])
    {
        NSString* Str = [NSString stringWithCString:szCMD encoding:NSUTF8StringEncoding];
        Str = [NSString stringWithFormat:@"send :%@",Str];
        //[[[[m_simpleComSetting getLogView] textStorage] mutableString] appendFormat:@"send :%@",[NSString stringWithCString:szCMD encoding:NSUTF8StringEncoding]];
        [self performSelectorOnMainThread:@selector(logOut:) withObject:[NSDictionary dictionaryWithObjectsAndKeys:Str,@"Message",[m_simpleComSetting getLogView],@"TextView",nil] waitUntilDone:YES];
    }
	return [m_mPboardComm WriteStringValue:[NSString stringWithCString:szCMD encoding:NSASCIIStringEncoding]];
}

-(void)AddLabelString:(const char *)LabelStr
{
    NSString* Str = [NSString stringWithUTF8String:LabelStr];
    [self performSelectorOnMainThread:@selector(logOut:) withObject:[NSDictionary dictionaryWithObjectsAndKeys:Str,@"Message",[m_simpleComSetting getLogView],@"TextView",nil] waitUntilDone:YES];
}
-(const char *)ReadStringRegex:(const char*)Regex Timeout:(int)timeout
{
    NSString *rtnStr=[m_mPboardComm ReadStringRegex:[NSString stringWithUTF8String:Regex] Timeout:timeout];
    return rtnStr.UTF8String;
}
-(const char*) ReadString
{
    NSData *data=[m_mPboardComm GetInBuffer];
    //return (const char*)[data bytes];
    NSString *rtnStr = [[[NSString alloc]initWithData:data encoding:NSASCIIStringEncoding] autorelease];
    return rtnStr.UTF8String;
}

-(int)SetDetectString:(const char*)regex;
{
    return [m_mPboardComm SetDetectString:[NSString stringWithCString:regex encoding:NSASCIIStringEncoding]];
}
-(int) WaitForString:(int) iTimeout
{
    return [m_mPboardComm WaitDetect:iTimeout];
}
-(void) ClearBuffer
{
    [m_mPboardComm ClearInBuffer];
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
@end
