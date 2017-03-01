//
//  Serial.m
//  Serial
//
//  Created by Liu Liang on 14-1-5.
//  Copyright (c) 2014年 Liu Liang. All rights reserved.
//

#import "Serial.h"

@implementation Serial

- (id)initWithInstr:(char*)instr_name rate:(int)rate bits:(int)bits parity:(int)parity stop:(int)stop
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
        status = viOpen (defaultRM, instr_name, VI_NULL, VI_NULL, &instr);
        if (status < VI_SUCCESS)
        {
            NSLog(@"viOpen returned %d", status);
            viClose (defaultRM);
            [self release];
            return nil;
        }
    }
    /*
     * At this point we now have a session open to the serial simplecom.
     * Now we need to configure the serial port:
     */
    
    /* Set the timeout to 5 seconds (5000 milliseconds). */
    status = viSetAttribute (instr, VI_ATTR_TMO_VALUE, 2000);
    
    /* Set the baud rate to 4800 (default is 9600). */
    status = viSetAttribute (instr, VI_ATTR_ASRL_BAUD, rate);
    
    /* Set the number of data bits contained in each frame (from 5 to 8).
     * The data bits for  each frame are located in the low-order bits of
     * every byte stored in memory.
     */
    status = viSetAttribute (instr, VI_ATTR_ASRL_DATA_BITS, bits);
    
    /* Specify parity. Options:
     * VI_ASRL_PAR_NONE  - No parity bit exists,
     * VI_ASRL_PAR_ODD   - Odd parity should be used,
     * VI_ASRL_PAR_EVEN  - Even parity should be used,
     * VI_ASRL_PAR_MARK  - Parity bit exists and is always 1,
     * VI_ASRL_PAR_SPACE - Parity bit exists and is always 0.
     */
    status = viSetAttribute (instr, VI_ATTR_ASRL_PARITY, parity);
    
    /* Specify stop bit. Options:
     * VI_ASRL_STOP_ONE   - 1 stop bit is used per frame,
     * VI_ASRL_STOP_ONE_5 - 1.5 stop bits are used per frame,
     * VI_ASRL_STOP_TWO   - 2 stop bits are used per frame.
     */
    status = viSetAttribute (instr, VI_ATTR_ASRL_STOP_BITS, stop);//10
    
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
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));
    status = viRead (instr, (ViPBuf)buffer, 100, &retCount);
    if (status < VI_SUCCESS)
    {
        NSLog(@"Error for reading response from device.\n");
        return nil;
    }
    else
    {
        NSString* str = [NSString stringWithUTF8String:buffer];
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
