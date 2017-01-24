//
//  CRS232.h
//  RS232
//
//  Created by IvanGan on 16/4/9.
//  Copyright © 2016年 IvanGan. All rights reserved.
//

#ifndef CRS232_h
#define CRS232_h

#import <Foundation/Foundation.h>
#include <stdio.h>
#include "SerialPort.h"
#include <string>

#import <pthread.h>

#include <Cocoa/Cocoa.h>


class CRS232 : CSerialPort
{
public:
    CRS232();
    ~CRS232();
public:
    int Open(const char * dev, const char * opt);//opt:"9600,8,n,1"
    int Close();
    
    int WriteString(const char * buffer);
    int WriteBytes(unsigned char * ucData, int len);
    int WriteStringBytes(const char * szData);//"0xFF,0xFE,..."
    
    const char * ReadString();
    void ClearBuffer();
    int SetDetectString(const char * det);
    int WaitDetect(int timeout);//msec
    
    const char * WriteReadString(const char * buffer,int timeout);
    
    void SetNotificationInfo(const char * detInfo, const char * notificationName);
    void AttachehView(NSTextView * NSView);
    void AddText(NSString * str);
    
protected:
    virtual void * didReceiveData(void * data, long len);
    
private:
    pthread_mutex_t m_mutex;
    pthread_mutex_t m_lockOperate;
    
    NSMutableString * m_strBuffer;
    NSMutableData * m_DataBuffer;
    NSMutableString * m_strDetect;
    NSMutableString * m_Return;
   
    NSMutableString * m_notificationInfo;
    NSMutableString * m_notificationName;
    NSTextView * txtView;
    
    int iTimeout;//msec
   
    
};

#endif /* CRS232_hpp */
