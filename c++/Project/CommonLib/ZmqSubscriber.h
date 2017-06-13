#ifndef _ZMQSUBSCRIBER_H
#define _ZMQSUBSCRIBER_H

/***************************************************************
 * Purpose:   zmq 接收广播消息类。
 * Author:    wuzx ()
 * Created:   2016-05-27
 **************************************************************/

#include <stdio.h>
#include <string>
#include <thread>
#include <mutex>
#include "NetCallBackDefine.hpp"


class  ZmqSubscriber
{
public:
    ZmqSubscriber();
    virtual ~ZmqSubscriber();

public:
    int Connect(const char * address);
	int Connect(const char* szSvrIp, int port);
	void CallReConnectThr();
    int Close();

    void Start();
    int SetFilter(const char * filter);
	int SetCallBack(SubCallBack cb, void * ctx);

	std::string GetIp() const { return _ip; }
	int GetPort() const { return _port; }
	std::string GetError();
private:
	void* OnSubscriberData(void *pdata, long len);
	void SetError(const std::string& msg);
	void DelReconnectThr();
	int ReConnect();
	
private:
    static void * threadEntry(void * arg);
	static void ReConnectThrFun(ZmqSubscriber* onwer);
	bool m_bMonitorSubscrib = true;
    void * m_context = NULL;
    void * m_socket = NULL;
	std::thread* m_thread = NULL;

	SubCallBack m_pCallBack;
    void * m_pCBcontext = NULL;
    std::string m_strFilter;

	std::string _ip;
	int _port;

	std::string _error;
	std::mutex _mutex;
	std::thread* _reConnectThr = nullptr;
};

#endif /* _ZMQSUBSCRIBER_H */
