#include "SerialPortDebug.h"


SerialPortDebug::SerialPortDebug()
{
	_pCom = new ASynCom();
	_pCom->SetRecvDataCallBack(SerialPortDebug::RecvDataCallBack, this);
	_pNetDebugPlug = new NetDebugPlug<ASynCom>(_pCom);
}


SerialPortDebug::~SerialPortDebug()
{
	delete _pNetDebugPlug;
	_pCom->Stop();
	delete _pCom;
}

int SerialPortDebug::Open(const SerialPortInfo& info, const char* reply, const char* publish)
{
	return Open(info.name.c_str(), info.baudRate, info.parity, info.dataBits, info.stopBits, reply, publish);
}

int SerialPortDebug::Open(const char* portName, unsigned int baudRate, unsigned int parity,
	unsigned int dataBits, unsigned int stopBits, const char* reply, const char* publish)
{
	if (_isConnect)
		return 0;
	int iresult = 0;
	bool ok = _pCom->Connect(portName, baudRate, parity, dataBits, stopBits);
	if (!ok)
	{
		SetError(_pCom->GetError());
		return -1;
	}
	_comInfo.name.assign(portName);
	_comInfo.baudRate = baudRate;
	_comInfo.dataBits = dataBits;
	_comInfo.stopBits = stopBits;
	_comInfo.parity = parity;

	if (reply != NULL)
	{
		iresult = _pNetDebugPlug->CreateIPC(reply, publish);
		if (iresult != 0)
		{
			_pCom->Close();
			SetError(_pNetDebugPlug->GetError());
			return iresult;
		}
	}
	
	_pCom->Start();
	_isConnect = true;
	return  iresult;
}

void SerialPortDebug::Close()
{
	if (!_isConnect)
		return;
	_pCom->Stop();
	_pCom->Close();
	_pNetDebugPlug->Close();
	_isConnect = false;
}

int SerialPortDebug::Send(const char* data, size_t len)
{
	if (!_isConnect)
		return -1;
	return _pCom->Send(data, len);
}

std::string SerialPortDebug::GetError()
{
	return  _error;
}

void SerialPortDebug::SetRecvDataCallBack(OnRecvDataCallBack callback, void* context)
{
	_onRecvData = callback;
	_recvDataContext = context;
}

void SerialPortDebug::RecvDataCallBack(const std::string& data, void* context)
{
	SerialPortDebug* pCom = reinterpret_cast<SerialPortDebug*>(context);
	pCom->_pNetDebugPlug->Pulish(data.c_str(), data.size());
	pCom->_onRecvData(data, pCom->_recvDataContext);
}

void SerialPortDebug::SetError(const std::string& msg)
{
	_error = msg;
}
