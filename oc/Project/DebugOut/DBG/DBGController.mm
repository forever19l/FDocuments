//
//  DutController.c
//  DUT
//
//  Created by Ryan on 11/3/15.
//  Copyright © 2015 ___Intelligent Automation___. All rights reserved.
//

#include "DBGController.h"
#include <iostream>
#include <Foundation/Foundation.h>
#include <sstream>
#include <map>
#include <pthread.h>

using namespace std;

@protocol DataInterface <NSObject>
-(void)OnData:(void *)bytes length:(long)len;
@end

CDBGController::CDBGController()
{
    pthread_mutex_init(&_mtx, NULL);
}
CDBGController::CDBGController(int index)
{
    m_index = index;
    pthread_mutex_init(&_mtx, NULL);
}

CDBGController::~CDBGController()
{
    pthread_mutex_destroy(&_mtx);
    CSubscriber::close();
}

int CDBGController::Initial(const char *arg2, void *arg)
{
    m_SubscriberAddress = arg2;
    
    CSubscriber::close();
    CSubscriber::connect(m_SubscriberAddress.c_str());
    return 0;
}

int CDBGController::Close()
{
    CSubscriber::close();
    return 0;
}


int CDBGController::SetDelegate(id object)
{
    m_object = object;
    return 0;
}

void * CDBGController::OnSubscriberData(void *pdata,long len)
{
    id<DataInterface> d = (id<DataInterface>)m_object;
    [d OnData:pdata  length:len];
    return nullptr;
}