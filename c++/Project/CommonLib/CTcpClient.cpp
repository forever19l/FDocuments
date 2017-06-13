#pragma once
#include "CTcpClient.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#include <winsock2.h>  
#include <mstcpip.h>
#pragma comment(lib, "ws2_32.lib")
#define sprintf sprintf_s
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include  <netdb.h>
#include <netinet/tcp.h>
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

#ifdef WIN32
	WSADATA  Ws;
	if (WSAStartup(MAKEWORD(2, 2), &Ws) != 0)
	{
		char szError[1024] = { 0 };
		sprintf(szError, "Init Socket Failed:%d", GetLastError());
		printf("%s\n", szError);
		SetError(szError);
		return -1;
	}

	_socketHandle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_socketHandle == INVALID_SOCKET)
	{
		char szError[1024] = { 0 };
		sprintf(szError, "Create Socket Failed:%d", GetLastError());
		printf("%s\n", szError);
		SetError(szError);
		return -1;
	}

	int ret;

	

	struct sockaddr_in ServerAddr;
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.s_addr = inet_addr(szSvrIp);
	ServerAddr.sin_port = htons(port);
	memset(ServerAddr.sin_zero, 0x00, 8);

	ret = connect(_socketHandle, (struct sockaddr*)&ServerAddr, sizeof(ServerAddr));
	if (ret == SOCKET_ERROR)
	{
		char szError[1024] = { 0 };
		sprintf(szError, "Connect server failed! Ip[%s] port[%d]", szSvrIp, port);
		printf("%s\n", szError);
		SetError(szError);
		return -1;

	}

	bool bKeepAlive = true;
	ret = ::setsockopt(_socketHandle, SOL_SOCKET, SO_KEEPALIVE, (char*)&bKeepAlive, sizeof(bKeepAlive));
	if (ret == SOCKET_ERROR)
	{
		char szError[1024] = { 0 };
		sprintf(szError, "set keepalive fail:%d", GetLastError());
		printf("%s\n", szError);
		SetError(szError);
	}

	tcp_keepalive alive_in = { 0 };
	tcp_keepalive alive_out = { 0 };
	alive_in.keepalivetime = 5000;
	alive_in.keepaliveinterval = 1000;
	alive_in.onoff = 1;
	unsigned long ulBytesReturn = 0;
	ret = WSAIoctl(_socketHandle, SIO_KEEPALIVE_VALS, &alive_in, sizeof(alive_in),
		&alive_out, sizeof(alive_out), &ulBytesReturn, NULL, NULL);
	if (ret == SOCKET_ERROR)
	{
		char szError[1024] = { 0 };
		sprintf(szError, "set SIO_KEEPALIVE_VALS fail:%d", GetLastError());
		printf("%s\n", szError);
		SetError(szError);
	}
	

#else
   
    struct hostent *he;

    struct sockaddr_in server;
    int sockfd;
    if((sockfd= socket(AF_INET, SOCK_STREAM, 0)) == -1)
    { 
		char szError[1024] = {0};
		sprintf(szError, "Create socket failed. %s", strerror(errno));
		printf("%s\n", szError);
		SetError(szError);
		return -1;
    }
    int portreused = 1;
    int keepalive = 1; // 开启keepalive属性
    int keepidle = 1; // 如该连接在60秒内没有任何数据往来,则进行探测
    int keepinterval = 1; // 探测时发包的时间间隔为5 秒
    int keepcount = 3; // 探测尝试的次数。如果第1次探测包就收到响应了,则后2次的不再发。
    setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepalive , sizeof(keepalive ));
    setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPALIVE, (void*)&keepidle , sizeof(keepidle ));
    setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPINTVL, (void *)&keepinterval , sizeof(keepinterval ));
    setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPCNT, (void *)&keepcount , sizeof(keepcount ));
    setsockopt(sockfd, IPPROTO_TCP, SO_REUSEPORT, (void *)&portreused , sizeof(portreused));

    he = gethostbyname(szSvrIp);
    if(he== NULL)
    {
		char szError[1024] = {0};
	   sprintf(szError, "gethostbyname(%s) error", szSvrIp);
	   printf("%s\n", szError);
	   SetError(szError);
       return -1;
    }

    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr = *((struct in_addr *)he->h_addr);
    if(connect(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
		char szError[1024] = {0};
		sprintf(szError, "connect() error");
		printf("%s\n", szError);
		SetError(szError);
        return -1;
    }
    _socketHandle = sockfd;
#endif
	_ip = szSvrIp;
	_port = port;

	printf("Connect Server Success. IP[%s] Port[%d]\n", szSvrIp, port);
	return 0;
}

void CTcpClient::Close()
{
	if (_workThread != NULL)
	{
		_isExit = true;
		if (_workThread->joinable())
			_workThread->join();
		delete _workThread;
		_workThread = NULL;
		
	}
	_isExit = true;
	ColseSocketHandle(_socketHandle);
}

int CTcpClient::SendString(const std::string& str)
{
	return Send(str.c_str(), str.size());
}

int CTcpClient::Send(const char* data, size_t len)
{
	if (_socketHandle == 0)
		return 0;
    int haveSend = 0;
    int left = len;
    while (left > 0)
    {
#ifdef WIN32
        haveSend = send(_socketHandle, data, left, 0);
        if (haveSend < 0)
        {
            int lastErr = WSAGetLastError();
            if (lastErr == WSAEWOULDBLOCK || lastErr == WSAEINTR)
            {
                haveSend = 0;
                continue;
            }
			char szError[1024] = { 0 };
            sprintf(szError, "Send data failed. Error is %d", lastErr);
            printf("%s\n", szError);
            SetError(szError);
            return -1;
        }
#else
        haveSend = write(_socketHandle, data, left);
        if (haveSend < 0)
        {
           if (errno == EINTR || EWOULDBLOCK  == errno)
           {
               haveSend = 0;
               continue;
           }

		   char szError[1024] = {0};
            sprintf(szError, "Send data failed. Error is %s", strerror(errno));
            printf("%s\n", szError);
            SetError(szError);
            return -1;
        }
#endif
        left -= haveSend;
        data += haveSend;
    }
    return len;
}

int CTcpClient::RecvData(std::string& data)
{
	char szBuf[8192] = {0};
    int readNum = 0;
 #ifdef WIN32
    readNum = recv(_socketHandle, szBuf, sizeof(szBuf), 0);
	if (readNum < 0)
	{
		int lastErr = WSAGetLastError();
		char szError[1024] = { 0 };
        sprintf(szError, "Recv data failed. Error is %d", lastErr);
        printf("%s\n", szError);
		SetError(szError);

		Stop();
		
		return -1;
	}
	else if (readNum == 0)
	{
		std::string info = "The Server have been closed.\n";
		printf("%s", info.c_str());
		SetError(info);
		Stop();
		
		return -1;
	}
#else
    readNum = read(_socketHandle, szBuf, sizeof(szBuf));
    if (readNum < 0)
    {
		char szError[1024] = {0};
        sprintf(szError, "Recv data failed. Error is %s", strerror(errno));
        printf("%s\n", szError);
        SetError(szError);
        Stop();
		
        return -1;
    }
    else if (readNum == 0)
    {
        std::string info = "The Server have been closed.\n";
        printf("%s", info.c_str());
        SetError(info);
        Stop();
		
        return -1;
    }
#endif
	data.assign(szBuf, readNum);

	return readNum;
}

bool CTcpClient::Select(int milisecond)
{
	fd_set rfd;
	FD_ZERO(&rfd);
	FD_SET(_socketHandle, &rfd);

	struct timeval timeout;
	timeout.tv_sec = 0;                
	timeout.tv_usec = milisecond * 1000;

	int ret = select(_socketHandle + 1, &rfd, NULL, NULL, &timeout);

    if (ret == 0 || ret == -1){
		return false;
	}

	if (FD_ISSET(_socketHandle, &rfd)){      
		return true;
	}

	return false;
}

void CTcpClient::OnProcServerData(void* context)
{
	CTcpClient* client = static_cast<CTcpClient*>(context);
	while (!client->_isExit){
		client->ProcRecvData();
	}
}

void CTcpClient::ProcRecvData()
{
	if (Select(500)){
		std::string data;
		if (RecvData(data)  < 0){
			return;
		}

		if (_recvDataCallback != NULL)
		{
			_recvDataCallback(data, _recvDataContext);
		}
		else{
			printf("Recv data:%s\n", data.c_str());
		}
	}
}

void CTcpClient::ColseSocketHandle(int& socketHandle)
{
	if (socketHandle == 0)
		return;
#ifdef WIN32
	closesocket(socketHandle);
#else
	close(socketHandle);
#endif
	socketHandle = 0;
}

void CTcpClient::Start()
{
	_isExit = false;
	if (_workThread != NULL)
	{
		_isExit = true;
		if (_workThread->joinable())
			_workThread->join();
		delete _workThread;
		_workThread = NULL;
		_isExit = false;
	}
	_workThread = new std::thread(CTcpClient::OnProcServerData, this);
}

void CTcpClient::Stop()
{
	_isExit = true;
	ColseSocketHandle(_socketHandle);
	if (_closeClientCallback != NULL){
		_closeClientCallback(_closeClientContext);
	}
}

std::string CTcpClient::GetError()
{
	std::unique_lock<std::mutex> lock(_mutex);
	return _error;
}

void CTcpClient::SetError(const std::string& msg)
{
	std::unique_lock<std::mutex> lock(_mutex);
	_error = msg;
}

