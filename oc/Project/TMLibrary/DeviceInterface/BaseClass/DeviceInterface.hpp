//
//  DeviceInterface.hpp
//  DevInterface
//
//  Created by Ryan on 1/18/17.
//  Copyright © 2017 ___Intelligent Automation___. All rights reserved.
//

#ifndef DeviceInterface_hpp
#define DeviceInterface_hpp

#include <stdio.h>
#include "Publisher.hpp"

class CDevInterface {
public:
    CDevInterface(void) {
        
    }
    
    CDevInterface(const char *deviceName, CPublisher *publiser = NULL) {
        
    }
    
    virtual ~CDevInterface(void) {
    
    }
    
public:
    //Base
    virtual int Open(const char *cmdString)=0;
    virtual int Close(void)=0;
    virtual int WriteString(const char *cmd)=0;
    virtual const char *ReadString(void)=0;
    virtual int SetDetectString(const char *regex, int flag = 0)=0;
    virtual int WaitForString(int timeoutMilliSec = 30*1000)=0;
    virtual void ClearBuffer(void)=0;

//    virtual int Write(const void *buffer, unsigned long len);
//    virtual int Read(void *buffer, int len);
};

#endif /* DeviceInterface_hpp */
