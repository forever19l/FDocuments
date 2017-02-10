#include "ZmqReplyer.h"

#include <iostream>
#include <vector>

#define CLI_CR_RED          "\033[31m"
#define CLI_CR_YELLOW       "\033[33m"
#define CLI_CR_CLOSE        "\033[0m"

ZmqReplyer::ZmqReplyer(void * context)
	: m_context(context)
{
	m_socket = NULL;
}

ZmqReplyer::~ZmqReplyer()
{
	if (m_socket)
	{
		this->Close();
	}
}

void* ZmqReplyer::GetSocket(){
	return m_socket;
}

int ZmqReplyer::Bind(const char*  szSvrIp, int port)
{
	char address[128] = {0};
	sprintf_s(address, "tcp://%s:%d", szSvrIp, port);
	return Bind(address);
}

int ZmqReplyer::Bind(const char *address)
{
	m_context = zmq_ctx_new();

	if (!m_context) 
	{
		std::cerr << CLI_CR_RED << "[Replier] " << "CReplier::failed to create context! with error :" << zmq_strerror(zmq_errno()) << std::endl << CLI_CR_CLOSE;
		return -1;
	}

	m_socket = zmq_socket(m_context, ZMQ_REP);

	if (!m_socket) 
	{
		std::cerr << CLI_CR_RED << "[Replier] " << "CReplier::failed to create publiser socket! with error :" << zmq_strerror(zmq_errno()) << std::endl << CLI_CR_CLOSE;
		return -2;
	}

	int ret = zmq_bind(m_socket, address);
	if (ret <0) 
	{
		std::cerr << CLI_CR_RED << "[Replier] " << "CReplier::Reply socket failed to bind port number :" << address << " with return value :" << ret << "with error :" << zmq_strerror(zmq_errno()) << std::endl << CLI_CR_CLOSE;
		return ret;
	}
	else
	{
		std::cout << "[Replier] " << "CReplier::Create REPLY server,bind with address :" << address << std::endl;
		//m_bMonitorReply = true;
		//m_thread = new std::thread(ZmqReplyer::threadEntry, this);
	}

	return 0;
}

//int ZmqReplyer::resv(char *data)
//{
//	char *buf = new char[4096];
//	memset(buf, 0, 4096);
//	DWORD gotLen = zmq_recv(m_socket, buf, 4096, 0);
//	memcpy(data, buf, 4096);
//	delete[] buf;
//	buf = nullptr;
//	return 0;
//}

//int ZmqReplyer::send(const char *data)
//{
//	char *buf = new char[1024];
//	memcpy(buf, data, strlen(data) + 1);
//	DWORD gotLen = zmq_send(m_socket, buf, strlen(buf), 0);
//	delete[] buf;
//	buf = nullptr;
//	return 0;
//}

int ZmqReplyer::recv_n(char * buffer, int size)
{
	return zmq_recv(m_socket, buffer, size, 0);
}

int ZmqReplyer::send_n(const char * buffer, int length)
{
	return zmq_send(m_socket, buffer, length, 0);
}

int ZmqReplyer::Close()
{
	zmq_close(m_socket);
	m_socket = NULL;
	return 0;
}
