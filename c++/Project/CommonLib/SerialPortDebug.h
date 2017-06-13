#pragma once
#include "NetDebugPlug.h"
#include "SerialPort.h"

class SerialPortDebug
{
public:
	SerialPortDebug();
	~SerialPortDebug();
	int Open(const char* portName, unsigned int baudRate, unsigned int parity,
		unsigned int dataBits, unsigned int stopBits, const char* reply, const char* publish);
	int Open(const SerialPortInfo& info, const char* reply, const char* publish);
	void Close();
	int Send(const char* data, size_t len);
	std::string GetError();
	void SetRecvDataCallBack(OnRecvDataCallBack callback, void* context);
private:
	static void RecvDataCallBack(const std::string& data, void* context);
	void SetError(const std::string& msg);
private:
	NetDebugPlug<ASynCom>* _pNetDebugPlug;
	std::string _error;
	ASynCom* _pCom;
	void* _recvDataContext;
	OnRecvDataCallBack _onRecvData;
	SerialPortInfo _comInfo;
	bool _isConnect{ false };
};

