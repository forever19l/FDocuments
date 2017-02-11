#pragma once

/***************************************************************
* Purpose:   Redis封装类
* Author:    wuzx ()
* Created:   2016-08-28
**************************************************************/

#include <string>
#include <vector>
#include <map>
#include "hiredis.h"

class CRedis
{
public:

	CRedis();
	~CRedis();

	//连接服务器  返回值 0：success; other:failed.
	int Connect(const char* szIp, const int port);

	//退出连接  返回值 0：success; other:failed.
	int Quit();

	//判断是否存在指定键  返回值 0：success; other:failed.
	int IsExistKey(const char* szKey);

	//获取list的长度
	long long GetListLen(const char* szKey);

	//从list中获取一个元素 返回值 0：success; other:failed.
	int GetListElement(const char* szKey, long long index, std::string& value);

	//获取列表所有元素 返回值 0：success; other:failed.
	int GetListAllElement(const char* szKey, std::vector<std::string>& items);

	//判断hash的值是否存在 返回值 0：success; other:failed.
	int IsHashKeyExist(const char* szKey, const char* szField);

	//获取hash指定field的值
	int GetHashElement(const char* szKey, const char* szField, std::string& value);

	const char* GetErrMsg(void) const //获取最后的错误信息
	{
		return _szErrMsg;
	}

public:
	int WriteElement(const std::string& key, const std::string& value);
	int ReadElement(const std::string& key, std::string& value);

	int WriteHashElement(const std::string& key, const std::string& itemField, const std::string& value);
	int ReadHashElement(const std::string& key, const std::string& itemField, std::string& value);

	int WriteHashElement(const std::string& key, const std::string& itemField, const int& value);

private:
	redisReply* ExecCommand(const char* szCommand, bool& ret);

private:
	redisContext* _redisCtx;
	char _szErrMsg[1024];
};

