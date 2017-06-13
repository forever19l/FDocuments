#include "ZmqRequester.h"
#include "zmq.h"

#include <iostream>
#include <stdio.h>
#include <vector>
#include <string.h>
#include <assert.h>

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

ZmqRequester::ZmqRequester()
{
    m_context = NULL;
    m_socket = NULL;
    m_timeout = 10000;
	_ip = "0.0.0.0";
	_port = -9999;
	_isBroken = true;
}

ZmqRequester::~ZmqRequester()
{
	DelReconnectThr();
	Close();
}



int ZmqRequester::Connect(const char* szSvrIp, int port)
{
    char address[64];
    sprintf(address, "tcp://%s:%d", szSvrIp, port);
    return Connect(address);
}

int ZmqRequester::Connect(const char* address)
{
	try
	{
		m_context = zmq_ctx_new();
		if (!m_context)
		{
			char szError[1024] = { 0 };
			sprintf(szError, "[Requester] Requester::failed to create context! with error : %s ", zmq_strerror(zmq_errno()));
			SetError(std::string(szError));
			std::cerr << CLI_CR_RED << GetTime().c_str() << szError << std::endl << CLI_CR_CLOSE;
			return -1;
		}

		m_socket = zmq_socket(m_context, ZMQ_REQ);

		if (!m_socket)
		{
			char szError[1024] = { 0 };
			sprintf(szError, "[Requester] Requester::failed to create requester socket! with error : %s ", zmq_strerror(zmq_errno()));
			SetError(std::string(szError));
			std::cerr << CLI_CR_RED << GetTime().c_str() << szError << std::endl << CLI_CR_CLOSE;
			return -1;
		}

		int ret = zmq_connect(m_socket, address);
		if (ret <0)
		{
			char szError[1024] = { 0 };
			sprintf(szError, "[Requester] Requester::Request socket failed to bind port number : %s with return value :%d with error :%s ", address, ret, zmq_strerror(zmq_errno()));
			SetError(std::string(szError));
			std::cerr << CLI_CR_RED << GetTime().c_str() << szError << std::endl << CLI_CR_CLOSE;
			return ret;
		}
		else
		{
			std::cout << GetTime().c_str() << "[Requester] " << "Requester::Connect to server,bind with address :" << address << std::endl;
		}
		int l = 0;
		zmq_setsockopt(m_socket, ZMQ_LINGER, &l, sizeof(l));

		zmq_setsockopt(m_socket, ZMQ_RCVTIMEO, &m_timeout, sizeof(m_timeout));

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
		_isBroken = false;
		return 0;
	}
	catch(...)
	{
		char szError[1024] = { 0 };
		sprintf(szError, "[Requester] ZmqRequester Connect except append ");
		SetError(std::string(szError));
		std::cerr << CLI_CR_RED << GetTime().c_str() << szError << std::endl << CLI_CR_CLOSE;
		_isBroken = true;
		return -1;
	}
    
}

void  ZmqRequester::SetTimeout(int timeout)
{
	m_timeout = timeout;
	zmq_setsockopt(m_socket, ZMQ_RCVTIMEO, &m_timeout, sizeof(m_timeout));
}

int ZmqRequester::Close()
{
	try
	{
		if (m_socket != NULL){
			zmq_close(m_socket);
			//zmq_term(m_context);
			zmq_ctx_destroy(m_context);
			m_socket = NULL;
			m_context = NULL;
			_isBroken = true;
		}
		return 0;
	}
	catch (...)
	{
		char szError[1024] = { 0 };
		sprintf(szError, "[Requester] ZmqRequester Close except append ");
		SetError(std::string(szError));
		std::cerr << CLI_CR_RED << GetTime().c_str() << szError << std::endl << CLI_CR_CLOSE;
		_isBroken = true;
		return -1;
	}
}

int ZmqRequester::SendString(const std::string& str)
{
	return Send(str.c_str(), str.size());
}

int ZmqRequester::Send(const char* data, size_t len)
{
	try
	{
		if (IsBroken())
		{
			CallReConnectThr();
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
			
		static int num = 0;
		int ret = zmq_send(m_socket, data, len, 0);
		if (ret != len)
		{
			if (num != 3)
			{
				num++;
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
				return Send(data, len);
			}
		}
		num = 0;
		return ret;
	}
	catch(...)
	{
		char szError[1024] = { 0 };
		sprintf(szError, "[Requester] ZmqRequester Send except append ");
		SetError(std::string(szError));
		std::cerr << CLI_CR_RED << GetTime().c_str() << szError << std::endl << CLI_CR_CLOSE;
		_isBroken = true;
		CallReConnectThr();
		return -1;
	}
}

int ZmqRequester::Recv(char*& buffer, size_t& len)
{
	try
	{
		std::vector<char> data_buffer;
		zmq_msg_t msg;
		int rc = zmq_msg_init(&msg);
		assert(rc == 0);
		int ret = zmq_msg_recv(&msg, m_socket, 0);
		if (ret >= 0)
		{
			void * pbuffer = zmq_msg_data(&msg);
			len = zmq_msg_size(&msg);
			for (size_t i = 0; i < len; i++)
			{
				data_buffer.push_back(((char *)pbuffer)[i]);
			}
			buffer = new char[len];
			memcpy(buffer, pbuffer, len);

			zmq_msg_close(&msg);
		}
		else{
			//std::cout << GetTime().c_str() << "[Requester:] Requester receive failed:" << zmq_strerror(zmq_errno()) << std::endl;
			zmq_msg_close(&msg);
			_isBroken = true;
			CallReConnectThr();
		}
		return ret;
	}
	catch(...)
	{
		char szError[1024] = { 0 };
		sprintf(szError, "[Requester] ZmqRequester Recv except append ");
		SetError(std::string(szError));
		std::cerr << CLI_CR_RED << GetTime().c_str() << szError << std::endl << CLI_CR_CLOSE;
		CallReConnectThr();
		_isBroken = true;
		return -1;
	}
   
}

int ZmqRequester::Select(int timeout)
{
	try
	{
		zmq_pollitem_t items[1];
		items[0].socket = m_socket;
		items[0].events = ZMQ_POLLIN;

		int rc = zmq_poll(items, ZMQ_POLLIN, timeout);

		return rc;//0：超时; >0:有消息	
	}
	catch(...)
	{
		char szError[1024] = { 0 };
		sprintf(szError, "[Requester] ZmqRequester Select except append ");
		SetError(std::string(szError));
		std::cerr << CLI_CR_RED << GetTime().c_str() << szError << std::endl << CLI_CR_CLOSE;
		_isBroken = true;
		return 0;
	}
}

int ZmqRequester::Reconnect()
{
	if (_isBroken){
		Close();
		return Connect(_ip.c_str(), _port);
	}
	return 0;
}

void ZmqRequester::ReConnectThrFun(ZmqRequester* onwer)
{
	onwer->Reconnect();
}

void ZmqRequester::CallReConnectThr()
{
	DelReconnectThr();
	_reConnectThr = new std::thread(ReConnectThrFun, this);
}

void ZmqRequester::DelReconnectThr()
{
	if (nullptr != _reConnectThr)
	{
		if (_reConnectThr->joinable())
			_reConnectThr->join();
		delete _reConnectThr;
		_reConnectThr = nullptr;
	}
}

std::string ZmqRequester::GetError()
{
	std::unique_lock<std::mutex> lock(_mutex);
	if (!_error.empty())
	{
		_error = zmq_strerror(zmq_errno());
	}
	std::string temp = _error;
	_error.clear();
	return temp;
}

void ZmqRequester::SetError(const std::string& msg)
{
	std::unique_lock<std::mutex> lock(_mutex);
	_error = msg;
}
