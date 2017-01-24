//
//  CommInterface.m
//  UIExternal
//
//  Created by Liang on 15-8-31.
//  Copyright (c) 2015年 ___Intelligent Automation___. All rights reserved.
//
#import <Foundation/Foundation.h>
#import "AppDelegate.h"
#define KEY_SPIIC       @"spIIC"
#define KEY_SETTING_IIC @"settingIIC"
#import "IICConfigWinDelegate.h"
#define __module_debug__ 1
int iAuthor = 0;

CFixture * pFixture = NULL;
IICController* g_FixtureController;
@implementation IICController
@synthesize dicConfiguration,strCfgFile,FailItem,DynamicCheck,MFlagCheck;

NSArray * ItemArr=[[NSArray alloc] initWithObjects:
                   @"Screw_Usage_Counts",
                   @"Left_Arm_Usage_Counts",
                   @"Right_Arm_Usage_Counts",
                   @"Top_Probe_Plate_Usage_Counts",
                   @"Left_Block_Usage_Counts",
                   @"Right_Block_Usage_Counts",
                   @"Spring_Usage_Counts",
                   @"Screw_Nut_Usage_Counts",
                   @"Buck_Probe_Usage_Counts",
                   @"Buck_Probe_Counts",
                   @"Maintenance_DateCode_Time",
                   @"VMT_Test_Counts",
                   @"VMT_Last_Module_Start_Time",
                   @"Module_Load_Time",
                   @"VMT_Last_Module_End_Time",
                   @"VMT_Pass_Counts",
                   @"VMT_Fail_Counts",
                   @"TRAP3_Test_Counts",
                   @"TRAP3_Last_Module_Start_Time",
                   @"TRAP3_Last_Module_End_Time",
                   @"TRAP3_Pass_Counts",
                   @"TRAP3_Fail_Counts",
                   @"Load_Station_Test_Counts",
                   @"Load_Station_Last_Module_Time",
                   @"Repair_Counts",
                   @"Repair_Time",
                   @"Usage_Time_Sinece_Last_PM",
                   @"Maintenance_Flag",
                   @"BUCK_Failure_Code",nil];
NSArray * LimitArr=[[NSArray alloc] initWithObjects :
                    @"8000",
                    @"8000",
                    @"8000",
                    @"15000",
                    @"15000",
                    @"15000",
                    @"15000",
                    @"20000",
                    @"20000",
                    @"20000",
                    @"Nil",
                    @"Nil",
                    @"Nil",
                    @"Nil",
                    @"Nil",
                    @"Nil",
                    @"Nil",
                    @"Nil",
                    @"Nil",
                    @"Nil",
                    @"Nil",
                    @"Nil",
                    @"Nil",
                    @"Nil",
                    @"Nil",
                    @"Nil",
                    @"Nil",
                    @"Nil",
                    @"Nil",nil];

// read address: @"RE 0015 1"; "1" means 1 byte adress; "4" meas 4 bytes adress from 0015 to 0018;
// write adress: @WE 0015 0"; "0" means set the data to "0" for the adress of 0015 to 0018; "1" means set to 1;

-(id)init
{
    self = [super init];
    if (self) {
        pFixture = new CFixture();
        m_pSerialPort = nil;
        m_Readdata = [[NSMutableString alloc] init];
//        IICACTSN = [[NSMutableString alloc] init];
        m_handle = nil;
        dicConfiguration = [NSMutableDictionary new];
        m_FixtureID = [[NSMutableString alloc] init];
        m_FixtureVer = [[NSMutableString alloc] init];
        
        [[NSMutableArray alloc] init];
        
        NSString * contentPath = [[NSBundle bundleForClass:[self class]]executablePath];
        contentPath = [[contentPath stringByDeletingLastPathComponent]stringByDeletingLastPathComponent];
        self.strCfgFile = [contentPath stringByAppendingPathComponent:@"/Config/GT_Auto_IIC_Config.plist"];
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

-(void)awakeFromNib
{
    [IICRESET setEnabled:false];
    [self LoadConfig:strCfgFile];
    [_IICView setUsesFindBar:YES];
    filepath = @"/vault/Intelli_log/i2c_log/Total of 15pcs and Mag unscrew data.txt";
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

-(void)windowWillBeginSheet:(NSNotification *)notification
{
    IICConfigWinDelegate * config = (IICConfigWinDelegate *)[winIICConfig delegate];
    [config InitialCtls:dicConfiguration];
}

-(void)sheetDidEnd:(NSWindow *)sheet returnCode:(int)returnCode contextInfo:(void *)contextInfo
{
    if (returnCode)
    {
        IICConfigWinDelegate * config = (IICConfigWinDelegate *)[winIICConfig delegate];
        [config InitialCtls:dicConfiguration];
        [self serialPortInitial];
    }
}

-(void)ShowMsg:(NSString*)msg
{
    [self performSelectorOnMainThread:@selector(fnErrMsg:) withObject:msg waitUntilDone:NO];
}

-(void)fnErrMsg:(id)par
{
    NSRunAlertPanel(@"Fixture Controlller Information", @"%@", @"OK", nil, nil, par);
}

-(void)showPanel:(id)sender
{
    [winIICMain center];
    [winIICMain makeKeyAndOrderFront:sender];
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
    [[_IICView textStorage] appendAttributedString: theString];
    length = (int)[[_IICView textStorage] length];
    theRange = NSMakeRange(length, 0);
    [_IICView scrollRangeToVisible:theRange];
    [dateFormatter release];
    [theString release];
    [_IICView setNeedsDisplay:YES];
}

-(void)savelog:(NSString*) str
{
    NSFileManager *fm = [NSFileManager defaultManager];
    NSString *fileName = [NSString stringWithFormat:@"BUCK_log.txt"];
    NSString * fullPath;
    NSString *path;
    path = [fm currentDirectoryPath];
    fullPath=[path stringByAppendingString:fileName];
    if([fm fileExistsAtPath:fullPath] == NO)
    {
        [fm createFileAtPath:fullPath contents: [str dataUsingEncoding: NSUTF8StringEncoding] attributes:nil];
    }
    else
    {
        NSFileHandle* fh=[NSFileHandle fileHandleForWritingAtPath:fullPath];
        [fh seekToEndOfFile];
        [fh writeData:[str dataUsingEncoding:NSUTF8StringEncoding]];
    }
}

//-(void)savelog
//{
//    fileName = [NSString stringWithFormat:@"IIC_log_txt"];
//    NSFileHandle *outFile,*inFile;
//    NSString * fullPath;
//    NSString *path;
//    path = [[NSFileManager defaultManager] currentDirectoryPath];
//    inFile = [NSFileHandle fileHandleForReadingAtPath:fileName];
//    if (inFile == nil) {
//        NSLog(@"File %@ not exist.Creat it!\r",fileName);
//        [[NSFileManager defaultManager] createFileAtPath:fileName contents:nil attributes:nil];
//    }
//    fullPath = [path stringByAppendingPathComponent:fileName];
//
//    NSData *bufferiic,*bufferdata,*bufferresult;
//    outFile = [NSFileHandle fileHandleForWritingAtPath:fullPath];
//    if (outFile == nil) {
//        NSLog(@"Open of %@ for writing failed.",fullPath);
//    }
//    [outFile seekToEndOfFile];
//
//    bufferiic = [[_IICView string] dataUsingEncoding:NSUTF8StringEncoding];
//    [outFile writeData:bufferiic];
//    bufferdata = [[IICData string] dataUsingEncoding:NSUTF8StringEncoding];
//    [outFile writeData:bufferdata];
//    bufferresult = [[IICResult string] dataUsingEncoding:NSUTF8StringEncoding];
//    [outFile writeData:bufferresult];
//
//    [inFile closeFile];
//    [outFile closeFile];
//
//    [self clearallview];
//
//}

-(void)WriteAdress:(NSString*)adress :(NSString* ) data
{
    [NSThread sleepForTimeInterval:0.5];
    NSString *cmd = [NSString stringWithFormat:@"WE %@ %@\r",adress,data];
    [m_pSerialPort WriteStringValue:cmd];
    //    [m_pSerialPort WaitDetect] ;
    //    NSLog(@"write return:%@",[m_pSerialPort ReadStringValue]);
}

-(void)printLog:(NSString*)strMsg
{
    [self performSelectorOnMainThread:@selector(logOut:) withObject:strMsg waitUntilDone:YES];
}

-(void)clearallview
{
    [_IICView setString:@""];
    //    [IICData setString:@""];
    //    [IICResult setString:@""];
}

-(int)LoadConfig:(NSString *)strfile
{
    NSFileManager * fm = [NSFileManager defaultManager];
    if (![fm fileExistsAtPath:strfile]) {
        [dicConfiguration setValue:@"/dev/cu.usbserial-PCIE" forKey:KEY_SPIIC];
        
        [dicConfiguration setValue:@"115200,None,8,1" forKey:KEY_SETTING_IIC];
        
        IICConfigWinDelegate * config = (IICConfigWinDelegate *)[winIICConfig delegate];
        [config InitialCtls:dicConfiguration];
    }
    else
    {
        [dicConfiguration setValuesForKeysWithDictionary:[NSDictionary dictionaryWithContentsOfFile:strfile]];
    }
    [self serialPortInitial];
    
    DynamicCheck=false;
    MFlagCheck=false;
    return 0;
}

-(int)serialPortInitial
{
    [m_pSerialPort close];
    NSString * strPort = [dicConfiguration valueForKey:KEY_SPIIC];
    m_pSerialPort = [[[SerialportEx alloc] init:strPort withName:strPort type:(NSString*)CFSTR(kIOSerialBSDModemType)]autorelease];
    
    m_handle = [m_pSerialPort open];;
    if (!m_handle)
    {
        NSString * msg = [NSString stringWithFormat:@"Couldn't open specified serial port with name:\r\n %@",strPort];
        [self ShowMsg:msg];
    }
    
    [m_pSerialPort PortSetting:[dicConfiguration valueForKey:KEY_SETTING_IIC]];
//    [m_pSerialPort AttachView:_IICView];            //附加UI到串口上，可自动接受串口返回
//    [_IICView AttachSerialPort:m_pSerialPort];      //UI附加串口，
    [m_pSerialPort SetCallback:@selector(SerialPortCallback:) Target:self];
    [m_pSerialPort SetDetectString:@"\r\n"];
    
    pFixture->AttachSerialPort(m_pSerialPort);
    return 0;
}

-(BOOL)windowShouldClose:(id)sender
{
    [winIICMain orderOut:nil];
    return NO;
}

-(NSString *)getBinaryByhex:(NSString *)hex
{
    NSMutableDictionary  *hexDic = [[NSMutableDictionary alloc] init];
    hexDic = [[NSMutableDictionary alloc] initWithCapacity:16];
    [hexDic setObject:@"0000" forKey:@"0"];
    [hexDic setObject:@"0001" forKey:@"1"];
    [hexDic setObject:@"0010" forKey:@"2"];
    [hexDic setObject:@"0011" forKey:@"3"];
    [hexDic setObject:@"0100" forKey:@"4"];
    [hexDic setObject:@"0101" forKey:@"5"];
    [hexDic setObject:@"0110" forKey:@"6"];
    [hexDic setObject:@"0111" forKey:@"7"];
    [hexDic setObject:@"1000" forKey:@"8"];
    [hexDic setObject:@"1001" forKey:@"9"];
    [hexDic setObject:@"1010" forKey:@"A"];
    [hexDic setObject:@"1011" forKey:@"B"];
    [hexDic setObject:@"1100" forKey:@"C"];
    [hexDic setObject:@"1101" forKey:@"D"];
    [hexDic setObject:@"1110" forKey:@"E"];
    [hexDic setObject:@"1111" forKey:@"F"];
    [hexDic setObject:@"1111" forKey:@"f"];
    [hexDic setObject:@"1110" forKey:@"e"];
    [hexDic setObject:@"1101" forKey:@"d"];
    [hexDic setObject:@"1100" forKey:@"c"];
    [hexDic setObject:@"1011" forKey:@"b"];
    [hexDic setObject:@"1010" forKey:@"a"];

    
    NSMutableString *binaryString=[[NSMutableString alloc] init];
    for (int i=0; i<[hex length]; i++) {
        NSRange rage;
        rage.length = 1;
        rage.location = i;
        NSString *key = [hex substringWithRange:rage];
        //NSLog(@"%@",[NSString stringWithFormat:@"%@",[hexDic objectForKey:key]]);
        binaryString = [NSString stringWithFormat:@"%@%@",binaryString,[NSString stringWithFormat:@"%@",[hexDic objectForKey:key]]];
    }
    NSLog(@"转化后的二进制为:%@",binaryString);
    return binaryString;
}


-(BOOL)Searchsn:(NSString*)str //search sn from file
{
    m_Readdata = [NSString stringWithContentsOfFile:filepath encoding:NSUTF8StringEncoding error:nil];
    NSMutableString *searchstr = [[NSMutableString alloc] init];         //创建可变字符串local mstr读取文件内sn，并操作。
    [searchstr setString:m_Readdata];
//    NSLog(@"文件内的string：%@",searchstr);
    NSArray *linestr = [searchstr componentsSeparatedByString:@"\r\n"];
    NSLog(@"文件内的string：%@",linestr);

    NSString *comparestr = [NSString stringWithFormat:@"%@",str];
    
    if(linestr)
    {
        for(int i = [linestr count]-1;i>-1; i--)
        {
            
            BOOL searchret = [linestr[i] containsString:comparestr];
            if (searchret)
            {
                BOOL searchpass = [linestr[i] containsString:@"PASS"];
                if (searchpass)
                {
                    //              [IICResult setString:@"FAIL"];
                    //              [IICResult setBackgroundColor:[NSColor redColor]];
                    return true;
                }
                else
                {
                    [self ShowMsg:@"FAIL !\r\nThe SN test FAIL"];
                    return false;
                }
            }
            else
            {
                [self ShowMsg:@"FAIL !\r\nCouldn't find SN!"];
                return false;
            }
        }
    }
    [self ShowMsg:@"Please confirm share folder right!"];
    return false;
}

-(void) OutPutLog:(NSString*)str
{
    NSArray * RetArry=[str componentsSeparatedByString:@" "];
    NSMutableArray * dealarry= [[NSMutableArray alloc] init];

    int j=[RetArry count]/4;
    for(int i=0; i<j; i++)
    {
        NSMutableString * tmp = [NSMutableString string];
        for(int k=3; k>=0; k--)
            [tmp appendString:[RetArry objectAtIndex:i*4+k]];
        [dealarry addObject:tmp ];
    }
//    NSLog(@"!!!!\r\n%@",dealarry);
    
    for(int i=0; i<[ItemArr count];i++)
    {
        unsigned long ret=strtoul([dealarry[i] UTF8String], 0, 16);
        unsigned long retL=[LimitArr[i] intValue];

        NSMutableString * ret1= [[[[[ItemArr[i]  stringByAppendingString:@"      "] stringByAppendingString: [NSString stringWithFormat:@"%lu",ret]] stringByAppendingString:@"      "] stringByAppendingString: LimitArr[i]] stringByAppendingString:@"\r\n"];

        if(ret>=retL && i<10)
        {
            NSMutableAttributedString *str = [[NSMutableAttributedString alloc]initWithString:ret1];
            //设置：内容显示成红色
            [str addAttribute:NSForegroundColorAttributeName value:[NSColor redColor] range:NSMakeRange(0, str.length)];
            [_IICView.textStorage appendAttributedString:str];
            continue;
        }
        NSMutableAttributedString *strxxxx = [[NSMutableAttributedString alloc]initWithString:ret1];
        [_IICView.textStorage appendAttributedString:strxxxx];
    }
}



-(NSString *)BUCKREAD
{
    [m_pSerialPort SetDetectString:@"@_@"];
    NSString *strCmd = @"BUCK\r";
    [m_pSerialPort WriteStringValue:strCmd];
    [m_pSerialPort WaitDetect] ;
    NSString *iicread = [m_pSerialPort ReadStringValue];
    NSLog(@"iicread %@ \r\n",iicread);
    return iicread;
}

-(NSString*)ReadBuckSN:(NSString*)str
{
    //realsn = mode + version + gys + sn
    //          5E     0400      0   0206
    
    /////////////////////////mode 0x04-0x05
    //1-5E;2-5F;3-5G;5-6E;6-6F;7-6G;8-6I
    NSString *IICMode = [str substringWithRange:NSMakeRange(28, 3)];
    long iicmode = strtoul([[IICMode substringWithRange:NSMakeRange(0, 2)] UTF8String],0,16);
    NSString *mode = @"";
    if (iicmode == 1) {mode = @"5E";}
    else if (iicmode == 2) {mode = @"5F";}
    else if (iicmode == 3) {mode = @"5G";}
    else if (iicmode == 4) {mode = @"6E";}
    else if (iicmode == 6) {mode = @"6F";}
    else if (iicmode == 7) {mode = @"6G";}
    else if (iicmode == 8) {mode = @"6I";}
    else {mode = @"0";}
//    NSLog(@"iicmode : %ld ;mode=%@",iicmode,mode);
    
    //version 0x00-0x03
    NSString *BuckVersion = [str substringWithRange:NSMakeRange(15, 14)];
    NSString *bv1 = [BuckVersion substringWithRange:NSMakeRange(1, 2)];
    NSString *bv2 = [BuckVersion substringWithRange:NSMakeRange(4, 2)];
    NSString *bv3 = [BuckVersion substringWithRange:NSMakeRange(7, 2)];
    NSString *bv4 = [BuckVersion substringWithRange:NSMakeRange(10, 2)];
    NSString *bv5 = [NSString stringWithFormat:@"%@%@%@%@",bv4,bv3,bv2,bv1];
    const char *buckver1 = [bv5 UTF8String];
    NSString * buckver2= [NSString stringWithFormat:@"%s",buckver1];
    unsigned int fix_data;
    NSScanner * nsscan = [[NSScanner alloc]initWithString: buckver2];
    [nsscan scanHexInt:&fix_data];
    Float32 val;
    memcpy(&val, &fix_data, 4);
    
    Float32 val1 = val*100;
    NSString *buckversion = [NSString stringWithFormat:@"%0.0f",val1];
    if (buckversion.length != 4){
        buckversion = [NSString stringWithFormat:@"0%@",buckversion];
    }
//    NSLog(@"buckversion %@",buckversion);

    //0x10供应商编号代码
    NSString *gsy = [str substringWithRange:NSMakeRange(64, 2)];
    if ([gsy isEqual: @"14"])
    {
        gsy = @"Q";
    }
    else
    {
        gsy = [[str substringWithRange:NSMakeRange(65, 1)] uppercaseString];
    }
//    NSLog(@"gsy=%@",gsy);

    //0x08-0x09 sn
    NSString *IICSN = [str substringWithRange:NSMakeRange(39, 7)];
    NSString *iicsn1 = [IICSN substringWithRange:NSMakeRange(4, 2)];
    NSString *iicsn2 = [IICSN substringWithRange:NSMakeRange(1, 2)];
    NSString *iicsn3 = [NSString stringWithFormat:@"%@%@",iicsn1,iicsn2];
//    NSLog(@"ReadBuckSN;iicsn=%@;sn1=%@;sn2=%@;sn3=%@",IICSN,iicsn1,iicsn2,iicsn3);
//    long iicsn4 = strtoul([[iicsn3 substringWithRange:NSMakeRange(0, 4)] UTF8String], 0, 16);
//    NSNumber *longnumber = [NSNumber numberWithLong:iicsn4];
//    NSString* iicsn = [longnumber stringValue];
    
    NSString *realsn = [NSString stringWithFormat:@"%@%@%@%@",mode,buckversion,gsy,iicsn3];
    
    return realsn;
}

-(int)getdatesp:(NSString *)datecode
{
    
    //            NSString *yy = [datecode substringWithRange:NSMakeRange(10, 2)];
    //            long dateyy = strtoul([[yy substringWithRange:NSMakeRange(0, 2)] UTF8String], 0, 16);
    //            NSString *ww = [datecode substringWithRange:NSMakeRange(7, 2)];
    //            long dateww = strtoul([[ww substringWithRange:NSMakeRange(0, 2)] UTF8String], 0, 16);
    NSString *d2 = [datecode substringWithRange:NSMakeRange(4, 2)];
    NSString *d1 = [datecode substringWithRange:NSMakeRange(2, 1)];
    //            NSString *dd = [NSString stringWithFormat:@"%@%@",d1,d2];
    //            long datedd = strtoul([[dd substringWithRange:NSMakeRange(0, 4)] UTF8String], 0, 16);
    //            NSString *buckdate = [NSString stringWithFormat:@"%ld%ld%ld",dateyy,dateww,datedd];
    //            NSString *buckdate = [NSString stringWithFormat:@"%@%@%@%@",yy,ww,d2,d1];
    NSString *buckdate = [NSString stringWithFormat:@"%@%@",d2,d1];
    int date2 = [buckdate intValue];
    //            NSLog(@"\ry=%ld ; w=%ld ; d=%ld",dateyy,dateww,datedd);
    NSLog(@"\rdatecode=%@  ; buckdate=%@  ;  date2=%d",datecode,buckdate,date2);
    
    NSDate *date = [NSDate date];
    NSTimeZone *zone = [NSTimeZone systemTimeZone];
    NSInteger interval = [zone secondsFromGMTForDate: date];
    NSDate *datenow = [date  dateByAddingTimeInterval: interval];
    
    NSDateFormatter *format = [[NSDateFormatter alloc] init];
    //            format.dateFormat=@"YYwwDD";//W本月周；D本年日；h半日时；H本日时；E星期几；F一月中第几个星期几
    format.dateFormat=@"DD";
    NSString *dateString = [format stringFromDate:datenow];
    int date1 = [dateString intValue];
    int datesp = date1-date2;
    NSLog(@"\r now date=%@  ; datestring = %@  ; date1=%d  ; datesp =%d",datenow,dateString,date1,datesp);

    return datesp;
}

-(NSDate *)getnowdate
{
    NSDate *date = [NSDate date];
    NSTimeZone *zone = [NSTimeZone systemTimeZone];
    NSInteger interval = [zone secondsFromGMTForDate: date];
    NSDate *datenow = [date  dateByAddingTimeInterval: interval];
    return datenow;
}

-(NSString*)CheckFailurCode:(NSString*)str
{
    NSString *detailflagreset;
    NSString *failcode = [str substringWithRange:NSMakeRange(414,12)];
    NSLog(@"failcode %@ \r\n",failcode);
    NSString *f3 = [failcode substringWithRange:NSMakeRange(10, 2)];
    NSString *f4 = [failcode substringWithRange:NSMakeRange(7, 2)];
    NSString *failure = [NSString stringWithFormat:@"%@%@",f3,f4];
    NSLog(@"failure =%@",failure);
    NSString *detailflag = [self getBinaryByhex:failure];
    
    if(detailflag and [detailflag isEqualToString: @"0000000000000000"])
    {
        [self ShowMsg:@"BUCK Fail code is 0x0000\r\nNo Error Item be found\r\nPlease send to spare part room"];
    }
    else
    {
        NSUInteger counts = 0;
        NSString * string2 = @"1";
        NSMutableString *flagindex = [[NSMutableString alloc] init];
        for (int i = 0; i < detailflag.length - string2.length + 1; i++)
        {
            if ([[detailflag substringWithRange:NSMakeRange(i, string2.length)] isEqualToString:string2])
            {
                [NSThread sleepForTimeInterval:0.2];
                counts++;
//                if (i<4)
//                {
//                    [self ShowMsg:@"Wrong Failure Code!"];
//                }
                if (i==15){[flagindex insertString:@"BUCK yield bad\r" atIndex:0];}
                else if (i==14)
                {
                    [flagindex insertString:@"Screw\r" atIndex:0];
                    [m_pSerialPort WriteStringValue:@"WE 0015 00 00 00 00\r"];
                }
                else if (i==13)
                {
                    [flagindex insertString:@"Left arm\r" atIndex:0];
                    [m_pSerialPort WriteStringValue:@"WE 0019 00 00 00 00\r"];
                }
                else if (i==12)
                {
                    [flagindex insertString:@"Right arm\r" atIndex:0];
                    [m_pSerialPort WriteStringValue:@"WE 001D 00 00 00 00\r"];
                }
                else if (i==11)
                {
                    [flagindex insertString:@"Top Probe Plate\r" atIndex:0];
                    [m_pSerialPort WriteStringValue:@"WE 0021 00 00 00 00\r"];
                }
                else if (i==10)
                {
                    [flagindex insertString:@"Left block\r" atIndex:0];
                    [m_pSerialPort WriteStringValue:@"WE 0025 00 00 00 00\r"];
                }
                else if (i==9)
                {
                    [flagindex insertString:@"Right block\r" atIndex:0];
                    [m_pSerialPort WriteStringValue:@"WE 0029 00 00 00 00\r"];
                }
                else if (i==8)
                {
                    [flagindex insertString:@"Spring\r" atIndex:0];
                    [m_pSerialPort WriteStringValue:@"WE 002D 00 00 00 00\r"];
                }
                else if (i==7)
                {
                    [flagindex insertString:@"Screw nut\r" atIndex:0];
                    [m_pSerialPort WriteStringValue:@"WE 0031 00 00 00 00\r"];
                }
                else if (i==6)
                {
                    [flagindex insertString:@"big Buck probe\r" atIndex:0];
                    [m_pSerialPort WriteStringValue:@"WE 0035 00 00 00 00\r"];
                }
                else if (i==5)
                {
                    [flagindex insertString:@"small BUCK Probe\r" atIndex:0];
//                    [self WriteAdress:@"0039":@"00 00 00 00"];
                    [m_pSerialPort WriteStringValue:@"WE 0039 00 00 00 00\r"];
                }
                else if (i==4)
                {
                    [flagindex insertString:@"Maintenance date code\r" atIndex:0];
                }
            }
        }
        NSLog(@"index:%@ \r counts:%lu",flagindex,(unsigned long)counts);
//        [m_pSerialPort WriteStringValue:@"WE 0085 00 00 00 00\r"];
        detailflagreset = [NSString stringWithFormat:@"failure flag :\r%@",flagindex];
    }
    return detailflagreset;
}

-(NSString *) ReadAdress:(NSString*)adress
{
    [m_pSerialPort SetDetectString:@"@_@"];
    NSMutableString * cmd= [[NSMutableString alloc] initWithString:@"RE "];
    cmd=[[cmd stringByAppendingString:adress] stringByAppendingString:@" "]; //stringByAppendingString:Num];//num无用
    [m_pSerialPort WriteStringValue:cmd];
    [m_pSerialPort WaitDetect] ;
    NSString *read = [m_pSerialPort ReadStringValue];
    return read;
}

- (IBAction)btnLogIn:(id)sender
{
    [winIICMain beginSheet:winUser completionHandler:^(NSModalResponse returnCode){
        switch (returnCode) {
            case NSModalResponseOK:
//                [menuLog setTitle:@"LogOut"];
                break;
            case NSModalResponseCancel:
                
                break;
            default:
                break;
        }
    }];
}

-(IBAction)btnReset:(id)sender
{
    if (iAuthor == 2)
    {
        [winIICMain beginSheet:winflagreset completionHandler:nil];
    }
    else
    {
        [self ShowMsg:@"Please Login!"];
    }
}



-(IBAction)btnRead:(id)sender
{
    
        NSString *iicread = @"Read EEPROM is: 33 33 83 40 01 00 00 00 06 02 00 00 00 00 00 00 02 00 00 00 00 ff ff ff ff 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 05 12 19 16 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ff ff ff ff 00 00 11 01";
    

    if ([iicread  isEqual: @"Please Check Hardware!\r\n"]) {
        [self ShowMsg:iicread];
    }
    else if ([iicread containsString:@"Read EEPROM is: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff"])
    {
        [self ShowMsg:@"Read from i2c is empty!"];
    }
    else//正常消息
    {
        NSString *str = [iicread substringWithRange:NSMakeRange(79, 347)];
            
        NSArray * RetArry=[str componentsSeparatedByString:@" "];
        NSMutableArray * dealarry= [[NSMutableArray alloc] init];
        
        int j=[RetArry count]/4;
        for(int i=0; i<j; i++)
        {
            NSMutableString * tmp = [NSMutableString string];
            for(int k=3; k>=0; k--)
                [tmp appendString:[RetArry objectAtIndex:i*4+k]];
            [dealarry addObject:tmp ];
        }
        //    NSLog(@"!!!!\r\n%@",dealarry);
        
        for(int i=0; i<[ItemArr count];i++)
        {
            SerialPortView *di = nil;
            if (i==0) {di=d1;}
            else if(i==1){di=d2;}
            else if(i==2){di=d3;}
            else if(i==3){di=d4;}
            else if(i==4){di=d5;}
            else if(i==5){di=d6;}
            else if(i==6){di=d7;}
            else if(i==7){di=d8;}
            else if(i==8){di=d9;}
            else if(i==9){di=d10;}
            else if(i==10){di=d11;}
            else if(i==11){di=d12;}
            else if(i==12){di=d13;}
            else if(i==13){di=d14;}
            else if(i==14){di=d15;}
            else if(i==15){di=d16;}
            else if(i==16){di=d17;}
            else if(i==17){di=d18;}
            else if(i==18){di=d19;}
            else if(i==19){di=d20;}
            else if(i==20){di=d21;}
            else if(i==21){di=d22;}
            else if(i==22){di=d23;}
            else if(i==23){di=d24;}
            else if(i==24){di=d25;}
            else if(i==25){di=d26;}
            else if(i==26){di=d27;}
            else if(i==27){di=d28;}
            else if(i==28){di=d29;}
            
//        NSMutableString * ret1= [[[[[ItemArr[i]  stringByAppendingString:@"      "] stringByAppendingString: [NSString stringWithFormat:@"%lu",ret]] stringByAppendingString:@"      "] stringByAppendingString: LimitArr[i]] stringByAppendingString:@"\r\n"];
            if ([ItemArr[i] hasSuffix:@"Counts"])
            {
                unsigned long ret = strtoul([dealarry[i] UTF8String], 0, 16);
                unsigned long retL=[LimitArr[i] intValue];
                NSMutableString * ret1= [[[[@"" stringByAppendingString: [NSString stringWithFormat:@"%lu",ret]] stringByAppendingString:@"      "] stringByAppendingString: LimitArr[i]] stringByAppendingString:@"\r\n"];

                if(ret>=retL && i<10)
                {
                    NSMutableAttributedString *str = [[NSMutableAttributedString alloc]initWithString:ret1];
                    //设置：内容显示成红色
                    [str addAttribute:NSForegroundColorAttributeName value:[NSColor redColor] range:NSMakeRange(0, str.length)];
                    [di.textStorage appendAttributedString:str];
                    continue;
                }
                NSMutableAttributedString *strxxxx = [[NSMutableAttributedString alloc]initWithString:ret1];
                [di.textStorage appendAttributedString:strxxxx];
            }
            else if([ItemArr[i] hasSuffix:@"Time"])
            {
                NSString *datecode = [iicread substringWithRange:NSMakeRange(198, 12)];//0x3D
                int datesp = [self getdatesp:datecode];
                NSDate *showdate = [NSDate dateWithTimeInterval:(-86400*datesp) sinceDate:[self getnowdate]];
                NSString *strxxx = [NSString stringWithFormat:@"%@",showdate];
                [di setString:strxxx];
            }
            else if ([ItemArr[i] hasSuffix:@"Code"])
            {
                NSString* fail = [dealarry[i] substringWithRange:NSMakeRange(0, 4)];
                [di setString:[self getBinaryByhex:fail]];
            }
            else
            {
                [di setString:dealarry[i]];
            }
        }
    }
}

-(IBAction)btnQuiet:(id)sender
{
    [winIICMain endSheet:winflagreset];
}

-(IBAction)btReset:(id)sender
{
    int i = [sender tag];
    NSLog(@"i=%d",i);
    if (i == 0) {[m_pSerialPort WriteStringValue:@"WE 0015 00 00 00 00\r"];}
    else if(i == 1){[m_pSerialPort WriteStringValue:@"WE 0019 00 00 00 00\r"];}
    else if(i == 2){[m_pSerialPort WriteStringValue:@"WE 001D 00 00 00 00\r"];}
    else if(i == 3){[m_pSerialPort WriteStringValue:@"WE 0021 00 00 00 00\r"];}
    else if(i == 4){[m_pSerialPort WriteStringValue:@"WE 0025 00 00 00 00\r"];}
    else if(i == 5){[m_pSerialPort WriteStringValue:@"WE 0029 00 00 00 00\r"];}
    else if(i == 6){[m_pSerialPort WriteStringValue:@"WE 002D 00 00 00 00\r"];}
    else if(i == 7){[m_pSerialPort WriteStringValue:@"WE 0031 00 00 00 00\r"];}
    else if(i == 8){[m_pSerialPort WriteStringValue:@"WE 0035 00 00 00 00\r"];}
    else if(i == 9){[m_pSerialPort WriteStringValue:@"WE 0039 00 00 00 00\r"];}
    else if(i == 10){[m_pSerialPort WriteStringValue:@"WE 003D 00 00 00 00\r"];}
    else if(i == 11){[m_pSerialPort WriteStringValue:@"WE 0041 00 00 00 00\r"];}
    else if(i == 12){[m_pSerialPort WriteStringValue:@"WE 0045 00 00 00 00\r"];}
    else if(i == 13){[m_pSerialPort WriteStringValue:@"WE 0049 00 00 00 00\r"];}
    else if(i == 14){[m_pSerialPort WriteStringValue:@"WE 004D 00 00 00 00\r"];}
    else if(i == 15){[m_pSerialPort WriteStringValue:@"WE 0051 00 00 00 00\r"];}
    else if(i == 16){[m_pSerialPort WriteStringValue:@"WE 0055 00 00 00 00\r"];}
    else if(i == 17){[m_pSerialPort WriteStringValue:@"WE 0059 00 00 00 00\r"];}
    else if(i == 18){[m_pSerialPort WriteStringValue:@"WE 005D 00 00 00 00\r"];}
    else if(i == 19){[m_pSerialPort WriteStringValue:@"WE 0061 00 00 00 00\r"];}
    else if(i == 20){[m_pSerialPort WriteStringValue:@"WE 0065 00 00 00 00\r"];}
    else if(i == 21){[m_pSerialPort WriteStringValue:@"WE 0069 00 00 00 00\r"];}
    else if(i == 22){[m_pSerialPort WriteStringValue:@"WE 006D 00 00 00 00\r"];}
    else if(i == 23){[m_pSerialPort WriteStringValue:@"WE 0071 00 00 00 00\r"];}
    else if(i == 24){[m_pSerialPort WriteStringValue:@"WE 0075 00 00 00 00\r"];}
    else if(i == 25){[m_pSerialPort WriteStringValue:@"WE 0079 00 00 00 00\r"];}
    else if(i == 26){[m_pSerialPort WriteStringValue:@"WE 007D 00 00 00 00\r"];}
    else if(i == 27){[m_pSerialPort WriteStringValue:@"WE 0081 00 00 00 00\r"];}
    else if(i == 28){[m_pSerialPort WriteStringValue:@"WE 0085 00 00 00 00\r"];}

}

-(IBAction)btIICConfig:(id)sender
{
    [NSApp beginSheet:winIICConfig modalForWindow:winIICMain modalDelegate:self didEndSelector:@selector(sheetDidEnd:returnCode:contextInfo:) contextInfo:nil];
    
    
    [winIICConfig center];
    if ([NSApp runModalForWindow:winIICConfig])    //Click On OK
    {
        [self serialPortInitial];
    }
    return;
}


-(IBAction)IICCheckin:(id)sender
{
    NSString * iicread = [self BUCKREAD];
    //    NSString *iicread = @"Read EEPROM is: 33 33 83 40 01 00 00 00 06 02 00 00 00 00 00 00 02 00 00 00 00 ff ff ff ff 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 05 12 19 16 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ff ff ff ff 00 00 11 01";
    
    //获取sn
    NSString *checkinsn = [self ReadBuckSN:iicread];
    NSLog(@"SN=%@",checkinsn);
    
    if ([iicread  isEqual: @"Please Check Hardware!\r\n"]) {
        [self ShowMsg:iicread];
    }
    else if ([iicread containsString:@"Read EEPROM is: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff"])
    {
        [self ShowMsg:@"Read from i2c is empty!"];
    }
    else//正常消息，进入check in
    {
        //check in failure code，不为0则进入下一步
        NSString *failcode = [iicread substringWithRange:NSMakeRange(414, 12)];//高位在后0x85
        NSLog(@"failcode=%@",failcode);
        NSString *f1 = [failcode substringWithRange:NSMakeRange(10, 2)];
        NSString *f2 = [failcode substringWithRange:NSMakeRange(7, 2)];
        NSString *failure = [NSString stringWithFormat:@"%@%@",f1,f2];
        NSString *detailflag = [self getBinaryByhex:failure];
        
        if(detailflag and [detailflag isEqualToString: @"0000000000000000"])
        {
            [self ShowMsg:@"BUCK Fail code is 0x0000\r\nNo Error Item be found\r\nPlease send to spare part room"];
        }
        else
        {
            
            //查询failcode内详细的spare part flag
            NSUInteger counts = 0;
            NSString * string2 = @"1";
            NSMutableString *flagindex = [[NSMutableString alloc] init];
            for (int i = 0; i < detailflag.length - string2.length + 1; i++)
            {
                if ([[detailflag substringWithRange:NSMakeRange(i, string2.length)] isEqualToString:string2])
                {
                    counts++;
                    if (i==15){[flagindex insertString:@"BUCK yield bad\r" atIndex:0];}
                    else if (i==14){[flagindex insertString:@"Screw\r" atIndex:0];}
                    else if (i==13){[flagindex insertString:@"Left arm\r" atIndex:0];}
                    else if (i==12){[flagindex insertString:@"Right arm\r" atIndex:0];}
                    else if (i==11){[flagindex insertString:@"Top Probe Plate\r" atIndex:0];}
                    else if (i==10){[flagindex insertString:@"Left block\r" atIndex:0];}
                    else if (i==9){[flagindex insertString:@"Right block\r" atIndex:0];}
                    else if (i==8){[flagindex insertString:@"Spring\r" atIndex:0];}
                    else if (i==7){[flagindex insertString:@"Screw nut\r" atIndex:0];}
                    else if (i==6){[flagindex insertString:@"big Buck probe\r" atIndex:0];}
                    else if (i==5){[flagindex insertString:@"small BUCK Probe\r" atIndex:0];}
                    else if (i==4){[flagindex insertString:@"Maintenance date code\r" atIndex:0];}
                    
                }
            }
            //                NSLog(@"index:%@ \r counts:%lu",flagindex,(unsigned long)counts);
            
            //check maintenance flag
            NSString *resetflag = [iicread substringWithRange:NSMakeRange(402, 12)];
            if([resetflag isEqualToString:@"00 00 00 01 "])
            {
                [self ShowMsg:@"THE BUCK IS IN <maintenance station> !"];
            }
            
            //check maintenance datecode》2d？不是则返回failure code；若大于2天则进入下一步。
            NSString *datecode = [iicread substringWithRange:NSMakeRange(198, 12)];//0x3D
            int sp = [self getdatesp:datecode];
            if (sp<=2)
            {
                NSString *datespmsg = [NSString stringWithFormat:@"THE Maintenance Date isn't more than two day\r\nPlease take it to FA\r\r                  failure code:\r%@",flagindex];
                [self ShowMsg:datespmsg];
            }
            else
            {
                //将flag设为1，查询detail flag，弹框提示sn+date+detail flag
                NSString *mainflag = @"WE 0081 00 00 00 01\r";  //new command for 4 shift bytes address write.
                [m_pSerialPort WriteStringValue:mainflag];
                //date
                NSDate *showdate = [NSDate dateWithTimeInterval:(-86400*sp) sinceDate:[self getnowdate]];
                //显示checkin信息
                NSString *checkin = [NSString stringWithFormat:@"SN:%@ \r\n date:%@ \r\n \r\n Detail failure flag:\r\n%@ \r\n",checkinsn,showdate,flagindex];
                [self ShowMsg:checkin];
                [self savelog:checkin];
                [_IICView setString:checkin];
                [self OutPutLog:[iicread substringWithRange:NSMakeRange(79, 347)]];//show spare part cycletime
            }
        }
    }
}

-(IBAction)IICcheckout:(id)sender
{
    NSString * iicread = [self BUCKREAD];
    //    NSString *iicread = @"Read EEPROM is: 33 33 83 40 01 00 00 00 06 02 00 00 00 00 00 00 02 00 00 00 00 ff ff ff ff 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 05 12 19 16 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ff ff ff ff 00 00 11 01";
    
    if ([iicread  isEqual: @"Please Check Hardware!\r\n"]) {
        [self ShowMsg:iicread];
    }
    else if ([iicread containsString:@"Read EEPROM is: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff"])
    {
        [self ShowMsg:@"Read from i2c is empty!"];
    }
    else//正常消息，进入check out
    {
        [self OutPutLog:[iicread substringWithRange:NSMakeRange(79, 347)]];
        //read SN
        NSString *checkoutsn = [self ReadBuckSN:iicread];
        //        [_IICView.textStorage appendAttributedString:checkoutsn];
        
        //check flag = 1? 为1则进入下一步,不为1则提示FA
        NSString *mainflag = [iicread substringWithRange:NSMakeRange(402, 12)];//高位在后0x81
        NSLog(@"mainflag:%@",mainflag);
        if([mainflag isEqualToString:@"00 00 00 00 "])
        {
            [self ShowMsg:@"FLAG ISN'T 1 \r\nPlease take it to <Check In station> !"];
        }
        else
        {
            //share folder查询
            if ([self Searchsn:checkoutsn])
            {
                NSString *checkoutfallmsg = [NSString stringWithFormat:@"PASS!\rPlease Reset!\rSN=%@",checkoutsn];
                [self ShowMsg:checkoutfallmsg];
                [IICRESET setEnabled:true];
            }
        }
    }
}

-(IBAction)IICRESET:(id)sender
{
    NSString * iicread = [self BUCKREAD];
    //    NSString *iicread = @"Read EEPROM is: 33 33 83 40 01 00 00 00 06 02 00 00 00 00 00 00 02 00 00 00 00 ff ff ff ff 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 05 12 19 16 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ff ff ff ff 00 00 11 01";
    
    //reset mainflag
    [NSThread sleepForTimeInterval:1];
    [m_pSerialPort WriteStringValue:@"WE 0081 00 00 00 00\r\n"];
    
    //    查询failcode内详细的spare part flag 并将flag为1的部分reset
    NSString *checkout = [self CheckFailurCode:iicread];
    
    //reset date
    NSDate *date = [NSDate date];
    NSTimeZone *zone = [NSTimeZone systemTimeZone];
    NSInteger interval = [zone secondsFromGMTForDate: date];
    NSDate *datenow = [date  dateByAddingTimeInterval: interval];
    
    NSDateFormatter *format = [[NSDateFormatter alloc] init];
    format.dateFormat=@"YYwwDD";//W本月周；D本年日；h半日时；H本日时；E星期几；F一月中第几个星期几
    NSString *dateString = [format stringFromDate:datenow];
    NSString *yy = [dateString substringWithRange:NSMakeRange(0, 2)];
    NSString *ww = [dateString substringWithRange:NSMakeRange(2, 2)];
    NSString *d1 = [dateString substringWithRange:NSMakeRange(4, 2)];
    NSString *d2 = [dateString substringWithRange:NSMakeRange(6, 1)];
    NSString *writedate = [NSString stringWithFormat:@"WE 003D 0%@ %@ %@ %@\r",d2,d1,ww,yy];
    NSLog(@"datenow=%@;%@;writedate=%@",datenow,dateString,writedate);
    [m_pSerialPort WriteStringValue:writedate];
    
    [IICRESET setEnabled:false];
    [self ShowMsg:checkout];
    [_IICView setString:checkout];
    
    
}

-(IBAction)IICCompare:(id)sender
{
    //    IICACTSN = [[NSMutableString alloc] init];
    [m_pSerialPort SetDetectString:@"@_@"];
    NSString *strCmd = @"BUCK\r";
    [m_pSerialPort WriteStringValue:strCmd];
    [m_pSerialPort WaitDetect] ;
    NSString *iicread = [m_pSerialPort ReadStringValue];
    NSLog(@"iicread %@ \r\n",iicread);
    
    if ([iicread  isEqual: @"Please Check Hardware!\r\n"]) {
        [self ShowMsg:iicread];
    }
    else if ([iicread containsString:@"Read EEPROM is: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff"])
    {
        [self ShowMsg:@"Read from i2c is empty!"];
    }
    else{
        //数据结构
        //11111111111 22 -------- 33333 44444444444 ---------------------15------
        //66 66 06 40 03 00 00 00 39 30 00 00 80 0d 00 00 00 00 00 00 00 00 00 00
        //1:version:2.1;    2:mode:3;   3:sn:12345;     4:cycletime:3456
        
        
        ///////////////////version
        
        NSString *BuckVersion = [iicread substringWithRange:NSMakeRange(15, 14)];
        NSString *bv1 = [BuckVersion substringWithRange:NSMakeRange(1, 2)];
        NSString *bv2 = [BuckVersion substringWithRange:NSMakeRange(4, 2)];
        NSString *bv3 = [BuckVersion substringWithRange:NSMakeRange(7, 2)];
        NSString *bv4 = [BuckVersion substringWithRange:NSMakeRange(10, 2)];
        NSString *bv5 = [NSString stringWithFormat:@"%@%@%@%@",bv4,bv3,bv2,bv1];
        const char *buckver1 = [bv5 UTF8String];
        
        NSString * str= [NSString stringWithFormat:@"%s",buckver1];
        unsigned int fix_data;
        NSScanner * nsscan = [[NSScanner alloc]initWithString: str];
        [nsscan scanHexInt:&fix_data];
        Float32 val;
        memcpy(&val, &fix_data, 4);
        
        NSString *buckversion = [NSString stringWithFormat:@"%f",val];
        //    NSLog(@"buckversion %@",buckversion);
        
        /////////////////////////mode
        
        NSString *IICMode = [iicread substringWithRange:NSMakeRange(28, 3)];
        long iicmode = strtoul([[IICMode substringWithRange:NSMakeRange(0, 2)] UTF8String],0,16);
        //    NSLog(@"iicmode : %ld \n",iicmode);
        
        /////////////////////////sn
        
        NSString *IICSN = [iicread substringWithRange:NSMakeRange(39, 7)];
        NSString *iicsn1 = [IICSN substringWithRange:NSMakeRange(4, 2)];
        NSString *iicsn2 = [IICSN substringWithRange:NSMakeRange(1, 2)];
        NSString *iicsn3 = [NSString stringWithFormat:@"%@%@",iicsn1,iicsn2];
        long iicsn4 = strtoul([[iicsn3 substringWithRange:NSMakeRange(0, 4)] UTF8String], 0, 16);
        NSNumber *longnumber = [NSNumber numberWithLong:iicsn4];
        NSString *iicsn = [longnumber stringValue];
        //    NSLog(@"iicsn : %ld \n",iicsn);
        
        /////////////////////////cycletime
        
        NSString *IICcycletime = [iicread substringWithRange:NSMakeRange( 45, 12)];
        NSString *iicc1 = [IICcycletime substringWithRange:NSMakeRange(10, 2)];
        NSString *iicc2 = [IICcycletime substringWithRange:NSMakeRange(7, 2)];
        NSString *iicc3 = [IICcycletime substringWithRange:NSMakeRange(4, 2)];
        NSString *iicc4 = [IICcycletime substringWithRange:NSMakeRange(1, 2)];
        NSString *iicc5 = [NSString stringWithFormat:@"%@%@%@%@",iicc1,iicc2,iicc3,iicc4];
        long iiccycletime = (strtoul([[iicc5 substringWithRange:NSMakeRange(0, 8)] UTF8String], 0, 16)/65536);
        //    NSLog(@"iiccycletime : %ld \n",iiccycletime);
        
        //////////////////////显示转义后的数据
        NSString *IICserialport = [NSString stringWithFormat:@" BuckVersion :%@\r Mode :%ld\r SN :%@\r Cycletime :%ld\n",buckversion,iicmode,iicsn,iiccycletime];
        //    NSLog(@"iic %@",IICserialport);
        [_IICView setString:IICserialport];
        
        ////////////////////与文件对比显示结果
        //    [IICACTSN setString:@""];
        //    [IICACTSN appendString:iicsn];
        //    [self Searchsn];
    }
}

@end
