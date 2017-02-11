#include <assert.h>
#include "JsonHelper.h"

JsonWirter::JsonWirter() : _writer(_buf)
{
}

JsonWirter::~JsonWirter()
{
}

void JsonWirter::StartArray()
{
	_writer.StartArray();
}

void JsonWirter::EndArray()
{
	_writer.EndArray();
}

void JsonWirter::StartObject()
{
	_writer.StartObject();
}

void JsonWirter::EndObject()
{
	_writer.EndObject();
}

//值为NULL
void JsonWirter::WriteValue()
{
	_writer.Null();
}

void JsonWirter::WriteValue(int value)
{
	_writer.Int(value);
}

void JsonWirter::WriteValue(long unsigned int value)
{
	_writer.Uint(value);
}

void JsonWirter::WriteValue(unsigned int value)
{
	_writer.Uint(value);
}

void JsonWirter::WriteValue(long long value)
{
	_writer.Int64(value);
}

void JsonWirter::WriteValue(bool value)
{
	_writer.Bool(value);
}

void JsonWirter::WriteValue(float value)
{
	_writer.Double((double)value);
}

void JsonWirter::WriteValue(double value)
{
	_writer.Double(value);
}

void JsonWirter::WriteValue(const std::string& value)
{
	_writer.String(value.c_str());
}

void JsonWirter::WriteValue(const char* value)
{
	assert(value != NULL);
	_writer.String(value);
}

void JsonWirter::WriteValue(char* value)
{
	assert(value != NULL);
	_writer.String(value);
}

void JsonWirter::Clear()
{
	_buf.Clear();
}

//返回对象序列化后端json字符串
const char* JsonWirter::GetString() const
{
	return _buf.GetString();
}

int JsonWirter::GetLength()const
{
	return (int)_buf.GetSize();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
JsonReader::JsonReader()
{
    _curVal = NULL;
}

JsonReader::~JsonReader()
{

}
bool JsonReader::ParserJson(const char* json)
{
	_doc.Parse<0>(json);
	if (_doc.HasParseError())
	{
		return false;
	}
	else
	{
	    _curVal = &_doc;
		return true;
	}
}

bool JsonReader::SetCurNode(const char* name)
{
    if (_curVal->IsObject()&&_curVal->HasMember(name))
    {
        _curVal = &((*_curVal)[name]);
        return true;
    }

    return false;
}

void JsonReader::ResetToRootNode()
{
     _curVal = &_doc;
}

bool JsonReader::IsObjectNow() const
{
    if (_curVal == NULL)
        return false;

    return _curVal->IsObject();
}

bool JsonReader::IsArrayNow() const
{
    if (_curVal == NULL)
        return false;

    return _curVal->IsArray();
}

int JsonReader::GetRowCount() const
{
    if (!_curVal->IsArray())
        return -9999;
	return _curVal->Size();
}

bool JsonReader::ReadValue(int rowIndex, const std::string& fieldName, int& value) const
{
    if (!_curVal->IsArray())
        return false;

	bool result = false;
	if ((*_curVal)[rowIndex][fieldName.c_str()].IsInt())
	{
		value = (*_curVal)[rowIndex][fieldName.c_str()].GetInt();
		result = true;
	}
	return result;
}

bool JsonReader::ReadValue(int rowIndex, const std::string& fieldName, unsigned int& value) const
{
    if (!_curVal->IsArray())
        return false;
	bool result = false;
	if ((*_curVal)[rowIndex][fieldName.c_str()].IsUint())
	{
		value = (*_curVal)[rowIndex][fieldName.c_str()].GetUint();
		result = true;
	}
	return result;
}

bool JsonReader::ReadValue(int rowIndex, const std::string& fieldName, float& value) const
{
    double val = -9999;
	bool ret = ReadValue(rowIndex, fieldName, val);
	if (ret)
        value = val;
    return ret;
}

bool JsonReader::ReadValue(int rowIndex, const std::string& fieldName, double& value) const
{
    if (!_curVal->IsArray())
        return false;

	bool result = false;
	if ((*_curVal)[rowIndex][fieldName.c_str()].IsDouble())
	{
		value = (*_curVal)[rowIndex][fieldName.c_str()].GetDouble();
		result = true;
	}
	return result;
}

bool JsonReader::ReadValue(int rowIndex, const std::string& fieldName, long long& value) const
{
    if (!_curVal->IsArray())
        return false;

	bool result = false;
	if ((*_curVal)[rowIndex][fieldName.c_str()].IsInt64())
	{
		value = (*_curVal)[rowIndex][fieldName.c_str()].GetInt64();
		result = true;
	}
	return result;
}

bool JsonReader::ReadValue(int rowIndex, const std::string& fieldName, std::string& value) const
{
    if (!_curVal->IsArray())
        return false;

	bool result = false;
	if ((*_curVal)[rowIndex][fieldName.c_str()].IsString())
	{
		value = (*_curVal)[rowIndex][fieldName.c_str()].GetString();
		result = true;
	}
	return result;
}

bool JsonReader::ReadValue(const std::string& fieldName, int& value) const
{
    if (!_curVal->HasMember(fieldName.c_str()))
        return false;

    if (!(*_curVal)[fieldName.c_str()].IsInt())
        return false;

    value = (*_curVal)[fieldName.c_str()].GetInt();
    return true;
}

bool JsonReader::ReadValue(const std::string& fieldName, unsigned int& value) const
{
    if (!_curVal->HasMember(fieldName.c_str()))
        return false;

    if (!(*_curVal)[fieldName.c_str()].IsUint())
        return false;

    value = (*_curVal)[fieldName.c_str()].GetUint();
    return true;
}

bool JsonReader::ReadValue(const std::string& fieldName, float& value) const
{
    double val = -9999;
    bool ret = ReadValue(fieldName, val);
    if (ret)
        value = val;
    return ret;
}

bool JsonReader::ReadValue(const std::string& fieldName, double& value) const
{
    if (!_curVal->HasMember(fieldName.c_str()))
        return false;

    if (!(*_curVal)[fieldName.c_str()].IsDouble())
        return false;

    value = (*_curVal)[fieldName.c_str()].GetDouble();
    return true;
}

bool JsonReader::ReadValue(const std::string& fieldName, long long& value) const
{
    if (!_curVal->HasMember(fieldName.c_str()))
        return false;

    if (!(*_curVal)[fieldName.c_str()].IsInt64())
        return false;

    value = (*_curVal)[fieldName.c_str()].GetInt64();
    return true;
}

bool JsonReader::ReadValue(const std::string& fieldName, std::string& value) const
{
    if (!_curVal->HasMember(fieldName.c_str()))
        return false;

    if (!(*_curVal)[fieldName.c_str()].IsString())
        return false;

    value = (*_curVal)[fieldName.c_str()].GetString();
    return true;
}

bool JsonReader::ReadValue(int rowIndex, int& value) const
{
    if (!_curVal->IsArray())
        return false;

    bool result = false;
	if ((*_curVal)[rowIndex].IsInt())
	{
		value = (*_curVal)[rowIndex].GetInt();
		result = true;
	}
	return result;
}

bool JsonReader::ReadValue(int rowIndex, unsigned int& value) const
{
    if (!_curVal->IsArray())
        return false;

    bool result = false;
	if ((*_curVal)[rowIndex].IsUint())
	{
		value = (*_curVal)[rowIndex].GetUint();
		result = true;
	}
	return result;
}

bool JsonReader::ReadValue(int rowIndex, float& value) const
{
    double val = -9999;
    bool ret = ReadValue(rowIndex, val);
    if (ret)
        value = val;
    return ret;
}

bool JsonReader::ReadValue(int rowIndex, double& value) const
{
    if (!_curVal->IsArray())
        return false;

    bool result = false;
	if ((*_curVal)[rowIndex].IsDouble())
	{
		value = (*_curVal)[rowIndex].GetDouble();
		result = true;
	}
	return result;
}

bool JsonReader::ReadValue(int rowIndex, long long& value) const
{
    if (!_curVal->IsArray())
        return false;

    bool result = false;
	if ((*_curVal)[rowIndex].IsInt64())
	{
		value = (*_curVal)[rowIndex].GetInt64();
		result = true;
	}
	return result;
}

bool JsonReader::ReadValue(int rowIndex, std::string& value) const
{
     if (!_curVal->IsArray())
        return false;

    bool result = false;
	if ((*_curVal)[rowIndex].IsString())
	{
		value = (*_curVal)[rowIndex].GetString();
		result = true;
	}
	return result;
}
