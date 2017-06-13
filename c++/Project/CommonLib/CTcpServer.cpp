#include "CTcpServer.h"

#include <iostream>
#include <stdio.h>

#ifdef WIN32
#define sprintf sprintf_s
#endif

#ifdef WIN32
#include <winsock2.h>  
#include <mstcpip.h>
#pragma comment(lib, "ws2_32.lib")
#define socklen_t int
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#endif

const int FD_DISCONNECT_STATE = 0;
const int FD_CONNECT_STATE = 1;
CTcpServer::CTcpServer()
{

}

CTcpServer::~CTcpServer()
{

}

void CTcpServer::Close()
{
	if (!_isExit && _workThread != NULL && _workThread->joinable())
	{
		_isExit = true;
		_workThread->join();
		delete _workThread;
		_workThread = NULL;
	}

    std::map<int, TSockClientInfo>::iterator iter = _clientFds.begin();
	for (; iter != _clientFds.end(); ++iter)
	{
		int fd = iter->first;
		CloseSocketHandle(fd);
	}
	_clientFds.clear();

	CloseSocketHandle(_socketHandle);
}

void CTcpServer::SetFDCallBack(FDCallBack callback, void* context)
{
	_fdCallback = callback;
	_fdContext = context;
}

void CTcpServer::SetRecvDataCallBack(RecvClientDataCallBack callback, void* context)
{
	_recvDataCallback = callback;
	_recvDataContext = context;
}

int CTcpServer::Bind(const char* szSvrIp, int port)
{
 #ifdef WIN32
	WSADATA  Ws;
	if (WSAStartup(MAKEWORD(2, 2), &Ws) != 0)
	{
		printf("Init Socket Failed:%d\n", GetLastError());
		return -1;
	}
#endif

	_socketHandle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (_socketHandle < 0)
	{
        printf("Create Socket Failed.\n");
		return -1;
	}
	
	int ret;
	struct sockaddr_in LocalAddr;
	LocalAddr.sin_family = AF_INET;
	LocalAddr.sin_addr.s_addr = inet_addr(szSvrIp);
	LocalAddr.sin_port = htons(port);
	memset(LocalAddr.sin_zero, 0x00, 8);

	 ret = bind(_socketHandle, (struct sockaddr*)&LocalAddr, sizeof(LocalAddr));
	if (ret != 0)
	{
        printf("Bind Socket Failed!\n");
		return -1;
	}

	ret = listen(_socketHandle, 10);
	if (ret != 0)
	{
        printf("listen Socket Failed!\n");
		return -1;
	}

	_ip = szSvrIp;
	_port = port;
	printf("Bind Server Success. IP[%s] Port[%d]\n", szSvrIp, port);

	return 0;
}

void CTcpServer::Start()
{
	_isExit = false;
	_workThread = new std::thread(CTcpServer::OnProcClientData, this);
	printf("Start Server Successful................................\n");
}

void CTcpServer::Stop()
{
	_isExit = true;
}

bool CTcpServer::AcceptClient()
{
	struct sockaddr_in clientAddr = { 0 };
    socklen_t addrLen = sizeof(clientAddr);
    int cientSocket = accept(_socketHandle, (struct sockaddr*)&clientAddr, &addrLen);
    if (cientSocket < 0)
	{
        printf("Accept Client Failed!\n");
		return false;
	}
#ifndef _WIN32
	int keepalive = 1;
	int keepidle = 3;
	int keepinterval = 1;
	int keepcount = 3;
    setsockopt(cientSocket, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepalive , sizeof(keepalive ));
    setsockopt(cientSocket, IPPROTO_TCP, TCP_KEEPALIVE, (void*)&keepidle , sizeof(keepidle ));
    setsockopt(cientSocket, IPPROTO_TCP, TCP_KEEPINTVL, (void *)&keepinterval , sizeof(keepinterval ));
    setsockopt(cientSocket, IPPROTO_TCP, TCP_KEEPCNT, (void *)&keepcount , sizeof(keepcount ));

#else
	bool bKeepAlive = true;
	int ret = ::setsockopt(cientSocket, SOL_SOCKET, SO_KEEPALIVE, (char*)&bKeepAlive, sizeof(bKeepAlive));
	if (ret == SOCKET_ERROR)
	{
		char szError[1024] = { 0 };
		sprintf(szError, "set keepalive fail:%d", GetLastError());
		printf("%s\n", szError);
	}

	tcp_keepalive alive_in = { 0 };
	tcp_keepalive alive_out = { 0 };
	alive_in.keepalivetime = 5000;
	alive_in.keepaliveinterval = 1000;
	alive_in.onoff = 1;
	unsigned long ulBytesReturn = 0;
	ret = WSAIoctl(cientSocket, SIO_KEEPALIVE_VALS, &alive_in, sizeof(alive_in),
		&alive_out, sizeof(alive_out), &ulBytesReturn, NULL, NULL);
	if (ret == SOCKET_ERROR)
	{
		char szError[1024] = { 0 };
		sprintf(szError, "set accept tcpclient SIO_KEEPALIVE_VALS fail:%d", GetLastError());
		printf("%s\n", szError);
	}
#endif
	std::string ip = inet_ntoa(clientAddr.sin_addr);
	int port = ntohs(clientAddr.sin_port);
	printf("Accept client, info[%s : %d]\n", ip.c_str(), port);

	AddFD(cientSocket, ip, port);
	
	return true;
}

void CTcpServer::OnProcClientData(void* context)
{
	CTcpServer* server = static_cast<CTcpServer*>(context);
	while (!server->_isExit)
	{
		fd_set rfd;
		FD_ZERO(&rfd);
		FD_SET(server->_socketHandle, &rfd);
        int maxFd = server->_socketHandle;
        std::map<int, TSockClientInfo>::iterator iter = server->_clientFds.begin();
		for (; iter != server->_clientFds.end();  ++iter)
		{
			FD_SET(iter->first, &rfd);
            if (iter->first > maxFd){
                maxFd = iter->first;
            }
		}

		struct timeval timeout;
		timeout.tv_sec = 3;
		timeout.tv_usec = 0;

        int ret = select(maxFd + 1, &rfd, NULL, NULL, &timeout);
        if (ret == 0 || ret == -1)
		{
            continue;
		}

        if (FD_ISSET(server->_socketHandle, &rfd)){
            server->AcceptClient();
        }
        else
        {
            iter = server->_clientFds.begin();
            for (; iter != server->_clientFds.end();  ++iter)
            {
                if (FD_ISSET(iter->first, &rfd))
                {
					if (server->RecvData(iter->first) < 0){
						break;
					}
                }
            }
        }
	}

	printf("Server thread stop.....\n");
}

int CTcpServer::RecvData(int fd)
{
	char szBuf[8192] = { 0 };
    int readNum = 0;
 #ifdef WIN32
    readNum = recv(fd, szBuf, sizeof(szBuf), 0);
	if (readNum < 0)
	{
		int lastErr = WSAGetLastError();
		char szError[1024] = { 0 };

		sprintf(szError, "Recv data failed.Error is %d", lastErr);
		printf("%s\n", szError);
		_error = szError;

		DelFD(fd);
		return -1;
	}
	else if (readNum == 0)
	{
		printf("The Client have been cloesed.");
		DelFD(fd);
		return -1;
	}
#else
    readNum = read(fd, szBuf, sizeof(szBuf));
    if (readNum < 0)
    {
		char szError[1024] = {0};
		sprintf(szError, "Recv data failed. Error is %s", strerror(errno));
        printf("%s\n", szError);
        _error = szError;

        DelFD(fd);
        return -1;
    }
    else if (readNum == 0)
    {
        printf("The Client have been cloesed.");
        DelFD(fd);
        return -1;
    }
#endif
	std::string data;
	data.assign(szBuf, readNum);
	if (_recvDataCallback != NULL)
	{
		_recvDataCallback(fd, data, _recvDataContext);
	}

	printf("fd=%d, Recv data=%s\n", fd, data.c_str());
	return readNum;
}

void CTcpServer::AddFD(int fd, const std::string& ip, int port)
{
	TSockClientInfo sockClient;
	sockClient.fd = fd;
	sockClient.ip = ip;
	sockClient.port = port;
	_clientFds[fd] = sockClient;

	if (_fdCallback != NULL)
	{
		_fdCallback(FD_CONNECT_STATE, fd, ip, port, _fdContext);
	}
}

void CTcpServer::DelFD(int fd)
{
	TSockClientInfo clientInfo;
	GetFdDetailInfo(fd, clientInfo);
    std::map<int, TSockClientInfo>::iterator iter = _clientFds.find(fd);
	if (iter != _clientFds.end())
	{
		_clientFds.erase(iter);
		printf("Del fd success. fd=%d, ip=%s, port=%d\n", fd, clientInfo.ip.c_str(), clientInfo.port);
	}
	else
	{
		printf("Del fd failed. fd=%d, ip=%s, port=%d\n", fd, clientInfo.ip.c_str(), clientInfo.port);
		return;
	}

	if (_fdCallback != NULL)
	{
		_fdCallback(FD_DISCONNECT_STATE, fd, clientInfo.ip, clientInfo.port, _fdContext);
	}
	CloseSocketHandle(fd);
}

bool CTcpServer::GetFdDetailInfo(int fd, TSockClientInfo& clientInfo)
{
    std::map<int, TSockClientInfo>::iterator iter = _clientFds.find(fd);
	if (iter == _clientFds.end())
	{
		printf("Get fd detail info failed. fd=%d\n", fd);
		return false;
	}
	
	clientInfo = iter->second;
	return true;
}

void CTcpServer::CloseSocketHandle(int& socketHandle)
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

int CTcpServer::SendString(int fd, const std::string& data)
{
	return Send(fd, data.c_str(), data.length());
}

int CTcpServer::Send(int fd, const char* data, size_t len)
{
	int haveSend = 0;
	int left = len;
	while (left > 0)
	{
#ifdef WIN32
		haveSend = send(fd, data, left, 0);
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
			_error = szError;
			return -1;
		}
#else
        haveSend = write(fd, data, left);
        if (haveSend < 0)
        {
           if (errno == EINTR || EWOULDBLOCK  == errno)
           {
               haveSend = 0;
               continue;
           }

            char szError[1024];
            sprintf(szError, "Send data failed. Error is %s", strerror(errno));
            printf("%s\n", szError);
            _error = szError;
            return -1;
        }
#endif
		left -= haveSend;
		data += haveSend;
	}

	return haveSend;
}

int CTcpServer::SendAll(const std::string& data)
{
	return SendAll(data.c_str(), data.length());
}

int CTcpServer::SendAll(const char* pbuf, int buflen)
{
	if (_clientFds.empty()){
		printf("There have no clients connect to server now, can not send to all.");
		return 1;
	}

    std::map<int, TSockClientInfo>::iterator iter = _clientFds.begin();
	for (; iter != _clientFds.end(); ++iter)
	{
		if (Send(iter->first, pbuf, buflen) < 0)
			return -1;
	}

	return 0;
}

int CTcpServer::DisConnectFd(unsigned int fd)
{
    std::map<int, TSockClientInfo>::iterator iter = _clientFds.find(fd);
	if (iter == _clientFds.end()) {
		return -1;
	}
	int fds = iter->first;
	_clientFds.erase(iter);	
	CloseSocketHandle(fds);
	return 0;
}
