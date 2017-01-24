//
//  IADataConnection.m
//  AINetFixture
//
//  Created by ZL-Pro on 16/1/12.
//  Copyright (c) 2016年 Louis. All rights reserved.
//
#import "CoreLib/Common.h"
#import <CoreLib/TestContext.h>
#import <CoreLib/TestEngine.h>
#import "PLCData.h"
#import "PLCAct.h"
#import "CIANetFixture.h"
#import "IASocketDelegate.h"
#import "IAFeelingData.h"

#define PLCDATAPORT 5058    //修改

extern int g_JoyDownFlag;
extern int g_JoyRotateFlag;
extern TestEngine * pTestEngine;

@implementation PLCData

-(IBAction)bt_onConnect:(id)sender
{
    if (m_socketData) {
        [m_socketData close];
        [m_socketData release];
        m_socketData=nil;

    }
    m_socketData=[[IASocketConnection alloc] initWithHost:[[m_tfIP stringValue] UTF8String] Port:PLCDATAPORT];
    [m_socketData setDelegate:self];
    [m_socketData open:5.0];
    m_pipedatalen=0;
    if (sender!=nil) {
       [(IASocketDelegate*)[_window delegate] SaveSocketConfigLocalCellIP:nil LocalCellPort:0 PLCIP:[m_tfIP stringValue]];
    }
    [CTestContext::m_dicGlobal setObject:g_PLCdata_ui forKey:@"Oculus_UI_PLCData"];//将view设为对象，关联仍在，其他地方直接使用对象操作
}
-(IBAction)bt_onClose:(id)sender
{
    [m_socketData close];
    [m_socketData release];
    m_socketData=nil;
    [self performSelectorOnMainThread:@selector(SocketStatus:) withObject:@NO waitUntilDone:YES];
}
-(IBAction)bt_onSendstart:(id)sender//close
{
    g_SendBeckHoff sendermsg = {0,0,0,0,0};
    NSInteger i = [sender tag];
    switch (i) {
        case 9:
//            sendermsg.commandhead = 0x44444444;
//            sendermsg.data1 = 0x01;
            break;
        default:
            break;
    }
    long b=[self SendCmd:sendermsg];
    if (b<0) {
        NSLog(@"Socket open fail %ld",b);
    }
    else{
        plcreadflag = 0;
        [self PLCDataDraw];
    }
}
-(IBAction)bt_onSendEnd:(id)sender//close
{
    g_SendBeckHoff sendermsg = {0,0,0,0,0};
    NSInteger i = [sender tag];
    switch (i) {
        case 10:
//            sendermsg.commandhead = 0x44444444;
//            sendermsg.data1 = 0x02;
            break;
        default:
            break;
    }
    long b=[self SendCmd:sendermsg];
    if (b<0) {
        NSLog(@"Socket open fail %ld",b);
    }
    else{
        plcreadflag = 1;
    }
}
-(IBAction)PLCADDFile:(id)sender
{
    NSOpenPanel * panel = [NSOpenPanel openPanel];
    [panel setCanChooseDirectories:YES];
    [panel runModal];
    BOOL bRet=NO;
    NSFileManager * fm = [NSFileManager defaultManager];
    NSString * str = [[panel URL] path];
    [fm fileExistsAtPath:str isDirectory:&bRet];
    if (str)
    {
        NSData *data1=[NSData dataWithContentsOfFile:str];
        NSData* senddata = [zipAndUnzip gzipInflate:data1];
        [m_drawPLCView plcfileread:senddata];
        [g_PLCdata_ui plcfileread:senddata];
        [self performSelectorOnMainThread:@selector(PLCUpdateView) withObject:nil waitUntilDone:NO];
    }
}

-(unsigned int)AppendData:(g_ReadBeckHoff)data
{
//    if ( m_plcsave.flen == 0) {
//        time_startpoint = data.data2;
//    }
//    m_plcsave.line1[m_plcsave.flen]=data.data1/10;
//    m_plcsave.line2[m_plcsave.flen]=data.data1/10;
//    m_plcsave.line2[m_plcsave.flen]=data.data1/10;
//    m_plcsave.time[m_plcsave.flen]=(data.data2-time_startpoint)/100000;
//    m_plcsave.flen++;
    return m_plcsave.flen;
}
-(void)clearData
{
    memset(&m_plcsave, 0, sizeof(m_plcsave));
}

-(int)PLCDataDraw
{
    [g_PLCdata_ui plcsetdata:m_plcsave];
    m_drawPLCView.plcviewtitle = @"JoyStick_Circle";
    [self performSelectorInBackground:@selector(LuaTestPLCDataSave) withObject:nil];
    [self performSelectorOnMainThread:@selector(PLCUpdateView) withObject:nil waitUntilDone:YES];
    return 0;
}

-(IBAction)PLCdataSave:(id)sender//close
{
    NSString *name = [m_dataname stringValue];
    if (![name  isEqual: @""]) {
        NSString *filepath = [NSString stringWithFormat:@"/vault/Intelli_PM/Data/%@.data",name];
        NSData *data1 = [[NSData alloc] initWithBytes:&m_plcsave length:sizeof(m_plcsave)];
        NSData *savedata=[zipAndUnzip gzipDeflate:data1];
        [data1 release];

        BOOL isSaved=[savedata writeToFile:filepath atomically:YES];
        if(isSaved){
            NSLog(@"plcdata save success!");
        }else{
            NSLog(@"plcdata save fail!");
        }
    }
}
-(void)LuaTestPLCDataSave
{
    CTestContext* pTestContext0 = (CTestContext *)[pTestEngine GetTestContext:0];

    const char * mlbsn=pTestContext0->getContext((char*)"MLB_SN",0);
    const char * LineNumber=pTestContext0->getContext((char*)"LineNumber",1);
    const char * StationNumber=pTestContext0->getContext((char*)"StationNumber",1);
    const char * Platfrom=pTestContext0->getContext((char*)"TEST_PLATFORM",0);
    if (Platfrom==NULL) {
        NSLog(@"TEST_PLATFORM is NULL");
        return;
    }
    NSDateFormatter * fmt = [[NSDateFormatter alloc]init];
    [fmt setDateFormat:@"yyyy-MM-dd_hh-mm-ss"];
    NSString * date = [fmt stringFromDate:[NSDate date]];
    [fmt release];
    
    NSDateFormatter * fmt1 = [[NSDateFormatter alloc]init];
    [fmt1 setDateFormat:@"yyyy-MM-dd"];
    NSString * date1 = [fmt1 stringFromDate:[NSDate date]];
    [fmt1 release];
    NSString *filepath;
    if (strcmp(mlbsn, "")==0) {
        filepath= [NSString stringWithFormat:@"%@/%@/%s_JoyStickCircle_NOSN__%@.csv",csvfilepath,date1,Platfrom,date];
    }else
    {
        filepath= [NSString stringWithFormat:@"%@/%@/%s_JoyStickCircle_%s_%@.csv",csvfilepath,date1,Platfrom,mlbsn,date];
    }

//    NSData *data1 = [[NSData alloc] initWithBytes:&m_plcsave length:sizeof(m_plcsave)];
//    NSData *savedata=[zipAndUnzip gzipDeflate:data1];
//        
//    BOOL isSaved=[savedata writeToFile:filepath atomically:YES];
//    [data1 release];
    NSString * flie_csvpath = [NSString stringWithFormat:@"%@/%@",csvfilepath,date1];
//    NSString * file_path = [NSString stringWithFormat:@"%@/%@/tmp",zipfilepath,date1];
//    [self CreatePath:file_path];
    [self CreatePath:flie_csvpath];
    
    BOOL isSaved=[IAFeelingData SavePLCDatatoFile:&m_plcsave LineNumber:LineNumber StationNumber:StationNumber SerialNumber:mlbsn Platfrom:Platfrom KeyName:"JoyStickCircle" Filename:[filepath cStringUsingEncoding:NSASCIIStringEncoding]];    
    if(isSaved){
        NSLog(@"PLCData data save success!/r%@",filepath);
    }else{
        NSLog(@"PLCData data save fail!/r%@",filepath);
    }
}
-(int)CreatePath:(NSString*)path
{
    NSFileManager *manager = [NSFileManager defaultManager];
    BOOL isDir, valid = [manager fileExistsAtPath:path isDirectory:&isDir];
    if(valid)
    {
        return 1;
    }
    BOOL r=[[NSFileManager defaultManager] createDirectoryAtPath:path withIntermediateDirectories:YES attributes:nil error:nil];
    if (r) {
        return 0;
    }
    return -1;
}

//-(void)SaveCSV
//{
//    int flen = m_plcsave.flen;
//    if(flen>0){
//        NSString * filePath = @"/vault/Intelli_PM/Data/Data.csv";
//        //dic为预留NSMutableDictionary，可传入所需保存的数据。对应下面dic_content。
//        //        NSMutableArray * items = [[NSMutableArray alloc] init];
//        //        NSArray *array=[[NSArray alloc]initWithContentsOfFile:filePath];
//        //        if ([array count]>0) {
//        //            for(int i=0;i<[array count];i++){
//        //                NSDictionary *dic=[array objectAtIndex:i];
//        //                [items addObject:dic];
//        //            }
//        //        }
//        if(![[NSFileManager defaultManager]fileExistsAtPath:filePath])
//        {
//            NSString * first = [NSString stringWithFormat:FIRSTLINE,@"Oculus GamePad"];
//            //            NSMutableString * itemname = [NSMutableString string];
//            //            NSMutableString * upper = [NSMutableString string];
//            //            NSMutableString * lower = [NSMutableString string];
//            //            NSMutableString * unit = [NSMutableString string];
//            //            for(int i=0;i<[key count]; i++)
//            //            {
//            //                NSDictionary * item = [dic objectForKey:[key objectAtIndex:i]];
//            //                [itemname appendString:[NSString stringWithFormat:@"%@,",[item objectForKey:keyNAME]]];
//            //                [upper appendString:[NSString stringWithFormat:@"%@,",[item objectForKey:keyUPPER]]];
//            //                [lower appendString:[NSString stringWithFormat:@"%@,",[item objectForKey:keyLOWER]]];
//            //                [unit appendString:[NSString stringWithFormat:@"%@,",[item objectForKey:keyUNIT]]];
//            //            }
//            NSString * second = [NSString stringWithFormat:SECONDLINE,_VALUE];
//            //            NSString * third = [NSString stringWithFormat:THIRDLINE,upper];
//            //            NSString * fourth = [NSString stringWithFormat:FOURTHLINE,lower];
//            //            NSString * fifth = [NSString stringWithFormat:FIFTHLINE,unit];
//            NSString * content = [NSString stringWithFormat:@"%@\n%@\n",first,second];
//            [content writeToFile:filePath atomically:YES encoding:NSUTF8StringEncoding error:nil];
//        }
//        
//        NSMutableDictionary *dic_content = [[NSMutableDictionary alloc] init];
//        NSMutableDictionary *dic_value = [[NSMutableDictionary alloc] init];
//        NSMutableString *value = [NSMutableString string];
//        NSDateFormatter * fmt = [[NSDateFormatter alloc]init];
//        [fmt setDateFormat:@"yyyy-MM-dd_hh-mm-ss"];
//        NSString * date = [fmt stringFromDate:[NSDate date]];
//        [fmt release];
//        
//        NSMutableString *Press = [[NSMutableString alloc] init];
//        NSMutableString *Time = [[NSMutableString alloc] init];
//        int i;
//        for(i=0; i<flen ; i++){
//            [Press appendString:[NSString stringWithFormat:@"%d/",m_plcsave.data1[i]]];
//            [Time appendString:[NSString stringWithFormat:@"%d/",m_plcsave.data2[i]]];
//        }
//        [dic_value setValue:Press forKey:_Y];
//        [dic_value setValue:Time forKey:_X];
//        [Press release];
//        [Time release];
//        
//        [value appendString:[NSString stringWithFormat:@"%@,%@,%@",date,[dic_value objectForKey:_X],[dic_value objectForKey:_Y]]];
//        NSString * cont = [NSString stringWithFormat:CONTENT,[dic_content objectForKey:_SN],[dic_content objectForKey:_STATION],[dic_content objectForKey:_SITE],[dic_content objectForKey:_RESULT],[dic_content objectForKey:_ERR],value];
//        NSFileHandle * file = [NSFileHandle fileHandleForWritingAtPath:filePath];
//        [file seekToEndOfFile];
//        [file writeData:[cont dataUsingEncoding:NSUTF8StringEncoding]];
//        [file closeFile];
//    }
//}

-(void)SaveDataStruct:(g_PLCStruct )data
{
    NSDateFormatter * fmt = [[NSDateFormatter alloc]init];
    [fmt setDateFormat:@"yyyy-MM-dd_hh-mm-ss"];
    NSString * date = [fmt stringFromDate:[NSDate date]];
    [fmt release];
    CTestContext* pTestContext0 = (CTestContext *)[pTestEngine GetTestContext:0];
    const char * mlbsn=pTestContext0->getContext((char*)"MLB_SN",0);
    NSString *filepath;
    if (strcmp(mlbsn, "")==0) {
        filepath= [NSString stringWithFormat:@"/vault/Intelli_PM/Data/tmp/JoyStickCircle_NOSN__%@.data",date];
    }else
    {
        filepath= [NSString stringWithFormat:@"/vault/Intelli_PM/Data/tmp/JoyStickCircle_%s_%@.data",mlbsn,date];
    }
    NSData * data1=[NSData dataWithBytes:&m_plcsave length:sizeof(m_plcsave)];
    NSData *savedata=[zipAndUnzip gzipDeflate:data1];
    [savedata writeToFile:filepath atomically:YES];
}

- (NSSize)windowWillResize:(NSWindow *)sender toSize:(NSSize)frameSize
{
    return frameSize;
}

-(void)PLCUpdateView
{
    [m_drawPLCView setNeedsDisplay:YES];
    [[CTestContext::m_dicGlobal objectForKey:@"Oculus_UI_PLCData"] setNeedsDisplay:YES];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"Oculus_UI_PLCData" object:nil];
}
- (void)SocketStatus:(NSNumber*)number
{
    [m_socketstatue setState:[number boolValue]];
    [m_tfIP setEnabled:![number boolValue]];
    [m_btConnect setEnabled:![number boolValue]];
    [m_btClose setEnabled:[number boolValue]];
}
-(BOOL)ConnectWithIP:(const char*)ip
{
    if (m_socketData)
    {
        [m_socketData close];
        [m_socketData release];
        m_socketData=nil;
        
    }
    m_socketData=[[IASocketConnection alloc] initWithHost:ip Port:PLCDATAPORT];
    [m_socketData setDelegate:self];
    return [m_socketData open:5.0];
}
-(void)close
{
    [m_socketData close];
    [m_socketData release];
    m_socketData=nil;
}
-(BOOL)isOpen
{
    if (m_socketData==nil) {
        return NO;
    }
    return [m_socketData isOpen];
}
-(void)dealloc
{
    [self close];
    [super dealloc];
}

-(void)ClearInBuffer
{
    if (![self isOpen])
    {
        return ;
    }
    char tmpbuffter[255];
    while ([[m_socketData InputStream] hasBytesAvailable]) {
        [m_socketData read:tmpbuffter maxLength:255];
    }
}

-(long)SendCmd:(g_SendBeckHoff)cmd
{
    if (![self isOpen]) {
        return -2;
    }
    [m_socketData write:(const char*)&cmd maxLength:sizeof(g_SendBeckHoff)];
    [self performSelectorOnMainThread:@selector(insertTexttoView:) withObject:[NSString stringWithFormat:@"PLCData SOCKET 5058 Send:%08x,%08x,%08x,%08x\r",cmd.bh_Array_cmd[0].bh_function_code,cmd.bh_Array_cmd[0].bh_device_id,cmd.bh_Array_cmd[0].bh_op_value1,cmd.bh_Array_cmd[0].bh_op_value2] waitUntilDone:YES];
    return 0;
}
-(void)insertTexttoView:(NSString*)text
{
    NSAttributedString * theString = [[NSAttributedString alloc] initWithString:[NSString stringWithFormat:@"%@",text]];
    [[m_textView textStorage] appendAttributedString: theString];
    NSUInteger length = [[m_textView textStorage] length];
    NSRange theRange = NSMakeRange(length, 0);
    [m_textView scrollRangeToVisible:theRange];
    [theString release];
}
-(long)ReadData:(g_ReadBeckHoff*) reader MaxLenght:(unsigned int)maxLenght
{
    if (![self isOpen]) {
        return -2;
    }
    memset(reader, 0, maxLenght*sizeof(g_ReadBeckHoff));
    long nlen=[m_socketData read:(char*)reader maxLength:maxLenght*sizeof(g_ReadBeckHoff)];
    return nlen/sizeof(g_ReadBeckHoff);
}


-(void)ShowMsg:(NSString*)msg
{
    [self performSelectorOnMainThread:@selector(fnErrMsg:) withObject:msg waitUntilDone:NO];
}

-(void)fnErrMsg:(id)par
{
    NSRunAlertPanel(@"Socket Information", @"%@", @"OK", nil, nil, par);
}
- (void)dataready:(NSStream *)aStream handleEvent:(NSStreamEvent)eventCode
{
    NSLog(@"a=%d,b=%d",g_JoyRotateFlag,g_JoyDownFlag);
    if (g_JoyRotateFlag==1 or g_JoyDownFlag==1)
    {
        NSLog(@"a");
        uint8_t buffer[20];
        NSInteger actuallyRead = sizeof(buffer);
        NSInputStream *instream=(NSInputStream*)aStream;
        actuallyRead = [instream read:(uint8_t *)buffer maxLength:sizeof(buffer)];
        if(actuallyRead<20)
        {
            NSLog(@"read data error(data lenght=%ld",actuallyRead);
            return;
        }
        buffer[actuallyRead]=0;
        memset(&m_tmpReadCmd, 0, sizeof(m_tmpReadCmd));
        memcpy(&m_tmpReadCmd, buffer, sizeof(m_tmpReadCmd));
        [self AppendData:m_tmpReadCmd];
    }
    else if (is_Debug)
    {
        uint8_t buffer[20];
        NSInteger actuallyRead = sizeof(buffer);
        NSInputStream *instream=(NSInputStream*)aStream;
        actuallyRead = [instream read:(uint8_t *)buffer maxLength:sizeof(buffer)];
        if(actuallyRead<20)
        {
            NSLog(@"read data error(data lenght=%ld",actuallyRead);
            return;
        }
        buffer[actuallyRead]=0;
        memset(&m_tmpReadCmd, 0, sizeof(m_tmpReadCmd));
        memcpy(&m_tmpReadCmd, buffer, sizeof(m_tmpReadCmd));
    }else
    {
        uint8_t buffer[20];
        NSInteger actuallyRead = sizeof(buffer);
        NSInputStream *instream=(NSInputStream*)aStream;
        if([instream hasBytesAvailable])
        {
            actuallyRead = [instream read:(uint8_t *)buffer maxLength:sizeof(buffer)];
            NSLog(@"plc data no receipt=%ld\n",actuallyRead);
        }else
        {
            
        }
    }
}
- (void)streamerror:(NSStream *)aStream handleEvent:(NSStreamEvent)eventCode
{
    switch ([aStream streamStatus]) {
        case NSStreamStatusNotOpen:
            [self ShowMsg:@"connect PLC Data error"];
            [self performSelectorOnMainThread:@selector(SocketStatus:) withObject:[NSNumber numberWithBool:NO] waitUntilDone:YES];
            break;
        case NSStreamStatusOpening:
            [self ShowMsg:@"connecting to PLC Data Server, please wait a minute"];
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
            [self bt_onClose:nil];
            [self ShowMsg:@"PLC Data Server stop"];
            [self performSelectorOnMainThread:@selector(SocketStatus:) withObject:[NSNumber numberWithBool:NO] waitUntilDone:YES];
            break;
        case NSStreamStatusClosed:
            [self bt_onClose:nil];
            [self ShowMsg:@"PLC Data Server stop"];
            [self performSelectorOnMainThread:@selector(SocketStatus:) withObject:[NSNumber numberWithBool:NO] waitUntilDone:YES];
            break;
        case NSStreamStatusError:
            [self bt_onClose:nil];
            [self ShowMsg:@"PLC Data Server error"];
            [self performSelectorOnMainThread:@selector(SocketStatus:) withObject:[NSNumber numberWithBool:NO] waitUntilDone:YES];
            break;
        default:
            break;
    }
}
- (void)ConnectTimeout:(NSTimer *)timer Socket:(IASocketConnection*)socket
{
    [self bt_onClose:nil];
    [self performSelectorOnMainThread:@selector(SocketStatus:) withObject:[NSNumber numberWithBool:NO] waitUntilDone:YES];
    
    [self ShowMsg:@"connect to PLC Data server timeout"];
}
- (void)opencompleted:(NSStream *)aStream handleEvent:(NSStreamEvent)eventCode
{
    [self performSelectorOnMainThread:@selector(SocketStatus:) withObject:[NSNumber numberWithBool:YES] waitUntilDone:YES];
}

@end
