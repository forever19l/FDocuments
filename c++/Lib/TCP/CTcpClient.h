#pragma once
#include <string>
#include <thread>

typedef void (*RecvDataCallBack)(const std::string& data, void* context);
typedef void (*CloseClientCallBack)(void* context);
class CTcpClient
{
public:
	CTcpClient();
	~CTcpClient();

	int Connect(const char* szSvrIp, int port);
	void Close();
	void Start();
	void Stop();
	void SetTimeout(int timeout) { _timeOut = timeout; }
	int SendString(const std::string& str);
	int Send(const char* data, size_t len);
	
	void SetRecvDataCallBack(RecvDataCallBack callback, void* context);
	void SetCloseClientCallBack(CloseClientCallBack callback, void* context);

	std::string GetError() { return _error; }
	std::string GetIp() const { return _ip; }
	int GetPort() const { return _port; }

private:
	static void OnProcServerData(void* context);
	int RecvData();
	bool Select();

private:
	std::thread* _workThread = NULL;
	unsigned int _socketHandle = 0;

	bool _isExit = false;
	std::string _ip;
	int _port = -9999;
	int _timeOut = 3000;

	std::string _error;

	RecvDataCallBack _recvDataCallback = NULL;
	void* _recvDataContext = NULL;

	CloseClientCallBack _closeClientCallback = NULL;
	void* _closeClientContext = NULL;
};

