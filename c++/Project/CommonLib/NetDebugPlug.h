#pragma once
#include "ZmqReplyer.h"
#include "ZmqPublisher.h"


template <class T>
class NetDebugPlug
{
public:
	NetDebugPlug(T* t);
	~NetDebugPlug();
	int CreateIPC(const char* reply, const char* publish);
	void PulishString(const char* buf);
	void Pulish(const char* pbuf, int buflen);
	int Open();
	void Close();
	std::string GetError();
private:
	std::string RecveFromRep(void * buf, long len);
	static std::string RecveFromRepCallBack(void * buf, long len, void * context);
private:
	ZmqReplyer* _zmqRep = NULL;
	ZmqPublisher* _zmqPub = NULL;
	std::string _reply;
	std::string _publish;
	T* _t;
	std::string _error;
};

template <class T>
std::string NetDebugPlug<T>::GetError()
{
	return _error;
}

template <class T>
NetDebugPlug<T>::NetDebugPlug(T* t)
{
	_t = t;
}

template <class T>
NetDebugPlug<T>::~NetDebugPlug()
{
	if (NULL != _zmqPub)
	{
		_zmqPub->Close();
		delete _zmqPub;
	}
	if (NULL != _zmqRep)
	{
		_zmqRep->Close();
		delete _zmqRep;
	}
}

template <class T>
void NetDebugPlug<T>::PulishString(const char* buf)
{
	if (_zmqPub == NULL)
		return;
	_zmqPub->PulishString(buf);
}

template <class T>
void NetDebugPlug<T>::Pulish(const char* pbuf, int buflen)
{
	if (_zmqPub == NULL)
		return;
	_zmqPub->Pulish((void*)pbuf, buflen);
}

template <class T>
int NetDebugPlug<T>::CreateIPC(const char * reply, const char * publish)
{
	int iresult = 0;
	if (NULL == _zmqPub)
	{
		_zmqPub = new ZmqPublisher();
		iresult = _zmqPub->Bind(publish);
		if (0 != iresult)
		{
			_error = _zmqPub->GetError();
			delete _zmqPub;
			_zmqPub = NULL;
			return iresult;
		}
	}

	if (NULL == _zmqRep)
	{
		_zmqRep = new ZmqReplyer();
		_zmqRep->SetCallBack(NetDebugPlug<T>::RecveFromRepCallBack, this);
		iresult = _zmqRep->Bind(reply);
		if (0 != iresult)
		{
			_error = _zmqRep->GetError();
			delete _zmqPub;
			_zmqPub = NULL;
			delete _zmqRep;
			_zmqRep = NULL;
			return iresult;
		}
		_zmqRep->Start();
	}
	_reply = std::string(reply);
	_publish = std::string(publish);
	return 0;
}

template <class T>
std::string NetDebugPlug<T>::RecveFromRep(void * buf, long len)
{
	size_t ibuflen = (size_t)len;
	void* temp = malloc(ibuflen + 1);
	memset(temp, 0, ibuflen + 1);
	memcpy(temp, buf, ibuflen);
	int ilen = _t->Send((char*)temp, ibuflen);
	free(temp);
	std::string sresult = std::to_string(ilen);
	return sresult;
}

template <class T>
std::string NetDebugPlug<T>::RecveFromRepCallBack(void * buf, long len, void * context)
{
	NetDebugPlug* onwer = reinterpret_cast<NetDebugPlug*>(context);
	std::string sresult = onwer->RecveFromRep(buf, len);
	if (!sresult.empty())
	{
		onwer->_zmqRep->SendString(sresult.c_str());
	}
	return sresult;
}

template <class T>
int NetDebugPlug<T>::Open()
{
	if (_zmqPub == NULL)
		return 0;
	int iresult = 0;
	iresult = _zmqPub->Bind(_publish.c_str());
	if (0 != iresult)
	{
		_error = _zmqPub->GetError();
		return iresult;
	}
	iresult = _zmqRep->Bind(_reply.c_str());
	if (0 != iresult)
	{
		_error = _zmqRep->GetError();
		return iresult;
	}
	_zmqRep->Start();
	return 0;
}

template <class T>
void NetDebugPlug<T>::Close()
{
	if (_zmqPub == NULL)
		return;
	_zmqPub->Close();
	_zmqRep->Close();
	delete _zmqPub;
	_zmqPub = NULL;
	delete _zmqRep;
	_zmqRep = NULL;
}


