#include "CTcpClient.h"

#include <Windows.h>
#include <iostream>
#include <stdio.h>

#pragma comment(lib,"wsock32.lib")

#ifdef WIN32
#define sprintf sprintf_s
#endif


CTcpClient::CTcpClient()
{
}

CTcpClient::~CTcpClient()
{
	Close();
}

void CTcpClient::SetRecvDataCallBack(RecvDataCallBack callback, void* context)
{
	_recvDataCallback = callback;
	_recvDataContext = context;
}

void CTcpClient::SetCloseClientCallBack(CloseClientCallBack callback, void* context)
{
	_closeClientCallback = callback;
	_closeClientContext = context;
}

int CTcpClient::Connect(const char* szSvrIp, int port)
{
	int Ret = 0;
	int AddrLen = 0;

	WSADATA  Ws;
	if (WSAStartup(MAKEWORD(2, 2), &Ws) != 0)
	{
		printf("Init Socket Failed:%d\n", GetLastError());
		return -1;
	}

	_socketHandle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_socketHandle == INVALID_SOCKET)
	{
		printf("Create Socket Failed:%d\n", GetLastError());
		return -1;
	}

	struct sockaddr_in ServerAddr;
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.s_addr = inet_addr(szSvrIp);
	ServerAddr.sin_port = htons(port);
	memset(ServerAddr.sin_zero, 0x00, 8);

	int ret = connect(_socketHandle, (struct sockaddr*)&ServerAddr, sizeof(ServerAddr));
	if (ret == SOCKET_ERROR)
	{
		printf("Connect server failed! Ip[%s] port[%d]\n", szSvrIp, port);
		return -1;
	}

	if (SOCKET_ERROR == setsockopt(_socketHandle, SOL_SOCKET, SO_SNDTIMEO, (char *)&_timeOut, sizeof(int)))
	{
		printf("Set Send OverTime. Error:%d\n", GetLastError());
		return -1;
	}

	if (SOCKET_ERROR == setsockopt(_socketHandle, SOL_SOCKET, SO_RCVTIMEO, (char *)&_timeOut, sizeof(int)))
	{
		printf("Set Recv OverTime. Error:%d\n", GetLastError());
		return -1;
	}

	_ip = szSvrIp;
	_port = port;

	printf("Connect Server Success. IP[%s] Port[%d]\n", szSvrIp, port);
	return 0;
}

void CTcpClient::Close()
{
	if (!_isExit && _workThread != NULL && _workThread->joinable())
	{
		_isExit = true;
		_workThread->join();
	}

	if (_socketHandle != 0)
	{
		closesocket(_socketHandle);
		_socketHandle = 0;
	}
}

int CTcpClient::SendString(const std::string& str)
{
	return Send(str.c_str(), str.size());
}

int CTcpClient::Send(const char* data, size_t len)
{
	int haveSend = 0;
	int left = len;
	while (left > 0)
	{
		haveSend = send(_socketHandle, data, left, 0);
		if (haveSend < 0)
		{
			int lastErr = WSAGetLastError();
			if (lastErr == WSAEWOULDBLOCK || lastErr == WSAEINTR)
			{
				haveSend = 0;
				continue;
			}
			char szError[1024];
			sprintf(szError, "Send data failed. Error is %d", lastErr);
			printf("%s\n", szError);
			_error = szError;
			return -1;
		}

		left -= haveSend;
		data += haveSend;
	}
	
	return 0;
}

int CTcpClient::RecvData()
{
	char szBuf[2048] = {0};
	int readNum = recv(_socketHandle, szBuf, sizeof(szBuf), 0);
	if (readNum < 0)
	{
		int lastErr = WSAGetLastError();

		char szError[1024];
		sprintf(szError, "Recv data failed. Error is %d", lastErr);
		printf("%s\n", _error);
		_error = szError;

		Stop();
		return -1;
	}
	else if (readNum == 0)
	{
		std::string info = "The Server have been closed.\n";
		printf("%s", info.c_str());
		if (_recvDataCallback != NULL)
		{
			_recvDataCallback(info, _recvDataContext);
		}
		Stop();
		return 0;
	}

	std::string data = szBuf;
	if (_recvDataCallback != NULL)
	{
		_recvDataCallback(data, _recvDataContext);
	}
	return 0;
}

bool CTcpClient::Select()
{
	fd_set rfd;
	FD_ZERO(&rfd);
	FD_SET(_socketHandle, &rfd);

	struct timeval timeout;
	timeout.tv_sec = 3;                
	timeout.tv_usec = 0;

	int ret = select(_socketHandle + 1, &rfd, NULL, NULL, &timeout);
	if (ret == 0 || ret == SOCKET_ERROR)
	{ 
		return false;
	}

	if (FD_ISSET(_socketHandle, &rfd))
	{      
		return true;
	}

	return false;
}

void CTcpClient::OnProcServerData(void* context)
{
	CTcpClient* client = static_cast<CTcpClient*>(context);
	while (!client->_isExit)
	{
		if (client->Select())
		{
			client->RecvData();
		}
	}

	printf("Client thread stop.....\n");
}

void CTcpClient::Start()
{
	_isExit = false;
	_workThread = new std::thread(CTcpClient::OnProcServerData, this);
	printf("Start recv data thread Successful................................\n");
}

void CTcpClient::Stop()
{
	_isExit = true;

	if (_closeClientCallback != NULL){
		_closeClientCallback(_closeClientContext);
	}
}