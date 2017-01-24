//
//  ZMQinterface.h
//  Server
//
//  Created by Ryan on 8/15/15.
//  Copyright (c) 2015 ___Intelligent Automation___. All rights reserved.
//

#ifndef __Server__ZMQinterface__
#define __Server__ZMQinterface__

#include <stdio.h>

#include <map>
#include <string>
#include <thread>

#include <pthread.h>


typedef void(*HostCallBack) (void * buffer,int length);
class CZMQHost {
public:
    CZMQHost();
    CZMQHost(const char * name,int port=-1);
    virtual ~CZMQHost();
    
public:
    
    virtual int CreateHost(const char * name,int port=-1);
    virtual int CreateHost(int pubPort, int repPort);
    virtual int SendMessage(void * buffer,int length);
    virtual int SendString(const char * buffer);
    
    static std::map<std::string, int>  m_portlist;
    
protected:
    virtual void * OnMessage(void * buffer,long length);
    
    static void * ThreadEntry(void * arg);
    virtual void * HostLoop(void * arg);
    
    bool m_bMonitorRequest;
    
    void * m_ContextPublisher;
    void * m_ContextReply;
    void * m_SocketPubliser;
    void * m_SocketReply;
    
    pthread_t m_thread;
    
};


typedef HostCallBack ClientCallBack;

class CZMQClient {
public:
    CZMQClient();
    CZMQClient(const char * name);
    virtual ~CZMQClient();
    
public:
    virtual int Connect(const char * ip="tcp://127.0.0.1",int port=-1);
    int Connect(const char * ip,int subPort, int reqPort);
    virtual int SendMessage(void * buffer,int leng);
    virtual int SendString(const char * buffer);
    
protected:
    virtual void * OnMessage(void * buffer,long length);
    
    int m_port;
    
    bool m_bMonitorPublisher;
    
    static void * ThreadEntry(void * arg);
    void * ClientLoop(void * arg);
    
    void * m_ContextSubscriber;
    void * m_ContextRequest;
    void * m_SocketSubscriber;
    void * m_SocketRequest;
    
    pthread_t m_thread;
};

#endif /* defined(__Server__ZMQinterface__) */
