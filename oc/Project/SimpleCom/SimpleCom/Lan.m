//
//  Serial.m
//  Serial
//
//  Created by Liu Liang on 14-1-5.
//  Copyright (c) 2014年 Liu Liang. All rights reserved.
//

#import "Lan.h"

@implementation Lan

- (id)initWithInstr: (char*)ipAddr
{
    defaultRM = 0;
    instr = 0;
    writeCount = 0;
    retCount = 0;
    ViStatus status = 0;
    self = [super init];
    if (self)
    {
        status=viOpenDefaultRM (&defaultRM);
        if (status < VI_SUCCESS)
        {
            NSLog(@"viOpenDefaultRM returned %d", status);
            [self release];
            return nil;
        }
        //status = viOpen (defaultRM, "ASRL1::INSTR", VI_NULL, VI_NULL, &instr);
        //status = viOpen (defaultRM, "TCPIP0::192.168.2.123::inst0::INSTR", VI_NULL, VI_NULL, &instr);
        status = viOpen (defaultRM, ipAddr, VI_NULL, VI_NULL, &instr);
        if (status < VI_SUCCESS)
        {
            NSLog(@"viOpen returned %d", status);
            NSLog(@"ViOpen Failed when try to open the ethernet interface to %@",[NSString stringWithCString:ipAddr  encoding:NSUTF8StringEncoding]);
            viClose (defaultRM);
            [self release];
            return nil;
        }
        
        memset(RecvBuffer, 0, sizeof(RecvBuffer));
    }
    /*
     * At this point we now have a session open to the Ethernet simplecom.
     * Now we need to configure the serial port:
     */
    
    /* Set the timeout to 5 seconds (5000 milliseconds). */
    status = viSetAttribute (instr, VI_ATTR_TMO_VALUE, 5000);
    
    /* Specify that the read operation should terminate when a termination
     * character is received.
     */
    status = viSetAttribute (instr, VI_ATTR_TERMCHAR_EN, VI_TRUE);
    
    /* Set the termination character to 0xA
     */
    status = viSetAttribute (instr, VI_ATTR_TERMCHAR, 0xA);
    
    viClear(instr);
    
    return self;
}

- (void)clear
{
    viClear(instr);
}

- (void)setTimeout:(int)timemout
{
    viSetAttribute (instr, VI_ATTR_TMO_VALUE, timemout);
}

- (void)setDetectChar:(char)c
{
    viSetAttribute (instr, VI_ATTR_TERMCHAR_EN, VI_TRUE);
    viSetAttribute (instr, VI_ATTR_TERMCHAR, c);
}

- (int)send:(char*)cmd
{
    //strcpy (stringinput,"*IDN?\n");
    //status = viWrite (instr, (ViBuf)stringinput, (ViUInt32)strlen(stringinput), &writeCount);
    ViStatus status = 0;
    status = viWrite (instr, (ViBuf)cmd, (ViUInt32)strlen(cmd), &writeCount);
    if (status < VI_SUCCESS)
    {
        NSLog(@"Error for writting device.\n");
        return -1;
    }
    return 0;
}
- (NSString*)recv
{
    ViStatus status = 0;
//    char buffer[LAN_RECV_BUFFER_SIZE];
//    memset(buffer, 0, sizeof(buffer));
    memset(RecvBuffer, 0, sizeof(RecvBuffer));
    status = viRead (instr, (ViPBuf)RecvBuffer, LAN_RECV_BUFFER_SIZE, &retCount);
//    status = viRead (instr, (ViPBuf)buffer, LAN_RECV_BUFFER_SIZE, &retCount);
    if (status < VI_SUCCESS)
    {
        NSLog(@"Error for reading response from device.\n");
        return nil;
    }
    else
    {
//        NSString* str = [NSString stringWithUTF8String:buffer];
        NSString* str = [NSString stringWithUTF8String:RecvBuffer];
        return str;
    }
}

- (void)dealloc
{
    viClose (instr);
    viClose (defaultRM);
    [super dealloc];
}
@end
