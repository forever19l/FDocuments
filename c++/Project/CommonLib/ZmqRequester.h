#ifndef ZMQREQUESTER_H
#define ZMQREQUESTER_H

/***************************************************************
 * Purpose:   zmq请求类。
 * Author:    wuzx ()
 * Created:   2016-05-27
 **************************************************************/
#pragma once

#include <string>
#include <mutex>
#include <thread>

class  ZmqRequester
{
public:
    ZmqRequester();
    virtual ~ZmqRequester();

    int Connect(const char* szSvrIp, int port);
    int Connect(const char* address);
	

	void CallReConnectThr();
    int Close();

	int SendString(const std::string& str);
    int Send(const char* data, size_t len);
    int Recv(char*& buffer, size_t& len);
	int Select(int timeout);
	void SetTimeout(int timeout);
	int GetTimeout() const { return m_timeout; }

	std::string GetError();
	std::string GetIp() const { return _ip; }
	int GetPort() const { return _port; }
	bool IsConnect() const { return !_isBroken; }
	bool IsBroken() const { return _isBroken; }
	int Reconnect();
private:
	void SetError(const std::string& msg);
	static void ReConnectThrFun(ZmqRequester* onwer);
	void DelReconnectThr();
	
	//重连, 当req超时时，调用该方法，可继续使用
private:
    void* m_context;
    void* m_socket;
    int m_timeout;
	std::string _ip;
	int _port;
	bool _isBroken;
	std::string _error;
	std::mutex _mutex;
	std::thread* _reConnectThr = nullptr;
};

#endif // ZMQREQUESTER_H
