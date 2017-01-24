//
//  sysdef.h
//  TestExecutor
//
//  Created by Ryan on 5/8/15.
//  Copyright (c) 2015 ___Intelligent Automation___. All rights reserved.
//

#ifndef TestExecutor_sysdef_h
#define TestExecutor_sysdef_h

typedef struct {
    int module;
    int slot;
    void* param1; //
} em_notification_param_t;

#define EN_ZMQ_PUB_PORT 5000
#define EN_ZMQ_SUB_PORT 5010
#define EN_ZMQ_REQ_PORT 5011
#define EN_ZMQ_REP_PORT 5011
#define FIXTURE_PORT    5012


//Message for trigger engine event Receive
#define     EM_DO_START_MODULE          "EM_DO_START_MODULE"  //Start Module. e.g. UI click Start button
#define     EM_DO_ABORT_MODULE          "EM_DO_ABORT_MODULE"  //Abort Module e.g. UI clock Stop button
#define     EM_DO_STOP_MODULE           "EM_DO_STOP_MODULE"   //stop Module e.g. slots in module test finished
#define     EM_DO_PAUSE_MODULE          "EM_DO_PAUSE_MODULE"
#define     EM_DO_RESUME_MODULE         "EM_DO_RESUME_MODULE"
#define     EM_DO_LOAD_SEQUENCE         "EM_DO_LOAD_SEQUENCE" //load seq, e.g. UI click menu LoadSeq
#define     EM_DO_UPDATE_CONFIG         "EM_DO_UPDATE_CONFIG" //update config, r.g. UI click menu Config

#define     EM_DO_ENABLE_SLOT           "EM_DO_ENABLE_SLOT"//ARGS:1-MODULE ID[0-N] 2-SLOT ID[0-N] 3-STATE["YES", "NO"]
#define     EM_DO_SET_SN                "EM_DO_SET_SN"//ARGS:1-MODULE ID[0-N] 2-SLOT ID[0-N] 3-STATE["YES", "NO"]
#define     EM_DO_SET_CONFIG             "EM_DO_SET_CONFIG"//ARGS:1-MODULE ID[0-N] 2-SLOT ID[0-N] 3-STATE["YES", "NO"]


#define     EM_DO_SET_DEBUG         "EM_DO_SET_DEBUG"       //Set debug message,key:keyword,arg:arguments
#define     EM_DO_SET_TEST_MODE         "EM_DO_SET_TEST_MODE"  

//Message post from test engine //Post
#define     EM_ON_MODULE_START          "EM_ON_MODULE_START"            //Engine start, before all the slot startup
#define     EM_ON_ABORT_MODULE          "EM_ON_ABORT_MODULE"    //Abort Module e.g. UI clock Stop button
#define     EM_ON_MODULE_FINISHED       "EM_ON_MODULE_FINISHED"         //Engine finished,when all the slot stop

#define     EM_ON_TEST_START            "EM_ON_TEST_START"              //Message when a slot is start testing
#define     EM_ON_TEST_FINISHED         "EM_ON_TEST_FINISHED"           //Message when a slot is test finished
#define     EM_ON_TEST_ABORT            "EM_ON_TEST_ABORT"              //Message when a slot is test abort
#define     EM_ON_TEST_ERROR            "EM_ON_TEST_ERROR"              //Message when a slot is test error

#define     EM_ON_ITEM_START            "EM_ON_ITEM_START"
#define     EM_ON_ITEM_FINISHED         "EM_ON_ITEM_FINISHED"

#define     EM_ON_SELF_CHECK            "EM_ON_SELF_CHECK"

#define     EM_ON_LOAD_SEQUENCE         "EM_ON_LOAD_SEQUENCE"
#define     EM_ON_UPDATE_STATION_INFO   "EM_ON_UPDATE_STATION_INFO" //ivangan,update station,line,fixture id on GUI



//PDCA Message handler
#define PDCA_MSG_ADD_ITEM                   "PDCA_MSG_ADD_ITEM"
#define PDCA_MSG_ADD_ATTRIBUTE              "PDCA_MSG_ADD_ATTRIBUTE"
#define PDCA_MSG_ADD_ID                     "PDCA_MSG_ADD_SET_ID"                   //fixture id & head id
#define PDCA_MSG_ADD_BLOB                   "PDCA_MSG_ADD_BLOB"                     //Add blob file
#define PDCA_MSG

//Context key define



#endif
