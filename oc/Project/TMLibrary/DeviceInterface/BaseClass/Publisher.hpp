//
//  Publisher.hpp
//  DevInterface
//
//  Created by Ryan on 1/19/17.
//  Copyright © 2017 ___Intelligent Automation___. All rights reserved.
//

#ifndef Publisher_hpp
#define Publisher_hpp

#include <stdio.h>

class CPublisher {
    CPublisher();
    virtual ~CPublisher();
    
public:
    virtual int CreatePubliser();
    virtual int ClosePubliser();
    virtual int publish(void * msg,int len,int level=0);
};

#endif /* Publisher_hpp */
