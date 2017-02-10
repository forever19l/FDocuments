#include "ZmqRequester.h"

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
ZmqRequester::ZmqRequester()
{
    m_context = NULL;
    m_socket = NULL;
    m_timeout = 3000;
	_ip = "0.0.0.0";
	_port = -9999;
}

ZmqRequester::~ZmqRequester()
{

}

int ZmqRequester::Connect(const char* szSvrIp, int port)
{
    char address[64];
    sprintf(address, "tcp://%s:%d", szSvrIp, port);
    return Connect(address);
}

int ZmqRequester::Connect(const char* address)
{
    m_context = zmq_ctx_new();
    if (!m_context)
    {
        std::cerr<<CLI_CR_RED<<"[Requester] "<< "Requester::failed to create context! with error :"<<zmq_strerror(zmq_errno())<<std::endl<<CLI_CR_CLOSE;
        return -1;
    }

    m_socket = zmq_socket(m_context, ZMQ_REQ);

    if (!m_socket)
    {
        std::cerr<<CLI_CR_RED<<"[Requester] " << "Requester::failed to create requester socket! with error :"<<zmq_strerror(zmq_errno())<<std::endl<<CLI_CR_CLOSE;
        return -1;
    }

    int ret = zmq_connect(m_socket, address);
    if (ret <0)
    {
        std::cerr<<CLI_CR_RED<<"[Requester] " << "Requester::Request socket failed to bind port number :"<<address<<" with return value :"<<ret<<"with error :"<<zmq_strerror(zmq_errno())<<std::endl<<CLI_CR_CLOSE;
        return ret;
    }
    else
    {
        std::cout<<"[Requester] "<< "Requester::Connect to server,bind with address :"<<address<<std::endl;
    }

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

    return 0;
}

void  ZmqRequester::SetTimeout(int timeout)
{
	m_timeout = timeout;
	zmq_setsockopt(m_socket, ZMQ_RCVTIMEO, &m_timeout, sizeof(m_timeout));
}

int ZmqRequester::Close()
{
	int l = 0;
	zmq_setsockopt(m_socket, ZMQ_LINGER, &l, sizeof(l));//fix destroy hungs
	zmq_close(m_socket);
	//zmq_term(m_context);
	zmq_ctx_destroy(m_context);
    return 0;
}

int ZmqRequester::SendString(const std::string& str)
{
	return Send(str.c_str(), str.size());
}

int ZmqRequester::Send(const char* data, size_t len)
{
    int ret = zmq_send(m_socket, data, len, 0);
    return ret;
}

std::string ZmqRequester::GetError()
{
	std::string str = zmq_strerror(zmq_errno());;
	return str;
}

int ZmqRequester::Recv(char*& buffer, size_t& len)
{
    std::vector<char> data_buffer;
    zmq_msg_t msg;
    int rc = zmq_msg_init(&msg);
    assert (rc == 0);
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
    }
    else{
        std::cout<<"[Requester:] Requester receive failed:"<<zmq_strerror(zmq_errno())<<std::endl;
    }

    zmq_msg_close(&msg);
    return ret;
}

int ZmqRequester::Select()
{
	zmq_pollitem_t items[1];
	items[0].socket = m_socket;
	items[0].events = ZMQ_POLLIN;
	
	int rc = zmq_poll(items, ZMQ_POLLIN, m_timeout);

	return rc;//0：超时; >0:有消息	
}
