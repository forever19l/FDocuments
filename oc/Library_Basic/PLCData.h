//
//  IADataConnection.h
//  AINetFixture
//
//  Created by ZL-Pro on 16/1/12.
//  Copyright (c) 2016年 Louis. All rights reserved.
//
/*TCP实用化子类，与PLCAct结合，分管数据*/

#import "IASocketConnection.h"
#import "ColorButtonView.h"
#import "CommandStruct.h"
#import "PLCChartView.h"
#import "zipAndUnzip.h"
#include <CoreLib/TestContext.h>

@interface PLCData: NSObject<NSStreamDelegate>
{
    BOOL is_Debug;
    
    IBOutlet ColorButtonView* m_socketstatue;
    IASocketConnection* m_socketData;
    unsigned long long m_pipedatalen;
    g_PLCStruct m_plcsave;

 @private
    unsigned int time_startpoint;
    g_ReadBeckHoff m_tmpReadCmd;
    int plcreadflag;

    IBOutlet NSTextView *m_textView;
    IBOutlet NSTextField *m_tfIP;
    IBOutlet NSButton *m_btConnect;
    IBOutlet NSButton *m_btClose;
    
    IBOutlet PLCIAChartView *g_PLCdata_ui;
    IBOutlet PLCIAChartView *m_drawPLCView;
    IBOutlet NSTextField *m_dataname;
    IBOutlet NSButton *m_datasave;
}
-(void)clearData;
-(IBAction)bt_onConnect:(id)sender;
-(IBAction)bt_onClose:(id)sender;
-(void)SocketStatus:(NSNumber*)number;

@property (assign) IBOutlet NSWindow *window;

-(BOOL)isOpen;
-(void)close;
-(BOOL)ConnectWithIP:(const char*)ip;
-(void)ClearInBuffer;
-(long)SendCmd:(g_SendBeckHoff) cmd;
-(long)ReadData:(g_ReadBeckHoff*) reader MaxLenght:(unsigned int)maxLenght;

//-(int)PLCDataRcv;
-(int)PLCDataDraw;

@end
