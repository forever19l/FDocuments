//#pragma once
#ifndef _TCP_H_
#define _TCP_H_

#include <string>
#include <thread>
#include <mutex>

#define  IO_SIZE 1312

typedef struct DataBody
{
	int m_Function_Code;
	int m_DeviceId;
	double m_Opvalue1;
	double m_Opvalue2;
}CmdBody;

typedef struct DataHeader
{
	unsigned short m_Version1;
	unsigned short m_Version2;
	int m_TransId;
	unsigned short m_CmdType;
	unsigned short m_BlockId;
	unsigned short m_GroupId;
	unsigned short m_CMD_Cnt;
	CmdBody m_CmdBody[10];
}CmdHeader;

typedef struct DataRetBody
{
	int m_Function_Code;
	int m_DeviceId;
	int m_Status_Code;
}RetCmdBody;

typedef struct DataRetHeader
{
	unsigned short m_Version1;
	unsigned short m_Version2;
	int m_TransId;
	unsigned short m_CmdType;
	unsigned short m_BlockId;
	unsigned short m_GroupId;
	unsigned short m_CMD_Cnt;
	RetCmdBody m_CmdBody[10];
}RetCmdHeader;

typedef struct DataIO
{
	bool MachineState[32];
	bool ByteInput[256];
	bool ByteOutput[256];
	int Dint[127];
	double Lreal[256];
}PlcIO;

typedef void (*CloseClientCallBack)(void* context);
typedef void(*RecvDataCallBack)(RetCmdHeader& data, void* context);
typedef void(*RecvIOCallBack)(PlcIO& data, void* context);

class CTcpClient
{
public:
	CTcpClient();
	~CTcpClient();

	int Connect(const char* szSvrIp, int port);
	void Close();
	void CloseIO();
	
	int SendString(const std::string& str);
	int Send(const char* data, size_t len);
	int RecvData(RetCmdHeader& data); 
	int RecvIO(PlcIO& data);
	bool Select(int milisecond);

	void Start();	
	void StartIO();
	void SetRecvDataCallBack(RecvDataCallBack callback, void* context);
	void SetRecvIOCallBack(RecvIOCallBack callback, void* context);
	void SetCloseClientCallBack(CloseClientCallBack callback, void* context);
	
	std::string GetError();

private:
	void Stop();
	static void OnProcServerData(void* context);
	static void OnProcServerIO(void* context);
	void SetError(const std::string& msg);
	void ProcRecvData();
	void ProcRecvIO();

private:
	std::thread* _workThread = NULL;
	std::thread* _IOThread = NULL;
	unsigned int _socketHandle = 0;

	bool _isExit = false;
	std::string _ip;
	int _port = -9999;

	std::string _error;
	RetCmdHeader* data = NULL;
	PlcIO* io = NULL;
	RecvDataCallBack _recvDataCallback = NULL;
	void* _recvDataContext = NULL;

	RecvIOCallBack _recvIOCallback = NULL;
	void* _recvIOContext = NULL;

	CloseClientCallBack _closeClientCallback = NULL;
	void* _closeClientContext = NULL;

	std::mutex _mutex;
};

#endif

