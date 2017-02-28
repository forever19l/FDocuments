
#include "CTcpClient.h"
#include "Beckhoff.h"
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
	delete data;
	delete io;
	//Close();
}

void CTcpClient::SetRecvDataCallBack(RecvDataCallBack callback, void* context)
{
	_recvDataCallback = callback;
	_recvDataContext = context;
}

void  CTcpClient::SetRecvIOCallBack(RecvIOCallBack callback, void* context)
{
	_recvIOCallback = callback;
	_recvIOContext = context;
}

void CTcpClient::SetCloseClientCallBack(CloseClientCallBack callback, void* context)
{
	_closeClientCallback = callback;
	_closeClientContext = context;
}

int CTcpClient::Connect(const char* szSvrIp, int port)
{
	if (port == 4999)
		io = new PlcIO();
	else
		data = new RetCmdHeader();

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

void CTcpClient::CloseIO()
{
	if (!_isExit && _IOThread != NULL && _IOThread->joinable())
	{
		_isExit = true;
		_IOThread->join();
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
	int haveSend = send(_socketHandle, data, len, 0);
	if (haveSend < 0)
	{
		int lastErr = WSAGetLastError();
		char szError[1024];
		sprintf(szError, "Send data failed. Error is %d", lastErr);
		printf("%s\n", szError);
		SetError(szError);

		return -1;
	}
	
	return haveSend;
}

int CTcpClient::RecvData(RetCmdHeader& data) 
{
	char szBuf[136] = { 0 };
	int readNum = recv(_socketHandle, szBuf, sizeof(szBuf), 0);
	if (readNum < 0)
	{
		int lastErr = WSAGetLastError();
		char szError[1024];
		sprintf(szError, "Recv data failed. Error is %d", lastErr);
		printf("%s\n", _error);
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
	memcpy(&data, szBuf, sizeof(szBuf));
	return readNum;
}

int CTcpClient::RecvIO(PlcIO& data)
{
	char szBuf[IO_SIZE] = { 0 };
	int readNum = recv(_socketHandle, szBuf, sizeof(szBuf), 0);
	if (readNum < 0)
	{
		int lastErr = WSAGetLastError();
		char szError[1024];
		sprintf(szError, "Recv data failed. Error is %d", lastErr);
		printf("%s\n", _error);
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

	/*char io[IO_SIZE] = { 0 };
	strncpy(io, szBuf + readNum - IO_SIZE, IO_SIZE);
	memcpy(&data, io, sizeof(io));*/
	memcpy(&data, szBuf, sizeof(szBuf));
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

	int ret = select(_socketHandle+1, &rfd, NULL, NULL, &timeout);
	if (ret == 0 || ret == SOCKET_ERROR){ 
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

void CTcpClient::OnProcServerIO(void* context)
{
	CTcpClient* client = static_cast<CTcpClient*>(context);
	while (!client->_isExit){
		client->ProcRecvIO();
	}
}

void CTcpClient::ProcRecvData()
{
	if (Select(100))
	{
		int count = RecvData(*data);
		if (count  < 0){
			return;
		}

		if (_recvDataCallback != NULL)
		{
			_recvDataCallback(*data, _recvDataContext);
		}
	}
}

void CTcpClient::ProcRecvIO()
{
	if (Select(10))
	{
		int count = RecvIO(*io);
		if (count < 0){
			return;
		}

		if (_recvIOCallback != NULL)
		{
			_recvIOCallback(*io, _recvIOContext);
		}
	}
}

void CTcpClient::Start()
{
	_isExit = false;
	_workThread = new std::thread(CTcpClient::OnProcServerData, this);
}

void CTcpClient::StartIO()
{
	_isExit = false;
	_IOThread = new std::thread(CTcpClient::OnProcServerIO, this);
}

void CTcpClient::Stop()
{
	_isExit = true;

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
