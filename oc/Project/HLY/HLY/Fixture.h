//
//  Fixture.h
//  Fixture
//
//  Created by Ryan on 12-11-29.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef Fixture_h
#define Fixture_h
#include "SerialportEx.h"
#ifdef __cplusplus

class CFixture {
public:
    CFixture();
    ~CFixture();
public:
    int WriteString(char * szText);         //Write data into HLY serial port;
    const char * HLYReadString();                    //Read data
    
    int ReadSensor();
    
    int IsClose();
    int CheckClose();

    char * syncSendCmd(int index,char *szText)  ;
    
    /*Motor action function*/
    int MotorSendCmd(char* szText) ;
    int SetDetectString(char * regex);
    int WaitForString(int iTimeout);
    
public:
    int AttachSerialPort(SerialportEx * sp);
    int m_SignalCount;
    
protected:
    SerialportEx * m_pSerialPort;
    NSCondition *  m_FixtureCondition;
    
//protected:
//    void initial_Condition();
//    int GetRunningModules();
};
#endif

#endif
