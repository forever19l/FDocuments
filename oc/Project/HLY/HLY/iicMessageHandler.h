//
//  MessageHandler.h
//  GUI
//
//  Created by Ryan on 8/27/15.
//  Copyright (c) 2015 ___Intelligent Automation___. All rights reserved.
//

#ifndef __GUI__MessageHandler__
#define __GUI__MessageHandler__

#include <stdio.h>
#include "TMLibrary/MessageInterface.h"

#define kNotificationDoTestStart        @"DO_TestStart"
#define kNotificationDoTestStop         @"DO_TestStop"
#define kNotificationUiCtrl             @"On_UiCtrl"
#define kIICReadCmd                     @"iic read cmd"

class CMessageHandler {
public:
    CMessageHandler();
    CMessageHandler(int index);
    virtual ~CMessageHandler();
    
public:
    int RequestStartModule(int module_id);
    int RequestStopModule(int module_id);
    int RequestEnableSlot(int module_id, int slot_id, int state);
    int RequestEnableSlot_WithSN(int module_id, int slot_id, int state,const char * sn);
    int RequestSetSN(int module_id, int slot_id, const char *sn, int sn_index);
    int SendMessage(std::string cmd,std::map<std::string, std::string> * arg);
    
protected:
    CMessageClient * pClient;
    int m_index;
};

#endif /* defined(__GUI__MessageHandler__) */
