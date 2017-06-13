#pragma once

/***************************************************************
* Purpose:   zmq Reply
* Author:    wuzx ()
* Created:   2016-10-08
**************************************************************/

#include <stdio.h>
#include <string>
#include <mutex>
#include <thread>


class  ZmqPublisher
{
public:
	ZmqPublisher();
	~ZmqPublisher();

public:
	int Bind(const char* szSvrIp, int port);
	int Bind(const char * address);
    int Close();

    int Pulish(void * data, int len);
    int PulishString(const char * str);

	std::string GetIp() const { return _ip; }
	int GetPort() const { return _port; }
	std::string GetError();
	void CallReConnectThr();
private:
	void SetError(const std::string& msg);
	static void ReConnectThrFun(ZmqPublisher* onwer);
	void DelReconnectThr();
	int ReConnect();
protected:
    void * m_context;
    void * m_socket;
    
	std::string _ip;
	int _port;

	std::string _error;
	std::mutex _mutex;
	std::thread* _reConnectThr = nullptr;
};
