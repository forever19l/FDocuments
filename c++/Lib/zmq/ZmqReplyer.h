#ifndef ZMQREPLYER_H
#define ZMQREPLYER_H

#include <thread>
#include "zmq.h"

//typedef void * (*ReplayCallBack)(void * buf, long len, void * context);

class ZmqReplyer
{
public:
    ZmqReplyer(void * context);
    virtual ~ZmqReplyer();

	int Bind(const char* szSvrIp, int port);
	int Bind(const char * address);
	int Close();

	int recv_n(char * buffer, int size);
	int send_n(const char * buffer, int length);

	void * GetSocket();
private:
	//int send(const char *data);
	//int SetCallBack(ReplayCallBack cb, void * ctx);
	
private:
	//int resv(char *data);
	// int Send(void * data, long len);
	//int SendStrig(const char * buffer);
	void* OnRequest(void *pdata, long len);

private:
	//static void * threadEntry(void * arg);
	//bool m_bMonitorReply;
	void * m_context;
	void * m_socket;
	//std::thread* m_thread;
	//ReplayCallBack m_pCallBack;
	//void * m_pCBcontext;
};

#endif // ZMQREPLYER_H
