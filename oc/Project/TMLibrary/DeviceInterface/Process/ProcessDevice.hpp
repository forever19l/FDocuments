//
//  ProcessDevice.hpp
//  DevInterface
//
//  Created by Ryan on 1/19/17.
//  Copyright © 2017 ___Intelligent Automation___. All rights reserved.
//

#ifndef ProcessDevice_hpp
#define ProcessDevice_hpp

#include <string>
#include <mutex>
#include <thread>

#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/time.h>

#include "DeviceInterface.hpp"

using std::string;
using std::mutex;


int popen_rw(const char *cmdString, int inPipe[2]);
uint64_t TimeInterval(const struct timeval *start, const struct timeval *end);


class CProcessDevice : public CDevInterface {
public:
    
    CProcessDevice(const char *deviceName, CPublisher *publisher = NULL);
    virtual ~CProcessDevice(void);
    
public:
    virtual int Open(const char *cmdString);
    virtual int Close(void);
    virtual int WriteString(const char * cmd);
    virtual const char *ReadString(void);
    virtual int SetDetectString(const char *regex, int flag = 0);
    virtual int WaitForString(int timeoutMilliSec = 30*1000);
    virtual void ClearBuffer(void);

private:
    static void *CopyChildProcessOutputToBuffer(void *parm);
    
private:
    string m_inBuffer;
    string m_outBuffer;
    string m_detectStr;
    string m_userBuffer;
    
    string m_deviceName;
    
    mutex m_writeMutex;
    mutex m_readMutex;
    
    int m_pipe[2];
    
    pid_t m_pid;
    pthread_t m_tid;
    
    bool m_runFlag;
    
    int m_regexFlag;
    
    CPublisher *m_publisher;
    
};

#endif /* ProcessDevice_hpp */
