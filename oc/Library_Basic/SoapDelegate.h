//
//  SoapDelegate.h
//  IANetFixture
//
//  Created by forever on 8/16/16.
//  Copyright © 2016 Louis. All rights reserved.
//
/*HTTP基础类，soap方式发生xml格式内容，直接Request请求发送url。。。*/

#import <AppKit/AppKit.h>

@interface SoapDelegate : NSObject <NSXMLParserDelegate, NSURLConnectionDelegate>

@property (strong, nonatomic) NSMutableData *webData;
@property (strong, nonatomic) NSMutableString *soapResults;
@property (strong, nonatomic) NSXMLParser *xmlParser;
@property (nonatomic) BOOL elementFound;
@property (strong, nonatomic) NSString *matchingElement;
@property (strong, nonatomic) NSURLConnection *conn;

-(int)PostXml:(NSString*)url Action:(NSString *)action Body:(NSString* )body TimeOut:(int) timeout;
-(NSString *) putSFC:(NSString*)url TimeOut:(int) timout;
@end