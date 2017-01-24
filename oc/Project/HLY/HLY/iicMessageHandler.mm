//
//  MessageHandler.cpp
//  GUI
//
//  Created by Ryan on 8/27/15.
//  Copyright (c) 2015 ___Intelligent Automation___. All rights reserved.
//

#include "iicMessageHandler.h"
#include "TMLibrary/sysdef.h"

//#include "UICommon.h"

#include <Foundation/Foundation.h>
//extern void * OnUixMessage(std::string cmd, std::map<std::string, std::string> arg, void *context);

CMessageHandler::CMessageHandler()
{
//    pClient = new CMessageClient();
//    pClient->Connect("tcp://127.0.0.1",6200);
//    pClient->SetCallBack(OnUixMessage,kIICReadCmd);
    
}

CMessageHandler::CMessageHandler(int index)
{
    CMessageHandler();
    m_index = index;
}


CMessageHandler::~CMessageHandler()
{
}

int CMessageHandler::SendMessage(std::string cmd, std::map<std::string, std::string> *arg)
{
    pClient->SendMessage("iic read cmd");
    return 0;// pClient->SendMessage(cmd,arg);
}



int CMessageHandler::RequestStartModule(int module_id)
{
#if 0
    std::map<std::string, std::string> map;
    char buffer[32];
    sprintf(buffer, "%d",module_id);
    map["id"]= buffer;
    return SendMessage(EM_DO_START_MODULE, &map);
#else
    NSLog(@"~~~~~~~~~~~~~~Request StateMachine to Start Test");
    [[NSNotificationCenter defaultCenter] postNotificationName:kNotificationDoTestStart object:nil];
#endif
    return 0;
}

int CMessageHandler::RequestStopModule(int module_id)
{
#if 0
    std::map<std::string, std::string> map;
    char buffer[32];
    sprintf(buffer, "%d",module_id);
    map["id"]= buffer;
    return SendMessage(EM_DO_STOP_MODULE, &map);
#else
    [[NSNotificationCenter defaultCenter] postNotificationName:kNotificationDoTestStop object:nil userInfo:nil];
#endif
    return 0;
}

int CMessageHandler::RequestEnableSlot(int module_id, int slot_id, int state)
{
#if 0
    std::map<std::string, std::string> map;
    char buffer[32];
    sprintf(buffer, "%d",slot_id);
    map["id"]= buffer;
    sprintf(buffer, "%d",state);
    map["state"] = buffer;
    return SendMessage(EM_DO_ENABLE_SLOT, &map);
#else
    NSLog(@"!!!!!!!!!!!!!!!!!!!!Request StateMachine to Enable SLOT @%d=%d",slot_id,state);
    //[NSThread sleepForTimeInterval:0.3];
    NSDictionary * dic = [NSMutableDictionary dictionary];
    [dic setValue:@"set_uut_state" forKey:@"cmd"];
    [dic setValue:[NSNumber numberWithInt:slot_id] forKey:@"id"];
    [dic setValue:[NSNumber numberWithInt:state] forKey:@"buf"];
    
    [[NSNotificationCenter defaultCenter] postNotificationName:kNotificationUiCtrl object:nil userInfo:dic];
    
#endif
    return 0;
}

int CMessageHandler::RequestSetSN(int module_id, int slot_id, const char *sn, int sn_index)
{
#if 0
    std::map<std::string, std::string> map;
    char buffer[32];
    sprintf(buffer, "%d",slot_id);
    map["id"]= buffer;
    sprintf(buffer, "%d",sn_index);
    map["sn_index"] = buffer;
    map["sn"]=sn;
    return SendMessage(EM_DO_SET_SN, &map);
#else
    NSLog(@"~~~~~~~~~~~~~~Request StateMachine to set sn @slot%d=%s",slot_id,sn);
    NSDictionary * dic = [NSMutableDictionary dictionary];
    [dic setValue:@"set_sn" forKey:@"cmd"];
    [dic setValue:[NSNumber numberWithInt:slot_id] forKey:@"id"];
    [dic setValue:[NSString stringWithFormat:@"%s",sn] forKey:@"buf"];
    
    [[NSNotificationCenter defaultCenter] postNotificationName:kNotificationUiCtrl object:nil userInfo:dic];
#endif
    return 0;
}