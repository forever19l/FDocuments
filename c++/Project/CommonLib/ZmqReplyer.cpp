#include "ZmqReplyer.h"
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

ZmqReplyer::ZmqReplyer()
{
	m_pCallBack = NULL;
	m_pCBcontext = NULL;

	m_thread = NULL;
	m_bMonitorReply = true;

	m_context = NULL;
	m_socket = NULL;
}

ZmqReplyer::~ZmqReplyer()
{
	DelReconnectThr();
	Close();
}

int ZmqReplyer::Close()
{
	try
	{
		if (m_bMonitorReply)
			m_bMonitorReply = false;
		if (NULL != m_thread)
		{
			if (m_thread->joinable())
				m_thread->join();
			delete m_thread;
			m_thread = NULL;
		}

		if (m_socket != NULL) {

			zmq_close(m_socket);
			zmq_ctx_destroy(m_context);
		}

		return 0;
	}
	catch (...)
	{
		char szError[1024] = { 0 };
		sprintf(szError, "[Replier] ZmqReplyer Close except append ");
		SetError(std::string(szError));
		std::cerr << CLI_CR_RED << GetTime().c_str() << szError << std::endl << CLI_CR_CLOSE;
		return -1;
	}

}

int ZmqReplyer::Bind(const char* szSvrIp, int port)
{
	char address[64];
	sprintf(address, "tcp://%s:%d", szSvrIp, port);
	return Bind(address);
}

int ZmqReplyer::Bind(const char *address)
{
	try
	{
		m_context = zmq_ctx_new();
		if (!m_context)
		{

			char szError[1024] = { 0 };
			sprintf(szError, "[Replier] CReplier::failed to create context! with error : %s", zmq_strerror(zmq_errno()));
			SetError(std::string(szError));
			std::cerr << CLI_CR_RED << GetTime().c_str() << szError << std::endl << CLI_CR_CLOSE;
			return -1;
		}

		m_socket = zmq_socket(m_context, ZMQ_REP);

		if (!m_socket)
		{
			char szError[1024] = { 0 };
			sprintf(szError, "[Replier] CReplier::failed to create publiser socket! with error : %s", zmq_strerror(zmq_errno()));
			SetError(std::string(szError));
			std::cerr << CLI_CR_RED << GetTime().c_str() << szError << std::endl << CLI_CR_CLOSE;
			return -2;
		}

		int ret = zmq_bind(m_socket, address);
		if (ret <0)
		{
			char szError[1024] = { 0 };
			sprintf(szError, "[Replier] CReplier::Reply socket failed to bind port number : %s  with return value : %d with error : %s", address, ret, zmq_strerror(zmq_errno()));
			SetError(std::string(szError));
			std::cerr << CLI_CR_RED << GetTime().c_str() << szError << std::endl << CLI_CR_CLOSE;
			return ret;
		}
		else
		{
			char szError[1024] = { 0 };
			sprintf(szError, "[Replier] CReplier::Create REPLY server,bind with address : %s", address);
			SetError(std::string(szError));
			std::cout << GetTime().c_str() << szError << std::endl;
		}
		int l = 0;
		zmq_setsockopt(m_socket, ZMQ_LINGER, &l, sizeof(l));

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
		sprintf(szError, "[Replier] ZmqReplyer Bind except append ");
		SetError(std::string(szError));
		std::cerr << CLI_CR_RED << GetTime().c_str() << szError << std::endl << CLI_CR_CLOSE;
		return -1;
	}
}

void ZmqReplyer::Start()
{
	std::cout << "Replier start." << std::endl;
	m_bMonitorReply = true;
	m_thread = new std::thread(ZmqReplyer::threadEntry, this);
}

bool ZmqReplyer::IsStart()
{
	return m_bMonitorReply;
}

int ZmqReplyer::ReConnect()
{
	int iresult = 0;
	Close();
	iresult = Bind(_ip.c_str(), _port);
	Start();
	return iresult;
}

void ZmqReplyer::ReConnectThrFun(ZmqReplyer* onwer)
{
	onwer->ReConnect();
}

void ZmqReplyer::CallReConnectThr()
{
	DelReconnectThr();
	_reConnectThr = new std::thread(ReConnectThrFun, this);
}

void ZmqReplyer::DelReconnectThr()
{
	if (nullptr != _reConnectThr)
	{
		if (_reConnectThr->joinable())
			_reConnectThr->join();
		delete _reConnectThr;
		_reConnectThr = nullptr;
	}
}


int ZmqReplyer::SetCallBack(ReplyCallBack cb, void *ctx)
{
	m_pCallBack = cb;
	m_pCBcontext = ctx;
	return 0;
}

int ZmqReplyer::Send(void *data, int len)
{
	try
	{
		return zmq_send(m_socket, data, len, 0);
	}
	catch (...)
	{
		char szError[1024] = { 0 };
		sprintf(szError, "[Replier] ZmqReplyer Send except append ");
		SetError(std::string(szError));
		std::cerr << CLI_CR_RED << GetTime().c_str() << szError << std::endl << CLI_CR_CLOSE;
		CallReConnectThr();
		return -1;
	}
}

int ZmqReplyer::SendString(const char *buffer)
{
	if (!buffer)
	{
		return -1;
	}
	return Send((void *)buffer, strlen(buffer));
}

void * ZmqReplyer::OnRequest(void *pdata, long len)
{
	if (len > 0)
	{
		if (m_pCallBack)
		{
			std::string pret = m_pCallBack(pdata, len, m_pCBcontext);
			if (!pret.empty()){ //if pret is not empty, send it. else pret is empty, need to send by manual call send function later
				SendString(pret.c_str());  //send back to requester
			}
		}
		else
		{//no call back send source to sender
			Send(pdata, len);
		}
	}
	else
	{
		//dump out
		std::cout << GetTime().c_str() << "[Replier] Get Data :" << (const char *)pdata << std::endl;
	}
	return NULL;
}

//
//#ifdef WIN32
//
//int ZmqReplyer::exception_access_violation_filter(LPEXCEPTION_POINTERS p_exinfo)
//{
//	if (p_exinfo == nullptr)
//		return -1;
//	if (p_exinfo->ExceptionRecord->NumberParameters == 1)
//	{
//		SetError(std::string((const char*)p_exinfo->ExceptionRecord->ExceptionInformation[0]));
//		std::cerr << CLI_CR_RED << (const char*)p_exinfo->ExceptionRecord->ExceptionInformation[0] << std::endl << CLI_CR_CLOSE;
//	}
//	if (p_exinfo->ExceptionRecord->ExceptionFlags == EXCEPTION_NONCONTINUABLE)
//	{
//		return -1;
//	}
//	return 0;
//}
//#endif


void * ZmqReplyer::threadEntry(void *arg)
{
	ZmqReplyer * pThis = (ZmqReplyer *)arg;
	try
	{
		while (pThis->m_bMonitorReply)
		{

			zmq_pollitem_t items[1];
			items[0].socket = pThis->m_socket;
			items[0].events = ZMQ_POLLIN;

			int rc = zmq_poll(items, 1, 1000);
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
			if (len <= 0)
			{
				zmq_msg_close(&msg);
				continue;
			}

			for (auto i = 0; i<(int)len; i++)
			{
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
					break;
				}

				zmq_msg_t msg;
				zmq_msg_init(&msg);
				zmq_msg_recv(&msg, pThis->m_socket, ZMQ_NOBLOCK);
				void * pbuffer = zmq_msg_data(&msg);
				size_t len = zmq_msg_size(&msg);

				for (auto i = 0; i<(int)len; i++)
				{
					data_buffer.push_back(((unsigned char *)pbuffer)[i]);
				}
				zmq_msg_close(&msg);

			}

			pThis->OnRequest(&data_buffer[0], data_buffer.size());
		}

	}
	catch (...)
	{
		char szError[1024] = { 0 };
		sprintf(szError, "[Replier] threadEntry Send except append ");
		pThis->SetError(std::string(szError));
		std::cerr << CLI_CR_RED << GetTime().c_str() << szError << std::endl << CLI_CR_CLOSE;
		pThis->CallReConnectThr();
	}
	return NULL;
}

std::string ZmqReplyer::GetError()
{
	std::unique_lock<std::mutex> lock(_mutex);
	return _error;
}

void ZmqReplyer::SetError(const std::string& msg)
{
	std::unique_lock<std::mutex> lock(_mutex);
	_error = msg;
}
