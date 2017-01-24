/*TCP连接的基础类*/

#import <Foundation/Foundation.h>

@interface IASocketConnection : NSObject
{
    NSTimer *myTimer;
    char tmpbuffter[255];
    dispatch_semaphore_t signal;
    BOOL m_onWaiting;
    BOOL m_isOpen;
    SEL m_SocketCallBack;
    id  m_CallbackTarget;
    NSInputStream *    m_inputStream;
    NSOutputStream *   m_outputStream;
    id delegate;
    
    BOOL isConnectTimeoutdelegate;
    BOOL isdatareadydelegate;
    BOOL isstreamerrordelegate;
    BOOL isopencompleteddelegate;
    

}
- (NSInputStream*)InputStream;
- (NSOutputStream*)OutputStream;
- (id <NSStreamDelegate>)delegate;
- (void)setDelegate:(id <NSStreamDelegate>)delegate;
-(BOOL)Senddata:(NSData*)data;
- (id)initWithHost:(const char*) host Port:(UInt32) port;
-(BOOL)read:(char*)buffer Lenght:(unsigned int)nLen Timeout:(NSUInteger)iTimeOut;
-(NSInteger)read:(char*)buffer maxLength:(unsigned int)nLen;
- (NSInteger)write:(const char*)buffer maxLength:(NSUInteger)len;
-(BOOL)isOpen;
- (BOOL)open:(double)vTimeout;
- (void)close;
-(void)ClearInputBuffter;

@end

@protocol IASocketConnection <NSObject>
@optional
- (void)dataready:(NSStream *)aStream handleEvent:(NSStreamEvent)eventCode;
- (void)streamerror:(NSStream *)aStream handleEvent:(NSStreamEvent)eventCode;
- (void)opencompleted:(NSStream *)aStream handleEvent:(NSStreamEvent)eventCode;
- (void)ConnectTimeout:(NSTimer *)timer Socket:(IASocketConnection*)socket;
@end