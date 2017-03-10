#include "CTcpServer.h"

#include <Windows.h>
#include <iostream>
#include <stdio.h>

#pragma comment(lib,"wsock32.lib")

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
	}

	std::map<unsigned int, TSockClientInfo>::iterator iter = _clientFds.begin();
	for (; iter != _clientFds.end(); ++iter)
	{
		closesocket(iter->first);
	}

	_clientFds.clear();

	if (_socketHandle != 0)
	{
		closesocket(_socketHandle);
		_socketHandle = 0;
	}
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

	struct sockaddr_in LocalAddr;
	LocalAddr.sin_family = AF_INET;
	LocalAddr.sin_addr.s_addr = inet_addr(szSvrIp);
	LocalAddr.sin_port = htons(port);
	memset(LocalAddr.sin_zero, 0x00, 8);

	int ret = bind(_socketHandle, (struct sockaddr*)&LocalAddr, sizeof(LocalAddr));
	if (ret != 0)
	{
		printf("Bind Socket Failed! Error is %d\n", GetLastError());
		return -1;
	}

	ret = listen(_socketHandle, 10);
	if (ret != 0)
	{
		printf("listen Socket Failed! Error is %d\n", GetLastError());
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
	struct sockaddr_in clientAddr;
	int addrLen = sizeof(clientAddr);
	SOCKET cientSocket = accept(_socketHandle, (struct sockaddr*)&clientAddr, &addrLen);
	if (cientSocket == INVALID_SOCKET)
	{
		printf("Accept Client Failed! Error is %d\n", GetLastError());
		return false;
	}

	std::string ip = inet_ntoa(clientAddr.sin_addr);
	int port = clientAddr.sin_port;
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

		std::map<unsigned int, TSockClientInfo>::iterator iter = server->_clientFds.begin();
		for (; iter != server->_clientFds.end();  ++iter)
		{
			FD_SET(iter->first, &rfd);
		}

		struct timeval timeout;
		timeout.tv_sec = 3;
		timeout.tv_usec = 0;

		int ret = select(0, &rfd, NULL, NULL, &timeout);
		if (ret == 0 || ret == SOCKET_ERROR)
		{
			continue;
		}

		for (unsigned int i = 0; i < rfd.fd_count; ++i)
		{
			if (server->_socketHandle == rfd.fd_array[i])
			{
				server->AcceptClient();
			}
			else
			{
				server->RecvData(rfd.fd_array[i]);
			}
		}
	}

	printf("Server thread stop.....\n");
}

int CTcpServer::RecvData(int fd)
{
	char szBuf[2048] = { 0 };
	int readNum = recv(fd, szBuf, sizeof(szBuf), 0);
	if (readNum < 0)
	{
		int lastErr = WSAGetLastError();
		char szError[1024];
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
		return 0;
	}

	std::string data = szBuf;

	if (_recvDataCallback != NULL)
	{
		_recvDataCallback(fd, data, _recvDataContext);
	}

	printf("fd=%d, Recv data=%s\n", fd, data.c_str());
	return 0;
}

void CTcpServer::AddFD(unsigned int fd, const std::string& ip, int port)
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

void CTcpServer::DelFD(unsigned int fd)
{
	TSockClientInfo clientInfo;
	GetFdDetailInfo(fd, clientInfo);
	std::map<unsigned int, TSockClientInfo>::iterator iter = _clientFds.find(fd);
	if (iter != _clientFds.end())
	{
		_clientFds.erase(iter);
		printf("Del fd success. fd=%d, ip=%s, port=%d\n", fd, clientInfo.ip.c_str(), clientInfo.port);
	}
	else
	{
		printf("Del fd failed. fd=%d, ip=%s, port=%d\n", fd, clientInfo.ip.c_str(), clientInfo.port);
	}

	if (_fdCallback != NULL)
	{
		_fdCallback(FD_DISCONNECT_STATE, fd, clientInfo.ip, clientInfo.port, _fdContext);
	}
}

bool CTcpServer::GetFdDetailInfo(int fd, TSockClientInfo& clientInfo)
{
	std::map<unsigned int, TSockClientInfo>::iterator iter = _clientFds.find(fd);
	if (iter == _clientFds.end())
	{
		printf("Get fd detail info failed. fd=%d\n", fd);
		return false;
	}
	
	clientInfo = iter->second;
	return true;
}

int CTcpServer::SendString(int fd, const std::string& data)
{
	return Send(fd, data.c_str(), data.size());
}

int CTcpServer::Send(int fd, const char* data, size_t len)
{
	int haveSend = 0;
	int left = len;
	while (left > 0)
	{
		haveSend = send(fd, data, left, 0);
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

int CTcpServer::SendAll(const std::string& data)
{
	std::map<unsigned int, TSockClientInfo>::iterator iter = _clientFds.begin();
	for (; iter != _clientFds.end(); ++iter)
	{
		if (0 != SendString(iter->first, data))
			return -1;
	}

	return 0;
}

int CTcpServer::DisConnectFd(unsigned int fd)
{
	std::map<unsigned int, TSockClientInfo>::iterator iter = _clientFds.find(fd);
	if (iter == _clientFds.end()) {
		return -1;
	}

	_clientFds.erase(iter);
	closesocket(fd);
	
	return 0;
}