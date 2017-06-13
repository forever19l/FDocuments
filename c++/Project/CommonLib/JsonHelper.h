#ifndef _JSONHELPER_H
#define _JSONHELPER_H

/***************************************************************
 * Purpose:   rapid json解析封装类，方便读写。
 * Author:    wuzx ()
 * Created:   2016-05-27
 **************************************************************/

#include <string>
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/document.h"

class JsonWirter
{
public:
	JsonWirter();
	~JsonWirter();

	void StartArray();
	void EndArray();

	void StartObject();
	void EndObject();

	template<typename R, typename T>
	void WriteJson(const R& key, const T& value)
	{
		WriteValue(key);
		WriteValue(value);
	}

	template<typename T>
	void Write(const T& val)
	{
		WriteValue(val);
	}

	void WriteNull()
	{
		WriteValue();
	}

	void Clear();
	//返回对象序列化后端json字符串
	const char* GetString() const;

private:
	void WriteValue(int value);
	void WriteValue(long unsigned int value);
	void WriteValue(unsigned int value);
	void WriteValue(long long value);
	void WriteValue(bool value);
	void WriteValue(float value);
	void WriteValue(double value);
	void WriteValue(const std::string& value);

	void WriteValue(const char* value);
	void WriteValue(char* value);
	void WriteValue();

private:
	rapidjson::StringBuffer _buf;
	rapidjson::Writer<rapidjson::StringBuffer> _writer;
};


class JsonReader
{
public:
	JsonReader();
	~JsonReader();

	bool ParserJson(const char* json);

    void ResetToRootNode();
	bool SetCurNode(const char* name);

    bool IsObjectNow() const;
    bool IsArrayNow() const;

	int GetRowCount() const;

    //简单的key value 如： {"name":value}
    template<typename T>
    bool GetValue(const std::string& fieldName, T& value) const
    {
        return ReadValue(fieldName, value);
    }

    //数组里包含map对象 如： [{"name":value}, {"name1":valu1}]
	template<typename T>
	bool GetRowFieldValue(int rowIndex, const std::string& fieldName, T& value) const
	{
		if (rowIndex >= (int) _curVal->Size())
			return false;
		return ReadValue(rowIndex, fieldName, value);
	}

    //获取数组里元素，如 ["a", "b", "c"]
    template<typename T>
	bool GetFieldValue(int rowIndex, T& value) const
	{
		if (rowIndex >= (int) _curVal->Size())
			return false;
		return ReadValue(rowIndex, value);
	}


private:
	bool ReadValue(int rowIndex, const std::string& fieldName, int& value) const;
	bool ReadValue(int rowIndex, const std::string& fieldName, unsigned int& value) const;
	bool ReadValue(int rowIndex, const std::string& fieldName, float& value) const;
	bool ReadValue(int rowIndex, const std::string& fieldName, double& value) const;
	bool ReadValue(int rowIndex, const std::string& fieldName, long long& value) const;
	bool ReadValue(int rowIndex, const std::string& fieldName, std::string& value) const;

    bool ReadValue(const std::string& fieldName, int& value) const;
	bool ReadValue(const std::string& fieldName, unsigned int& value) const;
	bool ReadValue(const std::string& fieldName, float& value) const;
	bool ReadValue(const std::string& fieldName, double& value) const;
	bool ReadValue(const std::string& fieldName, long long& value) const;
	bool ReadValue(const std::string& fieldName, std::string& value) const;

	bool ReadValue(int rowIndex, int& value) const;
	bool ReadValue(int rowIndex, unsigned int& value) const;
	bool ReadValue(int rowIndex, float& value) const;
	bool ReadValue(int rowIndex, double& value) const;
	bool ReadValue(int rowIndex, long long& value) const;
	bool ReadValue(int rowIndex, std::string& value) const;

private:
	rapidjson::Document _doc;
	rapidjson::Value* _curVal;
};

#endif // _JSONHELPER_H
