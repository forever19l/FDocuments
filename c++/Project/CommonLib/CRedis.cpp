#include "CRedis.h"
#include <stdio.h>
#include <stdlib.h>
#ifdef WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#endif

#ifdef WIN32
#define snprintf _snprintf
#endif

#define CHECK_PARAM(param) { \
if (NULL == param) \
	{ \
snprintf(_szErrMsg, sizeof(_szErrMsg), "%s", "Wrong parameter!"); \
return -1; \
	} \
}
CRedis::CRedis()
{
	_redisCtx = NULL;
	memset(_szErrMsg, 0, sizeof(_szErrMsg));
}


CRedis::~CRedis()
{
}

//连接服务器
int CRedis::Connect(const char* szIp, const int port)
{
	struct timeval timeout = { 1,0 };
	_redisCtx = redisConnectWithTimeout(szIp, port, timeout);
	if ((NULL == _redisCtx) || (_redisCtx->err))
	{
		if (NULL != _redisCtx)
		{
            snprintf(_szErrMsg, sizeof(_szErrMsg), "%s", _redisCtx->errstr);
			delete _redisCtx;
			_redisCtx = NULL;
		}
		else
		{
            snprintf(_szErrMsg, sizeof(_szErrMsg), "%s", "Connection error: can't allocate redis context");
		}
		return -1;
	}

	return 0;
}

int CRedis::Quit()
{
	if (_redisCtx == NULL)
		return -1;
	bool ret = false;
	redisReply* reply = ExecCommand("QUIT", ret);
	if (ret)
	{
		freeReplyObject(reply);
		return 0;
	}
		

	return -1;
}

int CRedis::IsExistKey(const char* szKey)
{
	if (_redisCtx == NULL)
		return -1;
	char szCommand[1024];
    snprintf(szCommand, sizeof(szCommand), "EXISTS %s", szKey);
	bool ret = false;
	int res = -1;
	redisReply* reply = ExecCommand(szCommand, ret);
	if (ret)
	{
		if (reply->integer == 1)
			res = 0;
		else
			res = 1;

		freeReplyObject(reply);
	}

	return res;
}

long long CRedis::GetListLen(const char* szKey)
{
	if (_redisCtx == NULL)
		return -1;

	CHECK_PARAM(szKey)

	bool ret = false;
	char szCommand[1024];
    snprintf(szCommand, sizeof(szCommand), "LLEN %s", szKey);
	redisReply* reply = ExecCommand(szCommand, ret);
	if (!ret)
	{
		return -1;
	}

	long long len = reply->integer;
	freeReplyObject(reply);

	return len;
}

int CRedis::GetListElement(const char* szKey, long long index, std::string& value)
{
	if (_redisCtx == NULL)
		return -1;

	CHECK_PARAM(szKey)

	//先获取list的size
	long long len = GetListLen(szKey);
	if (len < 0)
	{
		return -1;
	}

	char szCommand[1024];
    snprintf(szCommand, sizeof(szCommand), "LINDEX %s %lld", szKey, len - index - 1);
	bool ret = false;
	redisReply* reply = ExecCommand(szCommand, ret);
	if (!ret)
	{
		return -1;
	}

	if (REDIS_REPLY_NIL == reply->type)
	{
        snprintf(_szErrMsg, sizeof(_szErrMsg), "%s not exist", szKey);
		freeReplyObject(reply);
		return -2;
	}

	value.assign(reply->str, reply->len);
	return 0;
}

int CRedis::GetListAllElement(const char* szKey, std::vector<std::string>& items)
{
	if (_redisCtx == NULL)
		return -1;
	CHECK_PARAM(szKey)

	char szCommand[1024];
    snprintf(szCommand, sizeof(szCommand), "LRANGE %s 0 -1", szKey);
	bool ret = false;
	redisReply* reply = ExecCommand(szCommand, ret);
	if (!ret)
	{
		return -1;
	}

	for (int idx = reply->elements - 1; idx >= 0; --idx)
	{
		int len = reply->element[idx]->len;
		std::string value;
		value.assign(reply->element[idx]->str, reply->len);
		items.push_back(value);
	}

	freeReplyObject(reply);

	return 0;
}

int CRedis::IsHashKeyExist(const char* szKey, const char* szField)
{
	if (_redisCtx == NULL)
		return -1;

	CHECK_PARAM(szKey)
	CHECK_PARAM(szField)

	char szCommand[1024];
    snprintf(szCommand, sizeof(szCommand), "HEXISTS %s %s", szKey, szField);
	bool ret = false;
	redisReply* reply = ExecCommand(szCommand, ret);
	if (!ret)
	{
		return -1;
	}

	int flag = 1;
	if (reply->integer == 1)
		flag = 0;
	freeReplyObject(reply);
	return flag;
}

int CRedis::GetHashElement(const char* szKey, const char* szField, std::string& value)
{
	if (_redisCtx == NULL)
		return -1;

	CHECK_PARAM(szKey)
	CHECK_PARAM(szField)

	char szCommand[1024];
    snprintf(szCommand, sizeof(szCommand), "HGET  %s %s", szKey, szField);
	bool ret = false;
	redisReply* reply = ExecCommand(szCommand, ret);
    if (REDIS_REPLY_NIL == reply->type)
	{
        snprintf(_szErrMsg, sizeof(_szErrMsg), "%s not exist", szKey);
		freeReplyObject(reply);
		return -1;
	}

	value.assign(reply->str, reply->len);
	freeReplyObject(reply);
	return 0;
}

redisReply* CRedis::ExecCommand(const char* szCommand, bool& ret)
{
	redisReply* reply = (redisReply*)redisCommand(_redisCtx, szCommand);
	if (NULL == reply)
	{
        snprintf(_szErrMsg, sizeof(_szErrMsg), "%s", _redisCtx->errstr);
		ret = false;
		return NULL;
	}

	if (REDIS_REPLY_ERROR == reply->type)
	{
        snprintf(_szErrMsg, sizeof(_szErrMsg), "%s", reply->str);
		ret = false;
		delete reply;
		return NULL;
	}

	ret = true;
	return reply;
}
