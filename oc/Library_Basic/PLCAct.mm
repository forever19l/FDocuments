//
//  IADataConnection.m
//  AINetFixture
//
//  Created by ZL-Pro on 16/1/12.
//  Copyright (c) 2016年 Louis. All rights reserved.
//
#import "CoreLib/Common.h"
#import "PLCAct.h"
#import "CIANetFixture.h"
#import "IASocketDelegate.h"
#import "PLCData.h"
#define PLCActPort 5059

int g_JYDownFlag = 0;//origin 0 , act 1 ,done 2
int g_JYRotateFlag = 0;
@implementation PLCAct

- (instancetype)init
{
    self = [super init];
    if (self) {
        is_Stop = YES;
        pthread_mutex_init(&plcAct_mutex, NULL);
    }
    return self;
}
-(void)dealloc
{
    pthread_mutex_destroy(&plcAct_mutex);
    [self close];
    [super dealloc];
}

-(IBAction)bt_onConnect:(id)sender
{
    if (m_socketRec) {
        [m_socketRec close];
        [m_socketRec release];
        m_socketRec=nil;
    }
    m_socketRec=[[IASocketConnection alloc] initWithHost:[[m_tfIP stringValue] UTF8String] Port:PLCActPort];
    [m_socketRec setDelegate:self];
    [m_socketRec open:5.0];
    if (sender!=nil) {
        [(IASocketDelegate*)[_window delegate] SaveSocketConfigLocalCellIP:nil LocalCellPort:0 PLCIP:[m_tfIP stringValue]];
    }
}
-(IBAction)bt_onClose:(id)sender
{
    is_Stop=YES;
    [m_socketRec close];
    [m_socketRec release];
    m_socketRec=nil;
    [self performSelectorOnMainThread:@selector(SocketStatus:) withObject:@NO waitUntilDone:YES];
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
    if (m_socketRec)
    {
        [m_socketRec close];
        [m_socketRec release];
        m_socketRec=nil;
    }
    m_socketRec=[[IASocketConnection alloc] initWithHost:ip Port:PLCActPort];
    [m_socketRec setDelegate:self];
    return [m_socketRec open:5.0];
}

-(void)initwindow
{
    [m_tfIP setEnabled:YES];
    [m_btConnect setEnabled:YES];
    [m_btClose setEnabled:NO];
}
-(void)close
{
    is_Stop=YES;
    [m_socketRec close];
    [m_socketRec release];
    m_socketRec=nil;
}
-(BOOL)isOpen
{
    if (m_socketRec==nil) {
        return NO;
    }
    return [m_socketRec isOpen];
}
-(void)lock
{
    pthread_mutex_lock(&plcAct_mutex);
}
-(void)unlock
{
    pthread_mutex_unlock(&plcAct_mutex);
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
-(void)ShowMsg:(NSString*)msg
{
    [self performSelectorOnMainThread:@selector(fnErrMsg:) withObject:msg waitUntilDone:NO];
}
-(void)fnErrMsg:(id)par
{
    NSDictionary * dic = [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithInt:0],@"id",par,@"msg",nil];
    [[NSNotificationCenter defaultCenter] postNotificationName:kNotificationOnTestError object:nil userInfo:dic];
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
            [self bt_onClose:nil];
            [self ShowMsg:@"PLC Command Server stop"];
            [self performSelectorOnMainThread:@selector(SocketStatus:) withObject:[NSNumber numberWithBool:NO] waitUntilDone:YES];
            break;
        case NSStreamStatusClosed:
            [self bt_onClose:nil];
            [self ShowMsg:@"PLC Command Server stop"];
            [self performSelectorOnMainThread:@selector(SocketStatus:) withObject:[NSNumber numberWithBool:NO] waitUntilDone:YES];
            break;
        case NSStreamStatusError:
            [self bt_onClose:nil];
            [self ShowMsg:@"PLC Command Server error"];
            [self performSelectorOnMainThread:@selector(SocketStatus:) withObject:[NSNumber numberWithBool:NO] waitUntilDone:YES];
            break;
        default:
            break;
    }
}
-(void)ConnectTimeout:(NSTimer *)timer Socket:(IASocketConnection*)socket
{
    [self bt_onClose:nil];
    [self performSelectorOnMainThread:@selector(SocketStatus:) withObject:[NSNumber numberWithBool:NO] waitUntilDone:YES];
    [self ShowMsg:@"connect to PLC Command server timeout"];
}
-(void)opencompleted:(NSStream *)aStream handleEvent:(NSStreamEvent)eventCode
{
    [self performSelectorOnMainThread:@selector(SocketStatus:) withObject:[NSNumber numberWithBool:YES] waitUntilDone:YES];
}

- (void)dataready:(NSStream *)aStream handleEvent:(NSStreamEvent)eventCode      //缓冲区
{
    int flen_buffer = 36;
    uint8_t buffer[flen_buffer];     //head 16,cmd 20;是否plc-》mac只有一个cmd
    NSInteger actuallyRead = sizeof(buffer);
    NSInputStream *instream=(NSInputStream*)aStream;
    actuallyRead = [instream read:(uint8_t *)buffer maxLength:sizeof(buffer)];
    if(actuallyRead<flen_buffer)
    {
        NSLog(@"read data error(data lenght=%ld)",actuallyRead);
        return;
    }
    buffer[actuallyRead]=0;
    memset(&m_tmpBHread, 0, sizeof(m_tmpBHread));
    memcpy(&m_tmpBHread, buffer, sizeof(m_tmpBHread));
    
    NSDateFormatter * fmt = [[NSDateFormatter alloc]init];
    [fmt setDateFormat:@"yyyy-MM-dd_hh-mm-ss"];
    NSString * date = [fmt stringFromDate:[NSDate date]];
    [fmt release];
    NSString *strmu=[NSString stringWithFormat:@"%@ -> 5059 Read:%08x,%08x,%08x,%08x,%08x,%08x,CMD = %08x,%08x,%08x,%08x\r",date , m_tmpBHread.bh_Version , m_tmpBHread.bh_Trans_ID , m_tmpBHread.bh_cmd_Type , m_tmpBHread.bh_Block_ID , m_tmpBHread.bh_Group_ID , m_tmpBHread.bh_Cmd_cnt , m_tmpBHread.bh_Array_cmd[0].bh_function_code , m_tmpBHread.bh_Array_cmd[0].bh_device_id , m_tmpBHread.bh_Array_cmd[0].bh_op_value1 , m_tmpBHread.bh_Array_cmd[0].bh_op_value2 ];
    [self performSelectorOnMainThread:@selector(insertTexttoView:) withObject:strmu waitUntilDone:YES];
    
    if (m_tmpBHread.bh_Array_cmd[0].bh_function_code==0x270f) { //start test cmd
        NSDictionary *dic=[NSDictionary dictionaryWithObjectsAndKeys:
                           [NSNumber numberWithInt:m_tmpBHread.bh_Array_cmd[0].bh_op_value1],@"data1",
                           [NSNumber numberWithInt:m_tmpBHread.bh_Array_cmd[0].bh_op_value2],@"data2",
                           nil];
        [[NSNotificationCenter defaultCenter]postNotificationName:kNotificationDoTestStart object:nil userInfo:dic];
    }
//    if (m_tmpReadCmd.commandhead==0x05f5e0ff and m_tmpReadCmd.commandType == 2) {
//        NSDictionary *dic=[NSDictionary dictionaryWithObjectsAndKeys:
//                           [NSNumber numberWithInt:m_tmpReadCmd.data1],@"LOCKPANLE",
//                           nil];
//        
//        [[NSNotificationCenter defaultCenter]postNotificationName:@"LOCKBARCODE" object:nil userInfo:dic];
//    }
//    if (m_tmpReadCmd.commandhead==0x05f5e0ff and m_tmpReadCmd.commandType == 3) {
//        NSDictionary *dic=[NSDictionary dictionaryWithObjectsAndKeys:
//                           [NSNumber numberWithInt:3],@"LOCKPANLE",
//                           nil];
//        [[NSNotificationCenter defaultCenter]postNotificationName:@"LOCKBARCODE" object:nil userInfo:dic];
//    }
    else
    {
        [[m_inputpipe fileHandleForWriting] writeData:[NSData dataWithBytes:buffer length:flen_buffer]];
        m_pipedatalen=m_pipedatalen+flen_buffer;
    }
}

-(long)ReadPlcActCmd:(g_ReadBeckHoff)reader Timeout:(unsigned int)iTimeOut
{
    if (![self isOpen]) {
        return -2;
    }
    NSTimeInterval starttime = [[NSDate date]timeIntervalSince1970];
    double tm = (double)iTimeOut/1000.0;
    while(m_pipedatalen<sizeof(g_ReadBeckHoff))
    {
        NSTimeInterval now = [[NSDate date]timeIntervalSince1970];
        if ((now-starttime)>=tm)
        {
            return -1;       //time out
        }
        [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate date]];
        [NSThread sleepForTimeInterval:0.01];
    }
    NSData *data=[[m_inputpipe fileHandleForReading] readDataOfLength:sizeof(g_ReadBeckHoff)];
    m_pipedatalen=m_pipedatalen-[data length];
    memcpy(&reader, [data bytes], [data length]);
    return 0;
}

-(long)ReadActRecv:(unsigned int)itimeout{
    memset(&m_reader_act, 0, sizeof(g_ReadBeckHoff));
    long ret = [self ReadPlcActCmd:m_reader_act Timeout:10];
    if(m_reader_act.bh_Array_cmd[0].bh_op_value1 == 0x01){//recv
        if (m_reader_act.bh_Array_cmd[0].bh_function_code == 7) {//jy down
            g_JYDownFlag = 1;
        }
        if (m_reader_act.bh_Array_cmd[0].bh_function_code == 8) {//jy rotate
            g_JYRotateFlag = 1;
        }
        return 1;
    }
    else if (m_reader_act.bh_Array_cmd[0].bh_op_value1 == 0x03){//refuse
        return -9;
    }
    else{
        return ret;
    }
}

-(long)ReadActDone:(unsigned int)itimeout{
    memset(&m_reader_act, 0, sizeof(g_ReadBeckHoff));
    long ret = [self ReadPlcActCmd:m_reader_act Timeout:10];
    if(m_reader_act.bh_Array_cmd[0].bh_op_value1 == 0x02){//done
        if (m_reader_act.bh_Array_cmd[0].bh_function_code == 7) {//jy down
            g_JYDownFlag = 2;
        }
        if (m_reader_act.bh_Array_cmd[0].bh_function_code == 8) {//jy rotate
            g_JYRotateFlag = 2;
        }
        return 1;
    }
    else if (m_reader_act.bh_Array_cmd[0].bh_op_value1 == 0x03){//refuse
        return -9;
    }
    else{
        return ret;
    }
}

-(long)SendCmd:(g_SendBeckHoff)cmd
{
    if (![self isOpen]) {
        return -2;
    }
    [m_socketRec write:(const char*)&cmd maxLength:sizeof(g_SendBeckHoff)];
    NSDateFormatter * fmt = [[NSDateFormatter alloc]init];
    [fmt setDateFormat:@"yyyy-MM-dd_hh-mm-ss"];
    NSString * date = [fmt stringFromDate:[NSDate date]];
    [fmt release];
    NSMutableString *strmu = [[NSMutableString alloc] init];
    [strmu appendString:[NSString stringWithFormat:@"%@ ->PLCAct %d Read:%08x,%08x,%08x,%08x,%08x,%08x\r",date, PLCActPort ,cmd.bh_Version, cmd.bh_Trans_ID, cmd.bh_cmd_Type, cmd.bh_Block_ID, cmd.bh_Group_ID, cmd.bh_Cmd_cnt]];
    if (cmd.bh_Cmd_cnt > 0) {
        for (int i = 0; i < cmd.bh_Cmd_cnt; i++) {
            [strmu appendString:[NSString stringWithFormat:@"CMD%d = %08x,%08x,%08x,%08x\r" , i ,cmd.bh_Array_cmd[i].bh_function_code , cmd.bh_Array_cmd[i].bh_device_id , cmd.bh_Array_cmd[i].bh_op_value1 , cmd.bh_Array_cmd[i].bh_op_value2 ] ];
        }
    }
    [self performSelectorOnMainThread:@selector(insertTexttoView:) withObject:strmu waitUntilDone:YES];
    [strmu release];
    return 0;
}

-(long)m_SendCmd:(g_SendBeckHoff)cmd{           //调用之前先把send cmd置0再赋值
    long ret1 = [self SendCmd:m_sender_act];
    if (ret1 < 0) {
        NSLog(@"m_SendCmd send cmd failed=%ld",ret1);
        return ret1;
    }
    else{
        long ret2 = [self ReadActRecv:10];
        if (ret2 < 0) {
            NSLog(@"m_SendCmd read recv cmd failed=%ld",ret2);
            return ret2;
        }
        else{
            long ret3 = [self ReadActDone:10];
            if (ret3 < 0) {
                NSLog(@"m_SendCmd read done cmd failed=%ld",ret3);
                return ret3;
            }
            else{
                return 1;
            }
        }
    }
}

-(long)mm_Debug_Act:(int)i{
    memset(&m_sender_act, 0, sizeof(g_SendBeckHoff));
    float x = [l_x floatValue];
    float z = [l_z floatValue];
    float y1 = [l_y1 floatValue];
    float y2 = [l_y2 floatValue];
    
    switch (i) {
        case 1://y1 move
            m_sender_act.bh_Array_cmd[0].bh_function_code = 1;
            break;
        case 2://y2 move
            m_sender_act.bh_Array_cmd[0].bh_function_code = 1;
            break;
        case 3://ab rotate to plane
            m_sender_act.bh_Array_cmd[0].bh_function_code = 1;
            break;
        case 4://xz translation
            m_sender_act.bh_Array_cmd[0].bh_function_code = 1;
            break;
        case 5://xz safe move
            m_sender_act.bh_Array_cmd[0].bh_function_code = 1;
            break;
        case 6://loadcell press
            m_sender_act.bh_Array_cmd[0].bh_function_code = 1;
            break;
        case 7://jy down
            m_sender_act.bh_Array_cmd[0].bh_function_code = 1;
            break;
        case 8://jy rotate
            m_sender_act.bh_Array_cmd[0].bh_function_code = 1;
            break;
        case 9://read xz coorder
            m_sender_act.bh_Array_cmd[0].bh_function_code = 1;
            break;
        case 10://debuge for x move
            m_sender_act.bh_Array_cmd[0].bh_function_code = 1;
            break;
        case 11://debuge for z move
            m_sender_act.bh_Array_cmd[0].bh_function_code = 1;
            break;
        case 12://debuge for a rotate
            m_sender_act.bh_Array_cmd[0].bh_function_code = 1;
            break;
        case 13://debuge for b rotate
            m_sender_act.bh_Array_cmd[0].bh_function_code = 1;
            break;
        default:
            break;
    }
    long ret = [self m_SendCmd:m_sender_act];
    return ret;
}

-(long)mm_JY_Rotate:(int)act Value1:(ushort)v1 Value2:(ushort)v2{//act1:
    memset(&m_sender_act, 0, sizeof(g_SendBeckHoff));
    switch (act) {
        case 7:                                                 //jy down
            g_JYDownFlag = 0;
            m_sender_act.bh_Array_cmd[0].bh_function_code = 1;
            m_sender_act.bh_Array_cmd[0].bh_op_value1 = v1;
            m_sender_act.bh_Array_cmd[0].bh_op_value2 = v2;
            break;
        case 8:                                                 //jy rotate
            g_JYRotateFlag = 0;
            m_sender_act.bh_Array_cmd[0].bh_function_code = 1;
            m_sender_act.bh_Array_cmd[0].bh_op_value1 = v1;
            m_sender_act.bh_Array_cmd[0].bh_op_value2 = v2;
            break;
        default:
            break;
    }
    long ret = [self m_SendCmd:m_sender_act];
    return ret;
}

-(IBAction)JY_Act:(id)sender{
    [self mm_JY_Rotate:(int)[sender tag] Value1:[l_jy_v1 floatValue]*1000 Value2:[l_jy_v2 floatValue]*1000];
}

-(IBAction)m_Debug_Act:(id)sender{
    [self mm_Debug_Act:(int)[sender tag]];
}

@end
