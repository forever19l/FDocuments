
#import "IASocketConnection.h"
#import "IASocketDelegate.h"


@interface IASocketConnection () <NSStreamDelegate>
@end

@implementation IASocketConnection



- (id)initWithHost:(const char*) host Port:(UInt32) port
{
    m_isOpen=NO;
    self = [super init];
    if (self != nil) {
        CFReadStreamRef readStream = NULL;
        CFWriteStreamRef writeStream = NULL;
        CFStringRef strhost = CFStringCreateWithCString(NULL,host, kCFStringEncodingASCII);
        CFStreamCreatePairWithSocketToHost(kCFAllocatorDefault, strhost,port,&readStream,&writeStream);
        CFRelease(strhost);
        if (readStream && writeStream)
        {
           m_inputStream = (__bridge NSInputStream *)readStream;
           m_outputStream = (__bridge NSOutputStream *)writeStream;
            [m_inputStream  setDelegate:self];
            [m_outputStream setDelegate:self];
            [m_inputStream  scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSRunLoopCommonModes];
            [m_outputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSRunLoopCommonModes];
        }else
        {
            [super dealloc];
            return nil;
        }
    }
    return self;
    
}
-(void)ClearInputBuffter
{
    while ([m_inputStream hasBytesAvailable] && [self isOpen])
    {
        [m_inputStream read:(uint8_t*)tmpbuffter maxLength:255];
    }
}
- (NSInteger)write:(const char*)buffer maxLength:(NSUInteger)len
{
    return [m_outputStream write:(const uint8_t *)buffer maxLength:len];
}
-(NSInteger)read:(char*)buffer maxLength:(unsigned int)nLen
{
    if([m_inputStream hasBytesAvailable])
    {
        return [m_inputStream read:(uint8_t *)buffer maxLength:nLen];
    }else
    {
        return 0;
    }
}
-(BOOL)read:(char*)buffer Lenght:(unsigned int)nLen Timeout:(NSUInteger)iTimeOut
{
    if(m_isOpen)
    {
        NSUInteger readed=0;
        NSUInteger read=nLen;
        NSTimeInterval starttime = [[NSDate date]timeIntervalSince1970];
        double tm = (double)iTimeOut/1000.0;
        while (YES) {
            if([m_inputStream hasBytesAvailable])
            {
                NSInteger n=[m_inputStream read:(uint8_t *)buffer+readed maxLength:read];
                readed=readed+n;
                read=nLen-readed;
                if (read==0) {
                    return YES;
                }
                if (n==0) {
                    [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate date]];
                    [NSThread sleepForTimeInterval:0.1];
                }
            }else
            {
                [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate date]];
                [NSThread sleepForTimeInterval:0.1];
            }
            NSTimeInterval now = [[NSDate date]timeIntervalSince1970];
            if ((now-starttime)>=tm)
            {
                return NO;       //time out
            }
        }
        return NO;
    }
    return NO;
}
- (NSInputStream*)InputStream
{
    return m_inputStream;
}
- (NSOutputStream*)OutputStream
{
    return m_outputStream;
}
- (BOOL)open:(double)vTimeout
{
    [m_inputStream  open];
    [m_outputStream open];
    if (vTimeout>0)
    {
        myTimer=[NSTimer scheduledTimerWithTimeInterval:vTimeout target:self selector:@selector(timerFireMethod:) userInfo:self repeats:NO];
    }
    return YES;
}
- (void)timerFireMethod:(NSTimer *)timer {
    if (myTimer!=nil) {
        [myTimer invalidate];
        myTimer = nil;
    }
    [self close];
    if (delegate!=nil)
    {
        if (isConnectTimeoutdelegate)
        {
            [delegate ConnectTimeout:timer Socket:self];
        }else
        {
            NSLog(@"Connect Timeout");
        }
    }else
    {
        NSLog(@"Connect Timeout");
    }
}

-(BOOL)isOpen
{
    return m_isOpen;
}
- (void)close {
    
    if (m_inputStream!=nil)
    {
        [m_inputStream  removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        [m_outputStream removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        [m_inputStream  setDelegate:nil];
        [m_outputStream setDelegate:nil];
        [m_inputStream  close];
        [m_outputStream close];
        [m_inputStream release];
        [m_outputStream release];
        m_inputStream=nil;
        m_outputStream=nil;
        m_isOpen=NO;
    }

}

- (id <NSStreamDelegate>)delegate
{
    return delegate;
}
- (void)setDelegate:(id)newDelegate
{
	id old = nil;
	
	if (newDelegate != delegate) {
		old = delegate;
		delegate = [newDelegate retain];
		[old release];
        isConnectTimeoutdelegate=[delegate respondsToSelector:@selector(ConnectTimeout:Socket:)];
        isdatareadydelegate=[delegate respondsToSelector:@selector(dataready:handleEvent:)];
        isstreamerrordelegate=[delegate respondsToSelector:@selector(streamerror:handleEvent:)];
        isopencompleteddelegate=[delegate respondsToSelector:@selector(opencompleted:handleEvent:)];
	}
}


-(BOOL)Senddata:(NSData*)data
{
    if(m_isOpen)
    {
       [m_outputStream write:(const uint8_t *)[data bytes] maxLength:[data length]];
        return YES;
    }
    return NO;
}

-(int)WaitOpen:(int)iTimeOut
{
    BOOL bTrigger= NO;
    m_onWaiting=YES;
    NSTimeInterval starttime = [[NSDate date]timeIntervalSince1970];
    double tm = (double)iTimeOut/1000.0;
    while (1) {
        NSTimeInterval now = [[NSDate date]timeIntervalSince1970];
        if ((now-starttime)>=tm)
        {
            m_onWaiting=NO;
            return -1;       //time out
        }
        
        if ([[NSThread currentThread] isCancelled])
        {
            m_onWaiting=NO;
            return -2;
        }
        if (m_onWaiting==NO) {
            bTrigger=0;
            break;
        }
        if (m_isOpen)
        {
            bTrigger=0;
            break;
        }
        [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate date]];
        [NSThread sleepForTimeInterval:0.01];
    }
    m_onWaiting=NO;
    return bTrigger;
}
- (void)stream:(NSStream *)aStream handleEvent:(NSStreamEvent)streamEvent {
    switch(streamEvent)
    {
        case NSStreamEventHasBytesAvailable:
            if (delegate!=nil) {
                if (isdatareadydelegate) {
                    [delegate dataready:aStream handleEvent:streamEvent];
                }
            }
            break;
        case NSStreamEventEndEncountered:
        case NSStreamEventErrorOccurred:
        {
            if (myTimer!=nil) {
                [myTimer invalidate];
                myTimer = nil;
            }
            if (delegate!=nil) {
                
                if (isdatareadydelegate)
                {
                    [delegate streamerror:aStream handleEvent:streamEvent];
                }
            }
        } break;
        case NSStreamEventHasSpaceAvailable:
            break;
        case NSStreamEventOpenCompleted:
            if (myTimer!=nil) {
                [myTimer invalidate];
                myTimer = nil;
            }
            m_isOpen=YES;
            if (delegate!=nil) {
                if (isdatareadydelegate) {
                    [delegate opencompleted:aStream handleEvent:streamEvent];
                }
            }
            break;
        default: {
            // do nothing
        } break;
    }
}

@end
