//
//  DutController.h
//  DUT
//
//  Created by Ryan on 11/3/15.
//  Copyright © 2015 ___Intelligent Automation___. All rights reserved.
//

#ifndef DutController_h
#define DutController_h

#include <stdio.h>
//#include <string>

#include "Requester.hpp"
#include "Subscriber.hpp"

class CDUTController:CRequester,CSubscriber {
public:
    CDUTController();
    CDUTController(int index);
    virtual ~CDUTController();
    
public:
    int Initial(const char * arg1,const char * arg2,void * arg=nullptr);    //arg1 : requrest address, arge2: publisher address,
    int Close();
    int WriteString(const char * buffer);
    int SetDelegate(id object);
    
    virtual void * OnSubscriberData(void *pdata,long len);
    
protected:
    int m_index;
    //CZMQClient * pClient;
    pthread_mutex_t _mtx;
    
    std::string m_RequestAddress,m_SubscriberAddress;
    
    id m_object;
};

#endif /* DutController_h */
