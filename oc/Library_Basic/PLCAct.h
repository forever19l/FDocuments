//
//  IADataConnection.h
//  AINetFixture
//
//  Created by ZL-Pro on 16/1/12.
//  Copyright (c) 2016年 Louis. All rights reserved.
//
/*TCP连接实用化的子类，与PLCData结合，分管动作*/

#import "IASocketConnection.h"
#import "ColorButtonView.h"
#import "CommandStruct.h"

@interface PLCAct:NSObject<NSStreamDelegate>
{
    BOOL is_Stop;
    IASocketConnection* m_socketRec;
@private
    pthread_mutex_t plcAct_mutex;

    NSPipe *m_inputpipe;
    unsigned long long m_pipedatalen;
    g_ReadBeckHoff m_tmpBHread;     //for buffer
    g_ReadBeckHoff m_reader_act;    //for read function
    g_SendBeckHoff m_sender_act;    //for send function
    IBOutlet NSTextView *m_textView;
    IBOutlet NSTextField *m_tfIP;
    IBOutlet NSButton *m_btConnect;
    IBOutlet NSButton *m_btClose;
    IBOutlet ColorButtonView* m_socketstatue;

    //debug 需添加
    IBOutlet NSTextField *l_x;      //x coorder
    IBOutlet NSTextField *l_z;      //z coorder
    IBOutlet NSTextField *l_y1;     //axis platform y1
    IBOutlet NSTextField *l_y2;     //axis platform y2
    IBOutlet NSTextField *l_a1;     //left a axis
    IBOutlet NSTextField *l_b1;     //left b axis
    IBOutlet NSTextField *l_a2;     //right a axis
    IBOutlet NSTextField *l_b2;     //right b axis
    IBOutlet NSTextField *l_jy_v1;
    IBOutlet NSTextField *l_jy_v2;
}
-(IBAction)bt_onConnect:(id)sender;
-(IBAction)bt_onClose:(id)sender;

-(void)SocketStatus:(NSNumber*)number;

@property (assign) IBOutlet NSWindow *window;

-(BOOL)isOpen;
-(void)close;
-(BOOL)ConnectWithIP:(const char*)ip;
-(long)SendCmd:(g_SendBeckHoff)cmd;
-(long)ReadPlcActCmd:(g_ReadBeckHoff)reader Timeout:(unsigned int)iTimeOut;

-(long)ReadActRecv:(unsigned int)itimeout;
-(long)ReadActDone:(unsigned int)itimeout;
-(long)m_SendCmd:(g_SendBeckHoff)cmd;   //read recv and done after send
-(long)mm_JY_Rotate:(int)act Value1:(ushort)v1 Value2:(ushort)v2;

-(void)lock;
-(void)unlock;

@end
