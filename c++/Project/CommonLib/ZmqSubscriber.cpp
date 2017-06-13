#include "ZmqSubscriber.h"
#include "zmq.h"

#include <iostream>
#include <vector>
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

ZmqSubscriber::ZmqSubscriber()
{
	m_pCallBack = NULL;
	m_pCBcontext = NULL;

	m_bMonitorSubscrib = true;

	m_context = NULL;
	m_socket = NULL;
	m_thread = NULL;
	_ip = "0.0.0.0";
	_port = -9999;
}

ZmqSubscriber::~ZmqSubscriber()
{
	DelReconnectThr();
	Close();

}

int ZmqSubscriber::Close()
{
	
	try
	{
		if (m_bMonitorSubscrib)
			m_bMonitorSubscrib = false;

		if (NULL != m_thread)
		{
			if (m_thread->joinable())
				m_thread->join();
			delete m_thread;
			m_thread = NULL;
		}

		if (m_socket != NULL){

			zmq_close(m_socket);
			zmq_ctx_destroy(m_context);
			m_socket = NULL;
		}
		return 0;
	}
	catch (...)
	{
		char szError[1024] = { 0 };
		sprintf(szError, "[Subscriber] ZmqSubscriber Close except append ");
		SetError(std::string(szError));
		std::cerr << CLI_CR_RED << GetTime().c_str() << szError << std::endl << CLI_CR_CLOSE;
		return -1;
	}

}

int ZmqSubscriber::ReConnect()
{
	int iresult = 0;
	Close();
	iresult = Connect(_ip.c_str(), _port);
	Start();
	return iresult;
}

void ZmqSubscriber::ReConnectThrFun(ZmqSubscriber* onwer)
{
	onwer->ReConnect();
}

void ZmqSubscriber::CallReConnectThr()
{
	DelReconnectThr();
	_reConnectThr = new std::thread(ReConnectThrFun, this);
}

void ZmqSubscriber::DelReconnectThr()
{
	if (nullptr != _reConnectThr)
	{
		if (_reConnectThr->joinable())
			_reConnectThr->join();
		delete _reConnectThr;
		_reConnectThr = nullptr;
	}
}

int ZmqSubscriber::Connect(const char* szSvrIp, int port)
{
	char address[64];
	sprintf(address, "tcp://%s:%d", szSvrIp, port);
	return Connect(address);
}

int ZmqSubscriber::Connect(const char *address)
{
	try
	{
		m_context = zmq_ctx_new();
		if (!m_context) {
			char szError[1024] = { 0 };
			sprintf(szError, "[Subscriber] Subscriber::failed to create context! with error : %s ", zmq_strerror(zmq_errno()));
			SetError(std::string(szError));
			std::cerr << CLI_CR_RED << GetTime().c_str() << szError << std::endl << CLI_CR_CLOSE;
			return -1;
		}

		m_socket = zmq_socket(m_context, ZMQ_SUB);


		if (!m_socket) {
			char szError[1024] = { 0 };
			sprintf(szError, "[Subscriber] Subscriber::failed to create subscriber socket! with error : %s ", zmq_strerror(zmq_errno()));
			SetError(std::string(szError));
			std::cerr << CLI_CR_RED << GetTime().c_str() << szError << std::endl << CLI_CR_CLOSE;
			return -2;
		}

		int ret = zmq_setsockopt(m_socket, ZMQ_SUBSCRIBE, m_strFilter.c_str(), m_strFilter.length());
		if (ret != 0) {
			char szError[1024] = { 0 };
			sprintf(szError, "[Subscriber] Subscriber::failed to set subscriber filter! with error : %s ", zmq_strerror(zmq_errno()));
			SetError(std::string(szError));
			std::cerr << CLI_CR_RED << GetTime().c_str() << szError << std::endl << CLI_CR_CLOSE;
			return -3;
		}

		ret = zmq_connect(m_socket, address);
		if (ret < 0)
		{
			char szError[1024] = { 0 };
			sprintf(szError, "[Subscriber] Subscriber::Subscriber socket failed to connect port number : %s with return value :%d with error :%s ", address, ret, zmq_strerror(zmq_errno()));
			SetError(std::string(szError));
			std::cerr << CLI_CR_RED << GetTime().c_str() << szError << std::endl << CLI_CR_CLOSE;

			return ret;
		}
		else
		{
			std::cout << GetTime().c_str() << "[Subscriber] " << "Subscriber::Connect to server,bind with address :" << address << std::endl;
		}

		int l = 0;
		zmq_setsockopt(m_socket, ZMQ_LINGER, &l, sizeof(l));//fix destroy hungs

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
	catch (...)
	{
		char szError[1024] = { 0 };
		sprintf(szError, "[Subscriber] ZmqSubscriber Connect except append ");
		SetError(std::string(szError));
		std::cerr << CLI_CR_RED << GetTime().c_str() << szError << std::endl << CLI_CR_CLOSE;
		return -1;
	}
}

void ZmqSubscriber::Start()
{
	std::cout << GetTime().c_str() << "Subscriber start." << std::endl;
	m_bMonitorSubscrib = true;
	m_thread = new std::thread(ZmqSubscriber::threadEntry, this);
}

int ZmqSubscriber::SetCallBack(SubCallBack cb, void *ctx)
{
	m_pCallBack = cb;
	m_pCBcontext = ctx;
	return 0;
}

int ZmqSubscriber::SetFilter(const char *filter)
{
	m_strFilter = filter;
	return 0;
}


void * ZmqSubscriber::OnSubscriberData(void *pdata, long len)
{
	if (len > 0)
	{
		if (m_pCallBack)
		{  //defaule call cb function.
			const char * pret = (const char *)m_pCallBack(pdata, len, m_pCBcontext);
			return (void *)pret;
		}
	}
	else
	{
		//dump out
		std::cout << GetTime().c_str() << "[Subscriber] Get Data failed" << std::endl;
	}
	return NULL;
}

void * ZmqSubscriber::threadEntry(void *arg)
{
	ZmqSubscriber * pThis = (ZmqSubscriber *)arg;
	try
	{
		while (pThis->m_bMonitorSubscrib)
		{

			zmq_pollitem_t items[1];
			items[0].socket = pThis->m_socket;
			items[0].events = ZMQ_POLLIN;

			int rc = zmq_poll(items, ZMQ_POLLIN, 1000);
			if (rc <= 0)
			{
				continue;
			}

			std::vector<unsigned char> data_buffer;
			data_buffer.clear();

			zmq_msg_t msg;
			zmq_msg_init(&msg);
			zmq_msg_recv(&msg, pThis->m_socket, ZMQ_NOBLOCK);
			void * pbuffer = zmq_msg_data(&msg);
			size_t len = zmq_msg_size(&msg);
			if (len <= 0) {
				zmq_msg_close(&msg);
				continue;
			}

			for (auto i = 0; i < (int)len; i++) {
				data_buffer.push_back(((unsigned char *)pbuffer)[i]);
			}
			zmq_msg_close(&msg);

			while (true)
			{
				//get more data in a frame
				long more;
				size_t more_len = sizeof(more);
				int ret = zmq_getsockopt(pThis->m_socket, ZMQ_RCVMORE, &more, &more_len);
				if (ret < 0)
				{
					std::cout << GetTime().c_str() << "Get More data failed!" << zmq_strerror(zmq_errno()) << std::endl;
					return NULL;
				}
				if (!more)
				{    //no more data
					data_buffer.push_back(0);   //append 0 char
					break;
				}

				zmq_msg_t part;
				zmq_msg_init(&part);
				zmq_msg_recv(&part, pThis->m_socket, ZMQ_NOBLOCK);
				void * pbuffer = zmq_msg_data(&part);
				size_t len = zmq_msg_size(&part);

				data_buffer.push_back('!');//seperator
				data_buffer.push_back('@');
				data_buffer.push_back('#');
				for (auto i = 0; i < (int)len; i++)
				{
					data_buffer.push_back(((unsigned char *)pbuffer)[i]);
				}
				zmq_msg_close(&part);
			}

			pThis->OnSubscriberData(&data_buffer[0], data_buffer.size());
		}
	}

	catch (...)
	{
		char szError[1024] = { 0 };
		sprintf(szError, "[Subscriber] ZmqSubscriber threadEntry except append ");
		pThis->SetError(std::string(szError));
		std::cerr << CLI_CR_RED << GetTime().c_str() << szError << std::endl << CLI_CR_CLOSE;
		pThis->CallReConnectThr();
	}
	return NULL;
}

std::string ZmqSubscriber::GetError()
{
	std::unique_lock<std::mutex> lock(_mutex);
	return _error;
}

void ZmqSubscriber::SetError(const std::string& msg)
{
	std::unique_lock<std::mutex> lock(_mutex);
	_error = msg;
}

