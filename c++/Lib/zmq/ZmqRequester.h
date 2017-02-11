#ifndef ZMQREQUESTER_H
#define ZMQREQUESTER_H

/***************************************************************
 * Purpose:   zmq请求类。
 * Author:    wuzx ()
 * Created:   2016-05-27
 **************************************************************/

#include <string>
#include "zmq.h"
class ZmqRequester
{
public:
    ZmqRequester();
    virtual ~ZmqRequester();

    int Connect(const char* szSvrIp, int port);
    int Connect(const char* address);

    int Close();

	int SendString(const std::string& str);
    int Send(const char* data, size_t len);
    int Recv(char*& buffer, size_t& len);
	int Select();
	void SetTimeout(int timeout);
	int GetTimeout() const { return m_timeout; }

	std::string GetError();
	std::string GetIp() const { return _ip; }
	int GetPort() const { return _port; }

private:
    void* m_context;
    void* m_socket;
    int m_timeout;
	std::string _ip;
	int _port;
};

#endif // ZMQREQUESTER_H
