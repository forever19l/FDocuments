#include "ZmqPublisher.h"
#include "zmq.h"

#include <iostream>
#define CLI_CR_RED          "\033[31m"
#define CLI_CR_YELLOW       "\033[33m"
#define CLI_CR_CLOSE        "\033[0m"

#ifdef WIN32
#define sprintf sprintf_s
#endif

static std::string GetTime()
{
	std::string strNowTime;
#ifdef _WIN32
	char szTemp[128] = { 0 };
	SYSTEMTIME nowTime;
	GetLocalTime(&nowTime);
	sprintf(szTemp, "%d-%02d-%02d_%02d-%02d-%02d.%03d: ", nowTime.wYear, nowTime.wMonth, nowTime.wDay,
		nowTime.wHour, nowTime.wMinute, nowTime.wSecond, nowTime.wMilliseconds);
	strNowTime = szTemp;
#endif
	return strNowTime;
}

ZmqPublisher::ZmqPublisher()
{
    m_context = NULL;
    m_socket = NULL;
	_ip = "0.0.0.0";
	_port = -9999;
}

ZmqPublisher::~ZmqPublisher()
{
	Close();
}


int ZmqPublisher::Close()
{
	try
	{
		if (m_socket != NULL){

			zmq_close(m_socket);
			zmq_ctx_destroy(m_context);
		}
		return 0;
	}
	catch(...)
	{
		char szError[1024] = { 0 };
		sprintf(szError, "[Publisher] ZmqPublisher Close except append ");
		SetError(std::string(szError));
		std::cerr << CLI_CR_RED << GetTime().c_str() << szError << std::endl << CLI_CR_CLOSE;
		return -1;
	}
}

int ZmqPublisher::Bind(const char* szSvrIp, int port)
{
	char address[64];
	sprintf(address, "tcp://%s:%d", szSvrIp, port);
	return Bind(address);
}

int ZmqPublisher::Bind(const char *address)
{
	try
	{
		m_context = zmq_ctx_new();

		if (!m_context) {
			char szError[1024] = { 0 };
			sprintf(szError, "[Publisher] CPublisher::failed to create context! with error : %s ", zmq_strerror(zmq_errno()));
			SetError(std::string(szError));
			std::cerr << CLI_CR_RED << GetTime().c_str() << szError << std::endl << CLI_CR_CLOSE;
			return -1;
		}

		m_socket = zmq_socket(m_context, ZMQ_PUB);

		if (!m_socket) {
			char szError[1024] = { 0 };
			sprintf(szError, "[Publisher] CPublisher::failed to create publiser socket! with error : %s ", zmq_strerror(zmq_errno()));
			SetError(std::string(szError));
			std::cerr << CLI_CR_RED << GetTime().c_str() << szError << std::endl << CLI_CR_CLOSE;
			return -2;
		}

		int ret = zmq_bind(m_socket, address);
		if (ret <0) {
			char szError[1024] = { 0 };
			sprintf(szError, "[Publisher] CPublisher::Reply socket failed to bind port number :%s  with return value :%d with error :%s", address, ret, zmq_strerror(zmq_errno()));
			SetError(std::string(szError));
			std::cerr << CLI_CR_RED << GetTime().c_str() << szError << std::endl << CLI_CR_CLOSE;
			return ret;
		}
		else
		{
			std::cout << GetTime().c_str() << "[Publisher] " << "CPublisher::Create REPLY server,bind with address :" << address << std::endl;
		}
		int l = 0;
		zmq_setsockopt(m_socket, ZMQ_LINGER, &l, sizeof(l));//fix destroy 

		std::string str = address;
		int pos = str.find("tcp://");
		if (pos >= 0)
		{
			int slen = strlen("tcp://");
			str = str.substr(pos + slen, str.size() - slen);
			pos = str.find(":");
			if (pos >= 0)
			{
				_ip = str.substr(0, pos);
				std::string sport = str.substr(pos + 1, str.size() - pos - 1);
				_port = atoi(sport.c_str());
			}
		}
		return 0;
	}
	catch(...)
	{
		char szError[1024] = { 0 };
		sprintf(szError, "[Publisher] ZmqPublisher Bind except append ");
		SetError(std::string(szError));
		std::cerr << CLI_CR_RED << GetTime().c_str() << szError << std::endl << CLI_CR_CLOSE;
		return -1;
	}
}

int ZmqPublisher::Pulish(void *data, int len)
{
	try
	{
		return zmq_send(m_socket, data, len, 0);
	}
	catch(...)
	{
		char szError[1024] = { 0 };
		sprintf(szError, "[Publisher] ZmqPublisher Pulish except append ");
		SetError(std::string(szError));
		std::cerr << CLI_CR_RED << GetTime().c_str() << szError << std::endl << CLI_CR_CLOSE;
		CallReConnectThr();
		return -1;
	}
}

int ZmqPublisher::PulishString(const char *str)
{
    return Pulish((void *)str, strlen(str));
}

std::string ZmqPublisher::GetError()
{
	std::unique_lock<std::mutex> lock(_mutex);
	return _error;
}

void ZmqPublisher::SetError(const std::string& msg)
{
	std::unique_lock<std::mutex> lock(_mutex);
	_error = msg;
}


int ZmqPublisher::ReConnect()
{
	int iresult = 0;
	Close();
	iresult = Bind(_ip.c_str(), _port);
	return iresult;
}

void ZmqPublisher::ReConnectThrFun(ZmqPublisher* onwer)
{
	onwer->ReConnect();
}

void ZmqPublisher::CallReConnectThr()
{
	DelReconnectThr();
	_reConnectThr = new std::thread(ReConnectThrFun, this);
}

void ZmqPublisher::DelReconnectThr()
{
	if (nullptr != _reConnectThr)
	{
		if (_reConnectThr->joinable())
			_reConnectThr->join();
		delete _reConnectThr;
		_reConnectThr = nullptr;
	}
}