#ifndef ZMQREPLYER_H
#define ZMQREPLYER_H

/***************************************************************
* Purpose:   zmq Reply
* Author:    wuzx ()
* Created:   2016-04-27
**************************************************************/
#pragma once

#include <thread>
#include <string>
#include <mutex>
#include "NetCallBackDefine.hpp"

class  ZmqReplyer
{
public:
    ZmqReplyer();
    virtual ~ZmqReplyer();

	int Bind(const char* szSvrIp, int port);
	int Bind(const char * address);
	int Close();
	int SetCallBack(ReplyCallBack cb, void * ctx);
	void Start();
	std::string GetIp() const { return _ip; }
	int GetPort() const { return _port; }

	int Send(void * data, int len);
	int SendString(const char * buffer);
	std::string GetError();
	bool IsStart();
	void CallReConnectThr();
private:
	void* OnRequest(void *pdata, long len);
	void SetError(const std::string& msg);
	static void ReConnectThrFun(ZmqReplyer* onwer);
	void DelReconnectThr();
	int ReConnect(); 
private:
	static void * threadEntry(void * arg);
	bool m_bMonitorReply{ false };
	void * m_context;
	void * m_socket;
	std::thread* m_thread;
	ReplyCallBack m_pCallBack;
	void * m_pCBcontext;

	std::string _ip;
	int _port;

	std::string _error;
	std::mutex _mutex;
	std::thread* _reConnectThr = nullptr;
};

#endif // ZMQREPLYER_H
