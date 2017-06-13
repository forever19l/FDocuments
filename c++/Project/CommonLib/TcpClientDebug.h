#pragma once
#include "NetDebugPlug.h"
#include "CTcpClient.h"


class  TcpClientDebug
{
public:
	TcpClientDebug();
	~TcpClientDebug();
	int Open(const char* szSvrIp, int port, const char* reply, const char* publish);
	int ReOpen();
	void Close();
	int Send(const char* data, size_t len);
	std::string GetError();
	void SetRecvDataCallBack(OnRecvDataCallBack callback, void* context);
	void SetCloseClientCallBack(OnCloseClientCallBack callback, void* context);
private:
	static void RecvDataCallBack(const std::string& buf, void* context);
	static void CloseClientCallBack(void* context);
	void OnRecvData(const std::string& data);
	void OnCloseClient();
	void SetError(const std::string& msg);
private:
	NetDebugPlug<CTcpClient>* _pNetDebugPlug;
	std::string _IP;
	int _Port;
	std::string _error;
	CTcpClient* _pTcpClient;
	void* _recvDataContext;
	void*  _closeClientContext;
	OnRecvDataCallBack _onRecvData;
	OnCloseClientCallBack _onCloseClient;
	bool _isConnect{ false };
};

