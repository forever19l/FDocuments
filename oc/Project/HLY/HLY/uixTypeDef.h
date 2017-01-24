//
//  uixTypeDef.h
//  UIExternal
//
//  Created by Liang on 15-8-31.
//  Copyright (c) 2015年 ___Intelligent Automation___. All rights reserved.
//

#ifndef UIExternal_MessageDef_h
#define UIExternal_MessageDef_h

#define MAX_UUT 6
#define MAX_POSITION 4

typedef enum {
    TR_PASS = 0,
    TR_FAIL,
    TR_ERROR,
    TR_NA//uut slot disabled
} eTEST_RESULT;

typedef enum {
    TM_NA = 0,
    TM_IDLE,
    TM_TESTING,
    TM_TEST_FINISHED,
    TM_ERROR,
} eTEST_MANAGER_STATE;//test application state

#define EVENT_FIXTURE_ERROR @"EVENT_FIXTURE_ERROR"
#define EVENT_FIXTURE_QUERY_STATE   @"EVENT_FIXTURE_QUERY_STATE"
#define EVENT_FIXTURE_QUERY_DEPLOYMENT   @"EVENT_FIXTURE_QUERY_DEPLOYMENT"
#define EVENT_FIXTURE_ALIGNMENT_ENGAGE_OK   @"EVENT_FIXTURE_ALIGNMENT_ENGAGE_OK"
#define EVENT_FIXTURE_QUERY_TEST_RESULT @"EVENT_FIXTURE_QUERY_TEST_RESULT"



#endif
