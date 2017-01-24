//
//  SerialPortView.h
//  iTMP
//
//  Created by Ryan on 12-2-14.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

#include "SerialportEx.h"

typedef enum{
    CR_LF = 0,
    CR,
    LF,
    } Enter_Key_Emulation ;

@interface SerialPortView : NSTextView<SerialView> {
@private
    BOOL bEnableEcho;
    BOOL bHexMode;
    BOOL AutoTimeStamp;
    SerialportEx * SerialPort;
    
//    NSThread * readThread;
    NSMutableString * strBuf;
    Enter_Key_Emulation keyEnter;
}

-(void)EnableEcho:(BOOL)bEnable;
-(void)SetHexMode:(BOOL)HexMode;
-(void)EmptySerialPortView;
-(void)InsertData:(NSData *)data;
-(void)AttachSerialPort:(SerialportEx *)sp;
-(SerialportEx *)SerialPort;

-(void)ReadData:(id)sender;

-(int)SaveToFile:(NSString *)strfilename;
@property Enter_Key_Emulation keyEnter;
@property (readwrite,getter = isTimeStamp) BOOL AutoTimeStamp;
@end
