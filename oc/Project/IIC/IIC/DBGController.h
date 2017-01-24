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
#include <string>


class CDBGController:CSubscriber {
public:
    CDBGController();
    CDBGController(int index);
    virtual ~CDBGController();
    
public:
    int Initial(const char * arg2,void * arg=nullptr);    //arg1 : requrest address, arge2: publisher address,
    int Close();

    int SetDelegate(id object);
    
    virtual void * OnSubscriberData(void *pdata,long len);
    
protected:
    int m_index;
    //CZMQClient * pClient;
    pthread_mutex_t _mtx;
    
    std::string m_SubscriberAddress;
    
    id m_object;
};

#endif /* DutController_h */
