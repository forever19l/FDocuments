//
//  SerialPortView.m
//  iTMP
//
//  Created by Ryan on 12-2-14.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#import "SerialPortView.h"
//#include "luaGlobal.h"
#import "RegexKitLite.h"

#define VK_UP     (Byte[]){0x1b,0x5b,0x41}
#define VK_DOWN     (Byte[]){0x1b,0x5b,0x42}
#define VK_RIGHT     (Byte[]){0x1b,0x5b,0x43}
#define VK_LEFT     (Byte[]){0x1b,0x5b,0x44}

@implementation SerialPortView
@synthesize keyEnter,AutoTimeStamp;

- (id)init
{
    self = [super init];
    if (self) {
        // Initialization code here.
        bEnableEcho = YES;
        bHexMode = NO;
        self.AutoTimeStamp = YES;
        self.keyEnter = CR_LF;
    }
    
    return self;
}
-(id)initWithCoder:(NSCoder *)aDecoder
{
   self = [super initWithCoder:aDecoder];
    if (self) {
        bEnableEcho = YES;
        bHexMode = NO;
        strBuf = [[NSMutableString alloc ]init];
//        [self init];
    }
    return self;
}

-(void)ReadData:(id)sender
{
    while (1) {
        int bytes = [SerialPort bytesAvailable];
        if  (bytes>0)
        {
            NSData * data = [SerialPort readBytes:bytes error:NULL]; 
            NSString *text = [[NSString alloc] initWithData:data encoding:NSASCIIStringEncoding];
            [self insertText:text];
            [text release];
//            Delay(100); 
            [self setNeedsDisplay:YES];
            [self displayIfNeeded];
        }
    }

}
- (void)dealloc
{
    [strBuf release];
    [super dealloc];
}

-(void)EnableEcho:(BOOL)bEnable
{
    bEnableEcho = bEnable;
}

-(void)SetHexMode:(BOOL)HexMode
{
    bHexMode = HexMode;

}

-(void)InsertData:(NSData *)data
{
    if ([data length] > 0) {
        if (bHexMode)
        {
            Byte * pvalue = (Byte *)[data bytes];
            for (int i=0; i<[data length]; i++) {
                [self insertText:[NSString stringWithFormat:@"%02X ",pvalue[i]]];
            }
            //[self insertText:@"\r\n"];
        }
        else
        {
            //Byte * pdata = (Byte *)[data bytes];
            if ([data isEqualToData:[NSData dataWithBytes:(Byte []){0x08,0x20,0x08} length:3]])
            {
                [self deleteBackward:self];
            }
            else if([data isEqualToData:[NSData dataWithBytes:(Byte []){0x1b,0x5b,0x44} length:3]])
            {
                [self moveLeft:self];
            }
            else if([data isEqualToData:[NSData dataWithBytes:(Byte []){0x1b,0x5b,0x43} length:3]])
            {
                [self moveRight:self];
            }
            else
            {
#if 0
                NSString *text = [[NSString alloc] initWithData:data encoding:NSASCIIStringEncoding];
                
                //insert Time stamp
                NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
                [dateFormatter setDateFormat:@"\nyyyy/MM/dd HH:mm:ss.SSS : "];
                NSString * str = [dateFormatter stringFromDate:[NSDate date]];
//                NSString * buf = [text stringByReplacingOccurrencesOfString:@"\r\n" withString:str];
//                [text release];
                [self insertText:text];
                [strBuf appendString:text];
                [text release];
#else
                NSMutableString * text=[[NSMutableString alloc]initWithData:data encoding:NSASCIIStringEncoding];
                if (!text) return;
                
//                if (self.isTimeStamp)
                {
                    //insert Time stamp
                    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
                    [dateFormatter setDateFormat:@"yyyy/MM/dd HH:mm:ss.SSS : \t"];
                    NSString * str = [dateFormatter stringFromDate:[NSDate date]];
                    if ([[self string]length]==0)
                    {
                        //[self insertText:str];
                        NSAttributedString * theString = [[NSAttributedString alloc] initWithString:[NSString stringWithFormat:@"%@",text]];
                        [[self textStorage] appendAttributedString: theString];
                        NSUInteger length = [[self textStorage] length];
                        NSRange theRange = NSMakeRange(length, 0);
                        [self scrollRangeToVisible:theRange];
                        [theString release];
                    }
                    str = [NSString stringWithFormat:@"\n%@",str];
                    [text replaceOccurrencesOfRegex:@"[\r\n]+" withString:str];
                    [dateFormatter release];
                }
//                [self insertText:text];
//                [strBuf appendString:text];
                NSAttributedString * theString = [[NSAttributedString alloc] initWithString:[NSString stringWithFormat:@"%@",text]];
                [[self textStorage] appendAttributedString: theString];
                NSUInteger length = [[self textStorage] length];
                NSRange theRange = NSMakeRange(length, 0);
                [self scrollRangeToVisible:theRange];
                [theString release];
                
                [text release];
#endif
            }
        }
    }
}

-(void)AttachSerialPort:(SerialportEx *)sp
{
    id old = nil;
    if (sp!=SerialPort) {
        old  =SerialPort;
        SerialPort = [sp retain];
        [old release];
    }
}

-(SerialportEx *)SerialPort
{
    return SerialPort;
}

-(int)SaveToFile:(NSString *)strfilename
{
   return [[self string] writeToFile:strfilename atomically:YES encoding:NSASCIIStringEncoding error:nil];
}


-(void)keyUp:(NSEvent *)theEvent
{
    unsigned short keycode = [theEvent keyCode];
    NSLog(@"keycode : %d",keycode);
    NSString *characters = [theEvent characters];
    if (keycode==123)  //VK_LEFT
    {
        [SerialPort writeData:[NSData dataWithBytes:VK_LEFT length:3] error:nil];
        return;
    }
    else if (keycode == 124) //VK_RIGHT
    {
        [SerialPort writeData:[NSData dataWithBytes:VK_RIGHT length:3] error:nil];
        return;

    }
    else if (keycode == 125) //VK_DOWN
    {
        [SerialPort writeData:[NSData dataWithBytes:VK_DOWN length:3] error:nil];
        return;

    }
    else if (keycode == 126)
    {
        [SerialPort writeData:[NSData dataWithBytes:VK_UP length:3] error:nil];
        return;
    }
    if (keycode == 51) characters = @"\b";
    
//    if ([characters isEqualToString:@"\r"]&&bReturnWithNewline) characters = @"\r\n";
        if ([characters isEqualToString:@"\r"])
        {
            switch (self.keyEnter) {
                case CR_LF:
                    characters = @"\r\n";
                    break;
                case CR:
                    characters = @"\r";
                    break;
                case LF:
                    characters = @"\n";
                    break;
                    
                default:
                    break;
            }
        }

    [SerialPort writeString:characters usingEncoding:NSASCIIStringEncoding error:NULL];
}

-(void)keyDown:(NSEvent *)theEvent
{
    if (bEnableEcho)
        [super keyDown:theEvent];
}
@end