#include "TcpClientDebug.h"


TcpClientDebug::TcpClientDebug()
{
	_pTcpClient = new CTcpClient();
	_pTcpClient->SetRecvDataCallBack(TcpClientDebug::RecvDataCallBack, this);
	_pTcpClient->SetCloseClientCallBack(TcpClientDebug::CloseClientCallBack, this);
	_pNetDebugPlug = new NetDebugPlug<CTcpClient>(_pTcpClient);
}

TcpClientDebug::~TcpClientDebug()
{
	delete _pNetDebugPlug;
	_pTcpClient->Close();
	delete _pTcpClient;
}

int TcpClientDebug::Open(const char* szSvrIp, int port, const char* reply, const char* publish)
{
	int iresult = _pTcpClient->Connect(szSvrIp, port);
	if (iresult != 0)
	{
		SetError(_pTcpClient->GetError());
		return iresult;
	}
	if (reply != NULL)
	{
		iresult = _pNetDebugPlug->CreateIPC(reply, publish);
		if (iresult != 0)
		{
			_pTcpClient->Close();
			SetError(_pNetDebugPlug->GetError());
			return iresult;
		}
	}
	_pTcpClient->Start();
	_IP = std::string(szSvrIp);
	_Port = port;
	_isConnect = true;
	return iresult;
}

std::string TcpClientDebug::GetError()
{
	return _error;
}

void TcpClientDebug::SetError(const std::string& msg)
{
	_error = msg;
}

int TcpClientDebug::ReOpen()
{
	if (_isConnect)
		return 0;
	int iresult = _pTcpClient->Connect(_IP.c_str(), _Port);
	if (iresult != 0)
	{
		SetError(_pTcpClient->GetError());
		return iresult;
	}
	iresult = _pNetDebugPlug->Open();
	_pTcpClient->Start();
	return iresult;
}

void TcpClientDebug::Close()
{
	
	if (!_isConnect)
		return;
	 _pNetDebugPlug->Close();
	 _pTcpClient->Close();
	 _isConnect = false;
}

int TcpClientDebug::Send(const char* data, size_t len)
{
	if (!_isConnect)
		return -1;
	return _pTcpClient->Send(data, len);
}

void TcpClientDebug::RecvDataCallBack(const std::string& buf, void* context)
{
	TcpClientDebug* client = static_cast<TcpClientDebug*>(context);
	client->_pNetDebugPlug->Pulish(buf.c_str(), buf.size());
	client->_onRecvData(buf, client->_recvDataContext);
}

void TcpClientDebug::CloseClientCallBack(void* context)
{
	TcpClientDebug* client = static_cast<TcpClientDebug*>(context);
	client->_onCloseClient(client->_closeClientContext);
}

void TcpClientDebug::SetRecvDataCallBack(OnRecvDataCallBack callback, void* context)
{
	_onRecvData = callback;
	_recvDataContext = context;
}

void TcpClientDebug::SetCloseClientCallBack(OnCloseClientCallBack callback, void* context)
{
	_onCloseClient = callback;
	_closeClientContext = context;
}