//
//  CSerialPort.hpp
//  DevInterface
//
//  Created by Ryan on 1/19/17.
//  Copyright © 2017 ___Intelligent Automation___. All rights reserved.
//

#ifndef CSerialPort_hpp
#define CSerialPort_hpp

#include <stdio.h>

#include "DeviceInterface.hpp"

class CSerialDevice:public CDevInterface {
public:
    CSerialDevice(char * szName,CPublisher * publiser = nullptr);
    virtual ~CSerialDevice();
    
public:
    //Base
    virtual int Open(char * arg);
    virtual int Close();
    virtual int WriteString(const char * cmd);
    virtual const char * ReadString();
    virtual int SetDetectString(const char * regex,int flag = 0);
    virtual int WaitForString(int iTimeOut=30*1000);
    virtual void ClearBuffer();
};

#endif /* CSerialPort_hpp */
