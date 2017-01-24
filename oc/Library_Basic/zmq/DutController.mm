//
//  DutController.c
//  DUT
//
//  Created by Ryan on 11/3/15.
//  Copyright © 2015 ___Intelligent Automation___. All rights reserved.
//

#include "DutController.h"
#include <iostream>
#include <Foundation/Foundation.h>
#include <sstream>
#include <map>
#include <pthread.h>

using namespace std;

@protocol DataInterface <NSObject>
-(void)OnData:(void *)bytes length:(long)len;
@end

CDUTController::CDUTController()
{
    pthread_mutex_init(&_mtx, NULL);
}
CDUTController::CDUTController(int index)
{
    m_index = index;
    pthread_mutex_init(&_mtx, NULL);
}

CDUTController::~CDUTController()
{
    pthread_mutex_destroy(&_mtx);
    CRequester::close();
    CSubscriber::close();
}

int CDUTController::Initial(const char *arg1, const char *arg2, void *arg)
{    
    m_RequestAddress = arg1;
    m_SubscriberAddress = arg2;
    
    CRequester::close();
    CSubscriber::close();
    CRequester::connect(m_RequestAddress.c_str());
    CSubscriber::connect(m_SubscriberAddress.c_str());
    return 0;
}

int CDUTController::Close()
{
    CRequester::close();
    CSubscriber::close();
    return 0;
}

int CDUTController::WriteString(const char * buffer)
{
    return RequestString(buffer);
}

int CDUTController::SetDelegate(id object)
{
    m_object = object;
    return 0;
}

void * CDUTController::OnSubscriberData(void *pdata,long len)
{
    id<DataInterface> d = (id<DataInterface>)m_object;
    [d OnData:pdata  length:len];
    return nullptr;
}