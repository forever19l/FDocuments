#pragma once

/***************************************************************
* Purpose:   tcp/ip client 封装类
* Author:    wuzx ()
* Created:   2016-10-28
**************************************************************/

#include <string>
#include <thread>
#include <mutex>
#include "NetCallBackDefine.hpp"

class  CTcpClient
{
public:
	CTcpClient();
	~CTcpClient();

	int Connect(const char* szSvrIp, int port);
	void Close();
	
	int SendString(const std::string& str);
	int Send(const char* data, size_t len);
	int RecvData(std::string& data);


	void Start();

	void SetRecvDataCallBack(RecvDataCallBack callback, void* context);
	void SetCloseClientCallBack(CloseClientCallBack callback, void* context);
	std::string GetError();
	
private:
	bool Select(int milisecond);
	void Stop();
	static void OnProcServerData(void* context);
	void SetError(const std::string& msg);
	void ProcRecvData();
	void ColseSocketHandle(int& socketHandle);
private:
	std::thread* _workThread = NULL;
    int _socketHandle = 0;

	bool _isExit = false;
	std::string _ip;
	int _port = -9999;

	RecvDataCallBack _recvDataCallback = NULL;
	void* _recvDataContext = NULL;

	CloseClientCallBack _closeClientCallback = NULL;
	void* _closeClientContext = NULL;

	std::string _error;
	std::mutex _mutex;
};



