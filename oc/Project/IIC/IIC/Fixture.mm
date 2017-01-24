//
//  Fixture.cpp
//  Fixture
//
//  Created by Ryan on 12-11-29.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "Fixture.h"
//#include "CoreLib/TestEngine.h"
#include <libkern/OSAtomic.h>
#include <pthread.h>
//extern TestEngine * m_pTestEngine;
extern NSMutableDictionary *pDictConfigInfo ;

//static int  pMPosition = -1 ;


CFixture::CFixture()
{
    m_pSerialPort = nil;
    m_SignalCount = 0;
    m_FixtureCondition = [NSCondition new];
//    this->initial_Condition();
}

CFixture::~CFixture()
{
    [m_FixtureCondition release];
}

//
//void CFixture::initial_Condition()
//{
//    int modules = [m_pTestEngine Cores];
//    int count=0;
//    for (int i=0; i<modules; i++) {
//        BOOL bIsTesting = [m_pTestEngine IsTesting:i];
//        if (bIsTesting) count++;
//    }
//}

//int CFixture::GetRunningModules()
//{
//    int modules=0;
    //NSString * str[]={@kEngineUUT0Enable,@kEngineUUT1Enable,@kEngineUUT2Enable,@kEngineUUT3Enable};
//    NSString * str[]={@kEngineUUT0Enable,@kEngineUUT1Enable,@kEngineUUT2Enable,@kEngineUUT3Enable,@kEngineUUT4Enable,@kEngineUUT5Enable};
//    for (int i=0; i<=5; i++) {
//        if ([[CTestContext::m_dicConfiguration valueForKey:str[i]] boolValue])
//        {
//            modules++;
//        }
//    }
//    return modules;
//    
//    modules = [m_pTestEngine Cores];
//    int count=0;
//    for (int i=0; i<modules; i++) {
//        BOOL bIsTesting = [m_pTestEngine IsTesting:i];
//        if (bIsTesting) count++;
//    }
//    return count;
//}

int CFixture::AttachSerialPort(SerialportEx *sp)
{
    SerialportEx * old  = sp;
    m_pSerialPort = [sp retain];
    [old release];
    return 0;
}

int CFixture::WriteString(char *szText)
{
    if (m_pSerialPort == nil) return -1;
    [m_pSerialPort writeString:[NSString stringWithUTF8String:szText] usingEncoding:NSUTF8StringEncoding error:NULL];
    return 0;
}


const char * CFixture::IICReadString()
{
    NSData * data = [m_pSerialPort GetInBuffer];
    NSString * strBuf = [[NSString alloc]initWithData:data encoding:NSASCIIStringEncoding];
    NSString * str = [NSString stringWithString:strBuf];
    [strBuf release];
    return [str UTF8String];
}


int CFixture::ReadSensor()
{
    WriteString((char *)"read all");
    return 0;
}

int CFixture::IsClose()
{
    WriteString((char *)"in?\r");
    usleep(100) ;
    const char *rtn = IICReadString() ;
    if (rtn==NULL)
        return 0 ;
    NSString *strTmp = [NSString stringWithFormat:@"%s",rtn] ;
    NSRange range = [strTmp rangeOfString:@"yes"] ;
    if (range.location==NSNotFound)
        return 0 ;
    
    return 1; ///Close cover
}


int CFixture::CheckClose()
{
    [m_FixtureCondition lock];
    
    int rtn = IsClose() ;
//    if (!IsClose())
//    {
//        //NSRunAlertPanel(@"Check Cover", @"Fixture cover is not close,please close it before test start!", @"OK", nil, nil);
//        return 0;
//    }
    [m_FixtureCondition unlock];
    return rtn;
}

char * CFixture::syncSendCmd(int index,char *szText)  //add by Louis ,
{
    static char *rtn = NULL ;
    [m_FixtureCondition lock];
//    UUT_READY[index]=YES;
//    if (CheckFixtureReady(m_pTestEngine)) //所有的UUT均已ready
//    {
//        WriteString(szText);
//        usleep(200) ;
//        rtn = (char*)ReadString() ;
//        [m_FixtureCondition broadcast];
//    }
//    else {
//        //[m_FixtureCondition wait];
//    }
    [m_FixtureCondition unlock];
    
    return rtn ;
}


//add by liang
int CFixture::SetDetectString(char * regex)
{
    [m_pSerialPort SetDetectString:[NSString stringWithUTF8String:regex]];
    return 0;
}
int CFixture::WaitForString(int iTimeout)
{
    return [m_pSerialPort WaitDetect:iTimeout];
}
