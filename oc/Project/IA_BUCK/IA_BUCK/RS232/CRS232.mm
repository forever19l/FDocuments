//
//  CRS232.cpp
//  RS232
//
//  Created by IvanGan on 16/4/9.
//  Copyright © 2016年 IvanGan. All rights reserved.
//

#include "CRS232.h"



#include <iostream>
#include <sstream>

#include <unistd.h>

#include <IOKit/serial/IOSerialKeys.h>

NSLock * g_LockCB = [[NSLock alloc]init];

CRS232::CRS232()
{
    pthread_mutex_init(&m_mutex,nil);
    pthread_mutex_init(&m_lockOperate, nil);
    m_strBuffer = [[NSMutableString alloc]init];
    m_DataBuffer = [[NSMutableData alloc]init];
    m_strDetect = [[NSMutableString alloc]init];
    m_Return = [[NSMutableString alloc]init];
    
    m_notificationInfo = [[NSMutableString alloc]init];
    m_notificationName = [[NSMutableString alloc]init];
    
    txtView = nil;
    iTimeout = 3000;
}

CRS232::~CRS232()
{
    pthread_mutex_destroy(&m_mutex);
    pthread_mutex_destroy(&m_lockOperate);
    [m_strDetect release];
    [m_strBuffer release];
    [m_DataBuffer release];
    [m_notificationName release];
    [m_notificationInfo release];
    [m_Return release];
}

void CRS232::SetNotificationInfo(const char * detInfo, const char * notificationName)
{
    [m_notificationInfo setString:[NSString stringWithUTF8String:detInfo]];
    [m_notificationName setString:[NSString stringWithUTF8String:notificationName]];
}

void CRS232::AttachehView(NSTextView * view)
{
    txtView = view;
}

void CRS232::AddText(NSString * str)
{
    if(txtView != NULL)
    {
        NSDateFormatter * fmt = [[NSDateFormatter alloc]init];
        [fmt setDateFormat:@"yyyy/MM/dd HH:mm:ss.SSS : "];
        NSString * date = [fmt stringFromDate:[NSDate date]];
        [fmt release];
        NSAttributedString * attrStr = [[NSAttributedString alloc] initWithString:[NSString stringWithFormat:@"%@%@", date, str]];
        [txtView.textStorage appendAttributedString:attrStr];
        [attrStr release];
    }
}

void * CRS232::didReceiveData(void * data, long len)
{
    if(len < 0)
        return nullptr;
    pthread_mutex_lock(&m_mutex);
   
    [m_DataBuffer appendBytes:(Byte*)data length:len];
    char * p = (char *)data;
    for(int i=0; i<len; i++)
    {
        if(p[i] == '\0')
            p[i] = '.';
    }
    NSString * str = [[NSString alloc]initWithBytes:data length:len encoding:NSUTF8StringEncoding];
    if(str)
    {
       [m_strBuffer appendString:str];
        AddText(str);
    }
    [str release];
    
    if([m_notificationInfo length]>0 && [m_strBuffer containsString:m_notificationInfo])
    {
        [[NSNotificationCenter defaultCenter]postNotificationName:m_notificationName object:nil userInfo:nil];
        [m_strBuffer setString:@""];
    }
    pthread_mutex_unlock(&m_mutex);
    return nullptr;
}


int CRS232::Open(const char * dev, const char * opt)//opt:"19200,8,n,1"
{
    //int set_opt(int nSpeed, int nBits, char nEvent, int nStop);
    NSString * str = [NSString stringWithUTF8String:opt];
    NSLog(@"%@",str);
    NSArray * arr = [str componentsSeparatedByString:@","];
    int nSpeed = 19200;
    int nBits = 8;
    char nEvent = 'n';
    int nStop = 1;
    if([arr count]==4)
    {
        nSpeed = [[arr objectAtIndex:0]intValue];
        nBits = [[arr objectAtIndex:1]intValue];
        const char * tmp = [[arr objectAtIndex:2]UTF8String];
        nEvent = tmp[0];
        nStop = [[arr objectAtIndex:3]intValue];
    }
    int ret = CSerialPort::connect(dev);
    if (ret<0)
    {
        return ret;
    }
    ret = CSerialPort::set_opt(nSpeed, nBits, nEvent, nStop);
    return ret;
}

int CRS232::WriteString(const char * buffer)
{
    NSString * str = [NSString stringWithFormat:@"%s", buffer];
    
    return CSerialPort::write((void*)[str UTF8String], [str length]);
}

int CRS232::WriteBytes(unsigned char * ucData, int len)
{
        return CSerialPort::write((void*)ucData, len);
}

int CRS232::WriteStringBytes(const char * szData)//"0xFF,0xFE,..."
{
    if(szData == NULL) return -1;
    if(strlen(szData)<=0) return -2;
    NSArray * arr = [[NSString stringWithUTF8String:szData] componentsSeparatedByString:@","];
    if([arr count]< 1) return -3;
    int size = [arr count];
    unsigned char * ucData = new unsigned char [size];
    for(int i=0; i<size; i++)
    {
        NSScanner * scanner = [NSScanner scannerWithString:[arr objectAtIndex:i]];
        unsigned int tmp;
        [scanner scanHexInt:&tmp];
        ucData[i] = tmp;
    }
   
    CSerialPort::write((void*)ucData, size);
    return 0;
}

void CRS232::ClearBuffer()
{
    [m_strBuffer setString:@""];
}

const char * CRS232::ReadString()
{
    [NSThread sleepForTimeInterval:0.01];
    pthread_mutex_lock(&m_mutex);
    if(m_strBuffer && [m_strBuffer length]>0)
    {
        [m_Return setString:m_strBuffer];
        [m_strBuffer setString:@""];
    }
    else
        [m_Return setString:@""];
    [m_DataBuffer setLength:0];
    pthread_mutex_unlock(&m_mutex);
    return [m_Return UTF8String];
}

int CRS232::WaitDetect(int timeout)
{
    int r = -1;
    //    NSLog(@" * * * * * \ndylib Detect :%@ * * * * * \n",m_strStringToDetect);
    NSTimeInterval starttime = [[NSDate date]timeIntervalSince1970];
    double tm = (double)timeout/1000.0;
    NSLog(@"starting to wait : %@",m_strDetect);
    while (1)
    {
        NSTimeInterval now = [[NSDate date]timeIntervalSince1970];
        if ((now-starttime)>=tm)
        {
            r = -2;
            break;
        }
        
        pthread_testcancel();       //if is cancel,exist current loop.
        
        //if([m_strBuffer containsString:m_MutableDetect])
        pthread_mutex_lock(&m_mutex);
        NSRange range  = [m_strBuffer rangeOfString:m_strDetect];
        pthread_mutex_unlock(&m_mutex);
        
        if (range.location != NSNotFound)
        {
            r = 0;
            break;
        }
        
        [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate date]];
        [NSThread sleepForTimeInterval:0.01];
    }
    
    NSLog(@"waiting finished : %d",r);
    return r;  //cancel
}

int CRS232::SetDetectString(const char * det)
{
    [m_strDetect setString:[NSString stringWithUTF8String:det]];
    return 0;
}

const char * CRS232::WriteReadString(const char * buffer,int timeout)
{
    [NSThread sleepForTimeInterval:1];
    pthread_mutex_lock(&m_lockOperate);
    WriteString(buffer);
    WaitDetect(timeout);
    NSString * str = [NSString stringWithUTF8String:ReadString()];
    pthread_mutex_unlock(&m_lockOperate);
    return [str UTF8String];
}

int CRS232::Close()
{
    
    CSerialPort::close();
    return 0;
}







