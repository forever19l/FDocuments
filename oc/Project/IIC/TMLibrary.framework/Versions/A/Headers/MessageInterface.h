//
//  MessageInterface.h
//  Server
//
//  Created by Ryan on 8/20/15.
//  Copyright (c) 2015 ___Intelligent Automation___. All rights reserved.
//

#ifndef __Server__MessageInterface__
#define __Server__MessageInterface__

#include <stdio.h>
#include <map>
#include "ZMQinterface.h"

typedef void*(*MessageCallBack) (std::string cmd,std::map<std::string, std::string> arg,void * context);
class CMessageHost : public CZMQHost{
public:
    CMessageHost();
    virtual ~CMessageHost();
    
public:
    int SetCallBack(MessageCallBack pCallBack,void * context=nullptr);
    int SendMessage(std::string cmd,std::map<std::string, std::string> *arg=nullptr);
public:
    
protected:
    virtual void * OnMessage(void * buffer,long length);
    
protected:
    
    MessageCallBack m_pCallBack;
    void * pContext;
};

class CMessageClient:public CZMQClient {
public:
    CMessageClient();
    virtual ~CMessageClient();

public:
    
public:
    int SetCallBack(MessageCallBack pCallBack,void * context=nullptr);
    int SendMessage(std::string cmd,std::map<std::string, std::string> *arg=nullptr);
    
protected:
    virtual void * OnMessage(void * buffer,long length);
    
protected:
    
    MessageCallBack m_pCallBack;
    void * pContext;
};



#endif /* defined(__Server__MessageInterface__) */
