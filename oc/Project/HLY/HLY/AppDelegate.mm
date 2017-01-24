//
//  CommInterface.m
//  UIExternal
//
//  Created by Liang on 15-8-31.
//  Copyright (c) 2015年 ___Intelligent Automation___. All rights reserved.
//

#import "AppDelegate.h"
#define KEY_SPHLY       @"spHLY"
#define KEY_SETTING_HLY @"settingHLY"
#import "HLYConfigWinDelegate.h"
#define __module_debug__ 1

CFixture * pFixture = NULL;
HLYController* g_FixtureController;
@implementation HLYController
@synthesize dicConfiguration,strCfgFile;

-(id)init
{
    self = [super init];
    if (self) {
        pFixture = new CFixture();
        m_pSerialPort = nil;
        dac1 = [[NSMutableString alloc] init];
        m_handle = nil;
        dicConfiguration = [NSMutableDictionary new];
        m_FixtureID = [[NSMutableString alloc] init];
        m_FixtureVer = [[NSMutableString alloc] init];
        
        NSString * contentPath = [[NSBundle bundleForClass:[self class]]executablePath];
        contentPath = [[contentPath stringByDeletingLastPathComponent]stringByDeletingLastPathComponent];
        self.strCfgFile = [contentPath stringByAppendingPathComponent:@"/Config/GT_Auto_HLY_Config.plist"];
        [[NSFileManager defaultManager] createDirectoryAtPath:[self.strCfgFile stringByDeletingLastPathComponent] withIntermediateDirectories:YES attributes:nil error:nil];
        
        g_FixtureController = self;
    }
    return self;
}

-(void)dealloc
{
    if (m_pSerialPort) {
        [m_pSerialPort close];
    }
    [m_FixtureID release];
    [m_FixtureVer release];
    [dicConfiguration release];
    [super dealloc];
}

-(void)reset
{
    
}

-(int)LoadConfig:(NSString *)strfile
{
    NSFileManager * fm = [NSFileManager defaultManager];
    if (![fm fileExistsAtPath:strfile]) {
        [dicConfiguration setValue:@"/dev/cu.usbserial-BUCK" forKey:KEY_SPHLY];
        
        [dicConfiguration setValue:@"115200,None,8,1" forKey:KEY_SETTING_HLY];
        
        HLYConfigWinDelegate * config = (HLYConfigWinDelegate *)[winHLYConfig delegate];
        [config InitialCtls:dicConfiguration];
    }
    else
    {
        [dicConfiguration setValuesForKeysWithDictionary:[NSDictionary dictionaryWithContentsOfFile:strfile]];
    }
    [self serialPortInitial];
    return 0;
}

-(void)awakeFromNib
{
    [self LoadConfig:strCfgFile];
    [_HLYView setUsesFindBar:YES];
    filepath = @"/vault/Intelli_log/HLY_log/HLY_log.txt";
}

-(IBAction)btHLYConfig:(id)sender
{
    [NSApp beginSheet:winHLYConfig modalForWindow:winHLYMain modalDelegate:self didEndSelector:@selector(sheetDidEnd:returnCode:contextInfo:) contextInfo:nil];
    return;
    
    [winHLYConfig center];
    if ([NSApp runModalForWindow:winHLYConfig])    //Click On OK
    {
          [self serialPortInitial];
    }
}


-(void)windowWillBeginSheet:(NSNotification *)notification
{
    HLYConfigWinDelegate * config = (HLYConfigWinDelegate *)[winHLYConfig delegate];
    [config InitialCtls:dicConfiguration];
}

-(void)sheetDidEnd:(NSWindow *)sheet returnCode:(int)returnCode contextInfo:(void *)contextInfo
{
    if (returnCode)
    {
        HLYConfigWinDelegate * config = (HLYConfigWinDelegate *)[winHLYConfig delegate];
        [config InitialCtls:dicConfiguration];
        [self serialPortInitial];
    }
}


-(int)serialPortInitial
{
    [m_pSerialPort close];
    NSString * strPort = [dicConfiguration valueForKey:KEY_SPHLY];
    m_pSerialPort = [[[SerialportEx alloc] init:strPort withName:strPort type:(NSString*)CFSTR(kIOSerialBSDModemType)]autorelease];
    
    m_handle = [m_pSerialPort open];;
    if (!m_handle)
    {
        NSString * msg = [NSString stringWithFormat:@"Couldn't open specified serial port with name:\r\n %@",strPort];
        [self ShowMsg:msg];
    }
    
    [m_pSerialPort PortSetting:[dicConfiguration valueForKey:KEY_SETTING_HLY]];
//    [m_pSerialPort AttachView:_HLYView];            //附加UI到串口上，可自动接受串口返回
//    [_HLYView AttachSerialPort:m_pSerialPort];      //UI附加串口，
    [m_pSerialPort SetCallback:@selector(SerialPortCallback:) Target:self];
    [m_pSerialPort SetDetectString:@"\r\n"];
    
    pFixture->AttachSerialPort(m_pSerialPort);
    return 0;
}


-(void)SerialPortCallback:(id)ReadData
{
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    NSString * str = [[NSString alloc]initWithData:ReadData encoding:NSASCIIStringEncoding];
    
    [pool drain];
    
    if (str) {
        [str release];
        str = nil;
    }
}



-(void)SetCallback:(SEL)Callback Target:(id)object
{
    m_TestManagerCallBack = Callback;
    m_CallbackTarget = object;
}


-(void)ShowMsg:(NSString*)msg
{
    [self performSelectorOnMainThread:@selector(fnErrMsg:) withObject:msg waitUntilDone:NO];
}

-(void)fnErrMsg:(id)par
{
    NSRunAlertPanel(@"Fixture Controlller Information", @"%@", @"OK", nil, nil, par);
}

-(BOOL)windowShouldClose:(id)sender
{
    [winHLYMain orderOut:nil];
    return NO;
}

-(void)showPanel:(id)sender
{
    [winHLYMain center];
    [winHLYMain makeKeyAndOrderFront:sender];
}


-(void)logOut:(NSString*)msg
{
    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
    [dateFormatter setDateFormat:@"yyyy/MM/dd HH:mm:ss.SSS : "];
    int length = 0;
    NSAttributedString *theString;
    NSRange theRange;
    
    NSString * str = [NSString stringWithFormat:@"%@ %@\r",[dateFormatter stringFromDate:[NSDate date]],msg];
    theString = [[NSAttributedString alloc] initWithString:str];
    [[_HLYView textStorage] appendAttributedString: theString];
    length = (int)[[_HLYView textStorage] length];
    theRange = NSMakeRange(length, 0);
    [_HLYView scrollRangeToVisible:theRange];
    [dateFormatter release];
    [theString release];
    [_HLYView setNeedsDisplay:YES];
}

-(void)printLog:(NSString*)strMsg
{
    [self performSelectorOnMainThread:@selector(logOut:) withObject:strMsg waitUntilDone:YES];
}

-(void)clearallview
{
    [_HLYView setString:@""];

}

- (BOOL)isPureFloat:(NSString*)string
{
    NSScanner* scan = [NSScanner scannerWithString:string];
    float val;
    return[scan scanFloat:&val] && [scan isAtEnd];
}

//-(IBAction)HLYREAD:(id)sender
//{
//    [m_pSerialPort SetDetectString:@"@_@"];
//    NSString *strCmd = @"@READ_I?";
//    [m_pSerialPort WriteStringValue:strCmd];
//    [m_pSerialPort WaitDetect] ;
//    
//    NSString *HLYread = [m_pSerialPort ReadStringValue];
//    NSLog(@"HLY READ %@ \r\n",HLYread);
//    
//    if ([HLYread  isEqual: @"ERROR,CMD is ERROR"]) {
//        [self ShowMsg:HLYread];
//    }
//    else
//    {
//        [self ShowMsg:@"Read from HLY is empty!"];
//    }
//}

-(IBAction)DACSET1:(id)sender
{
    //read dac value
    if ([HLY_DAC1 stringValue]) {
        [dac1 setString: HLY_DAC1.stringValue];
        if([self isPureFloat:dac1])
        {
            NSString *DAC1 = [NSString stringWithFormat:@"@SET_DAC=%@",dac1];
            [m_pSerialPort WriteStringValue:DAC1];
        }
        else
        {
            [self ShowMsg:@"Please set dac a float number"];
        }
    }
    else
    {
        [self ShowMsg:@"Please set dac param1"];
    }
    
    //read IA value
    
    //read back ia value
    [m_pSerialPort SetDetectString:@"@_@"];
    NSString *strCmd = @"@READ_I?";
    [m_pSerialPort WriteStringValue:strCmd];
    [m_pSerialPort WaitDetect] ;
    
    NSString *HLYread1 = [m_pSerialPort ReadStringValue];
    NSLog(@"HLY READ %@ \r\n",HLYread1);
    
    if ([HLYread1  containsString: @"ERROR,CMD is ERROR"]) {
        [self ShowMsg:HLYread1];
    }
    else
    {
        [self ShowMsg:@"Read from HLY is empty!"];
    }
    NSString * DATA1 = [NSString stringWithFormat:@"DATA 1: DAC1=%@ ,IA1=%@ ,BACK1=%@",dac1,ia1,back1];
    [HLY_VALUE setString:DATA1];
}

-(IBAction)DACSET2:(id)sender
{
    if ([HLY_DAC2 stringValue]) {
        [dac2 setString: HLY_DAC2.stringValue];
        if([self isPureFloat:dac2])
        {
            NSString *DAC2 = [NSString stringWithFormat:@"@SET_DAC=%@",dac2];
            [m_pSerialPort WriteStringValue:DAC2];
        }
        else
        {
            [self ShowMsg:@"Please set dac a float number"];
        }
    }
    else
    {
        [self ShowMsg:@"Please set dac param2"];
    }
    
    //read IA value
    
    //read back ia value
    [m_pSerialPort SetDetectString:@"@_@"];
    NSString *strCmd = @"@READ_I?";
    [m_pSerialPort WriteStringValue:strCmd];
    [m_pSerialPort WaitDetect] ;
    
    NSString *HLYread2 = [m_pSerialPort ReadStringValue];
    NSLog(@"HLY READ %@ \r\n",HLYread2);
    
    if ([HLYread2  containsString: @"ERROR,CMD is ERROR"]) {
        [self ShowMsg:HLYread2];
    }
    else
    {
        [self ShowMsg:@"Read from HLY is empty!"];
    }
    NSString * DATA2 = [NSString stringWithFormat:@"DATA 2: DAC2=%@ ,IA2=%@ ,BACK2=%@",dac2,ia2,back2];
    [HLY_VALUE setString:DATA2];
}

//-(void)fatormath:(NSArray *)paramtable
//{
//    if([paramtable count]=4){
//    }
//}

-(IBAction)IAREAD:(id)sender
{
    if(dac1,dac2,ia1,ia2){
        float x1 = [dac1 floatValue];
        float x2 = [dac2 floatValue];
        float y1 = [ia1 floatValue];
        float y2 = [ia2 floatValue];
//        float x1 = 2.0;
//        float x2 = 3.0;
//        float y1 = 2.1;
//        float y2 = 3.1;
//        NSLog(@"参数为：%f，%f，%f，%f",x1,x2,y1,y2);
        float a1,b1;
        a1 = (x1-x2)/(y1-y2);
        b1 = y1-(x1*a1);
        NSLog(@"a1=%f,b1=%f",a1,b1);
        NSString * A1 = [NSString stringWithFormat:@"%f",a1];
        NSString * B1 = [NSString stringWithFormat:@"%f",b1];
        [HLY_ISETHE setString:B1];
        [HLY_ISETHK setString:A1];
    }
}


-(IBAction)BACKREAD:(id)sender
{
    if (ia1,ia2,back1,back2) {
        float z1 = [dac1 floatValue];
        float z2 = [dac2 floatValue];
        float y1 = [ia1 floatValue];
        float y2 = [ia2 floatValue];
        float a2,b2;
        a2 = (z1-z2)/(y1-y2);
        b2 = y2-(z1*a2);
        NSString * A2 = [NSString stringWithFormat:@"%f",a2];
        NSString * B2 = [NSString stringWithFormat:@"%f",b2];
        [HLY_IREADHE setString:B2];
        [HLY_IREADHK setString:A2];
    }
}

//-(BOOL)Searchsn
//{
//    m_Readdata = [NSString stringWithContentsOfFile:filepath encoding:NSUTF8StringEncoding error:nil];
//    NSMutableString *searchstr = [[NSMutableString alloc] init];         //创建可变字符串local mstr读取文件内sn，并操作。
//    [searchstr setString:m_Readdata];
//    NSLog(@"文件内的string：%@",searchstr);
//    NSString *comparestr = [NSString stringWithFormat:@"SN:%@;",HLYACTSN];
//    
//    if(searchstr)
//    {
//        BOOL searchret = [searchstr containsString:comparestr];
//
//        if (searchret) {
//            [HLYResult setString:@"FAIL"];
//            [HLYResult setBackgroundColor:[NSColor redColor]];
//            return true;
//        }
//        else{
//            [HLYResult setString:@"PASS"];
//            [HLYResult setBackgroundColor:[NSColor greenColor]];
//            return false;
//        }
//    }
//    return true;
//}

@end
