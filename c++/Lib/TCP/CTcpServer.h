#pragma once
#include <string>
#include <thread>
#include <map>

//type: 0:del fd(client closed or socket failed.);  1:add fd(client connect);
typedef void (*FDCallBack)(int type, unsigned int fd, const std::string& ip, int port, void* context);
typedef void (*RecvClientDataCallBack)(unsigned int fd, const std::string& data, void* context);

struct TSockClientInfo
{
	unsigned int fd;
	std::string ip;
	int port;

	TSockClientInfo()
	{
		fd = 0;
		ip = "";
		port = -9999;
	}
};

class CTcpServer
{
public:
	CTcpServer();
	~CTcpServer();

	int Bind(const char* szSvrIp, int port);
	void Close();
	void Start();
	void Stop();

	int DisConnectFd(unsigned int fd);
	
	void SetFDCallBack(FDCallBack callback, void* context);
	void SetRecvDataCallBack(RecvClientDataCallBack callback, void* context);
	int SendString(int fd, const std::string& data);
	int Send(int fd, const char* data, size_t len);

	int SendAll(const std::string& data);

	std::string GetError() { return _error; }
	std::string GetIp() const { return _ip; }
	int GetPort() const { return _port; }

private:
	bool AcceptClient();
	int RecvData(int fd);

	static void OnProcClientData(void* context);

	void AddFD(unsigned int fd, const std::string& ip, int port);
	void DelFD(unsigned int fd);
	bool GetFdDetailInfo(int fd, TSockClientInfo& clientInfo);

private:
	std::thread* _workThread = NULL;

	FDCallBack _fdCallback = NULL;
	void* _fdContext = NULL;

	RecvClientDataCallBack _recvDataCallback = NULL;
	void* _recvDataContext = NULL;

	bool _isExit = false;
	unsigned int _socketHandle = 0;
	int _port = -9999;
	std::string _ip;
	std::string _error;

	std::map<unsigned int, TSockClientInfo> _clientFds;
};

