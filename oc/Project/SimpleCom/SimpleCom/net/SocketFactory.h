//
//  IASocket.h
//  IANetFixture
//
//  Created by Yuekie on 7/5/16.
//  Copyright (c) 2016 Louis. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "IASocketConnection.h"
#import "SimpleComFactory.h"

@interface SocketFactory : SimpleComFactory<NSStreamDelegate,IASocketConnection>
{
    SimpleComSetting *m_simpleComSetting;
    
    NSData * m_strSerialWait;
    
    uint8_t m_tmpbuffer[1024];
    IASocketConnection* m_socket;
    pthread_mutex_t m_socketmutex;
    pthread_mutex_t m_SendCmdmutex;
    NSMutableData *m_Socketinputdata;
    ColorButtonView *m_statusview;
    
    NSMutableString* m_UartLog;

@public
    bool m_isFixtureStart;
}

//- (void)lock;
//- (void)unlock;

-(BOOL)CheckResourceName:(NSString*)Resource;
-(id)initwithName:(SimpleComSetting*)simpleComSetting;
-(void)SetSimpleComSetting:(SimpleComSetting*)simpleComSetting;
-(SimpleComSetting*)GetSimpleComSetting;
-(BOOL)OpenSimpleCom;

-(bool)WriteString:(const char *)szCMD;
-(void)AddLabelString:(const char *)LabelStr;
-(const char*)ReadString;
-(const char *)ReadStringRegex:(const char*)Regex Timeout:(int)timeout;
-(int)SetDetectString:(const char *)regex;
-(int)WaitForString:(int) iTimeout;
-(void)ClearBuffer;

- (BOOL)connectSocket:(NSString*) host Port:(UInt32) port timeOut:(double)timeout;
- (BOOL)isOpen;
- (void)closeSimpleCom;

- (void)dataready:(NSStream *)aStream handleEvent:(NSStreamEvent)eventCode;
- (void)streamerror:(NSStream *)aStream handleEvent:(NSStreamEvent)eventCode;
- (void)opencompleted:(NSStream *)aStream handleEvent:(NSStreamEvent)eventCode;
- (void)ConnectTimeout:(NSTimer *)timer Socket:(IASocketConnection*)socket;




//-(bool)GetFixtureState;

@end
