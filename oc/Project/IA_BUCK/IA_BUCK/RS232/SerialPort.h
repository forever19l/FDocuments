//
//  RS232.h
//  RS232
//
//  Created by IvanGan on 16/4/9.
//  Copyright © 2016年 IvanGan. All rights reserved.
//
#include <stdio.h>
#include <string.h>
#include <pthread.h>

typedef int (*OnReceiveData)(void * data, size_t len, void * context);
class CSerialPort
{
public:
    CSerialPort();
    ~CSerialPort();
    
    int connect(const char * dev);
    int set_opt(int nSpeed, int nBits, char nEvent, int nStop);
    int read(void * buffer, int len);
    int write(void * buffer, long len);
    
    int setCallBack(OnReceiveData pf, void * context=nullptr);
    virtual void * didReceiveData(void * data, long len);
    int close();
    
protected:
    pthread_t m_thread;
    static void * ReadDataInBackGround(void * arg);
    
private:
    int fd;
    bool bReadDataInBackGround;
    OnReceiveData m_CallBack;
    void * m_pCallBackContext;
};

