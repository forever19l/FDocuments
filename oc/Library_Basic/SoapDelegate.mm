//
//  SoapDelegate.m
//  IANetFixture
//
//  Created by forever on 8/16/16.
//  Copyright © 2016 Louis. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreLib/TestContext.h>
#import "SoapDelegate.h"
@interface SoapDelegate ()

@end

@implementation SoapDelegate

@synthesize webData;
@synthesize soapResults;
@synthesize xmlParser;
@synthesize elementFound;
@synthesize matchingElement;
@synthesize conn;

-(int)PostXml:(NSString*)url Action:(NSString *)action Body:(NSString* )body TimeOut:(int) timeout
{
    // 设置我们之后解析XML时用的关键字，与响应报文中Body标签之间的getMobileCodeInfoResult标签对应
    //    matchingElement = @"getMobileCodeInfoResult";
    matchingElement = [CTestContext::m_dicGlobal valueForKey:@"matchingElement"];
    // 创建SOAP消息，内容格式就是网站上提示的请求报文的主体实体部分
       NSString *soapMsg = [NSString stringWithFormat:
                         @"<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                         "<soap12:Envelope xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
                         "xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" "
                         "xmlns:soap12=\"http://www.w3.org/2003/05/soap-envelope\">"
                         "<soap12:Header>"
                         "<Credentials xmlns=\"MESWebPortal\">"
                         "<key>%@</key>"
                         "</Credentials>"
                         "</soap12:Header>"
                         "<soap12:Body>"
                         "%@"
                         "</soap12:Body>"
                         "</soap12:Envelope>",[CTestContext::m_dicGlobal valueForKey:@"SFIP"],body];
    // 创建URL，内容是前面的请求报文报文中第二行主机地址加上第一行URL字段
    NSURL *strurl = [NSURL URLWithString: url];
    // 根据上面的URL创建一个请求
    NSLog(@"soapMsgsoapMsgsoapMsgsoapMsgsoapMsg==========%@",soapMsg);
    NSMutableURLRequest *req = [NSMutableURLRequest requestWithURL:strurl];
    req.timeoutInterval = timeout;
    NSString *msgLength = [NSString stringWithFormat:@"%lu", (unsigned long)[soapMsg length]];
    
    // 添加请求的详细信息，与请求报文前半部分的各字段对应
    //    [req addValue:@"webservice.webxml.com.cn" forHTTPHeaderField:@"Host"];
    [req addValue:[CTestContext::m_dicGlobal valueForKey:@"SFIP"]  forHTTPHeaderField:@"Host"];
    [req addValue:@"application/soap+xml; charset=utf-8" forHTTPHeaderField:@"Content-Type"];
    [req addValue:msgLength forHTTPHeaderField:@"Content-Length"];
    
    // 设置请求行方法为POST，与请求报文第一行对应
    [req setHTTPMethod:@"POST"];
    // 将SOAP消息加到请求中
    [req setHTTPBody: [soapMsg dataUsingEncoding:NSUTF8StringEncoding]];

//method 1:同步获取返回数据
    NSError * err = [[[NSError alloc ] init] autorelease];
    NSData *conn1 = [[[NSData alloc] init] autorelease];
    conn1 = [NSURLConnection sendSynchronousRequest:req returningResponse:nil error:&err];
    NSString *ret = [[[NSString alloc] initWithData:conn1 encoding:NSUTF8StringEncoding] autorelease];
    //    xmlParser = [[NSXMLParser alloc] initWithData: conn1];
    //    [xmlParser setDelegate: self];
    //    [xmlParser setShouldResolveExternalEntities: YES];
    //    [xmlParser parse];
    if ([ret containsString:matchingElement]) {
        return 1;
    }
    else{
        return -1;
    }
//method 2:异步获取返回数据
//    conn = [[NSURLConnection alloc] initWithRequest:req delegate:self];
//    if (conn) {
//        webData = [[NSMutableData alloc] init];
//        return 1;
//    }
//    else{
//        return -1;
//    }
}

-(NSString *) putSFC:(NSString*)url TimeOut:(int) timout
{
    NSURL * strURL = [NSURL URLWithString:[NSString stringWithUTF8String:url]];
    if (!strURL)
    {
        NSLog(@"Invalid URL : %@",[NSString stringWithUTF8String:url]);
        return @"Invalid URL";
    }
    
    NSURLRequest* request = [NSURLRequest requestWithURL:strURL cachePolicy:(NSURLRequestCachePolicy)0 timeoutInterval:timout];
    NSURLResponse* response=nil;
    NSError* error=nil;
    NSData* data=[NSURLConnection sendSynchronousRequest:request returningResponse:&response error:&error];
    if (data==nil) {
        return @"empty data";
    }
    NSString * str = [[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding] autorelease];
    // SaveDebug([NSString stringWithFormat:@"-----SFC RET=%@;",str],__func__,__LINE__);
    if (str==nil) {
        return @"empty str";
    }
    NSLog(@"SFC Response : %@",str);
    return str;
}

#pragma mark -
#pragma mark URL Connection Data Delegate Methods

// 刚开始接受响应时调用
-(void) connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *) response{
    [webData setLength: 0];
}

// 每接收到一部分数据就追加到webData中
-(void) connection:(NSURLConnection *)connection didReceiveData:(NSData *) data {
    [webData appendData:data];
}

// 出现错误时
-(void) connection:(NSURLConnection *)connection didFailWithError:(NSError *) error {
    NSLog(@"errroreooeroroeerror:%@",error);
    [conn release];
    [webData release];
}

// 完成接收数据时调用
-(void) connectionDidFinishLoading:(NSURLConnection *) connection {
    NSString *theXML = [[NSString alloc] initWithBytes:[webData mutableBytes]
                                                length:[webData length]
                                              encoding:NSUTF8StringEncoding];
    
    // 打印出得到的XML
    NSLog(@"xmlxmlxmlxmlxml=================:%@\n", theXML);
    // 使用NSXMLParser解析出我们想要的结果
    xmlParser = [[NSXMLParser alloc] initWithData: webData];
    [xmlParser setDelegate: self];
    [xmlParser setShouldResolveExternalEntities: YES];
    [xmlParser parse];
    
}

#pragma mark -

#pragma mark XML Parser Delegate Methods

// 开始解析一个元素名
-(void) parser:(NSXMLParser *) parser didStartElement:(NSString *) elementName namespaceURI:(NSString *) namespaceURI qualifiedName:(NSString *) qName attributes:(NSDictionary *) attributeDict {
    if ([elementName isEqualToString:matchingElement]) {
        if (!soapResults) {
            soapResults = [[NSMutableString alloc] init];
        }
        elementFound = YES;
    }

}

// 追加找到的元素值，一个元素值可能要分几次追加
-(void)parser:(NSXMLParser *) parser foundCharacters:(NSString *)string {
    if (elementFound) {
        [soapResults appendString: string];
    }
    
}

// 结束解析这个元素名
-(void)parser:(NSXMLParser *)parser didEndElement:(NSString *)elementName namespaceURI:(NSString *)namespaceURI qualifiedName:(NSString *)qName {
    if ([elementName isEqualToString:matchingElement]) {
        NSLog(@"jieshujieshujieshujiehsu================");
        NSAlert *alert = [ NSAlert alertWithMessageText:@"SOAP_Result" defaultButton:@"OK" alternateButton:nil otherButton:nil informativeTextWithFormat:[NSString stringWithFormat:@"%@", soapResults] ];
        [CTestContext::m_dicGlobal setValue:soapResults forKey:@"SOAP_Result"];
        [alert runModal];
        elementFound = FALSE;
        // 强制放弃解析
        [xmlParser abortParsing];
    }
}

// 解析整个文件结束后
- (void)parserDidEndDocument:(NSXMLParser *)parser {
    if (soapResults) {
        [soapResults release];
        [xmlParser release];
    }
}

// 出错时，或者强制结束解析
- (void) parser:(NSXMLParser *)parser parseErrorOccurred:(NSError *)parseError {
    if (soapResults) {
        [soapResults release];
        [xmlParser release];
    }
}

@end
