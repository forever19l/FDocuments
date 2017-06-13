#pragma once

/***************************************************************
* Purpose:   File·â×°Àà
* Author:    wuzx ()
* Created:   2017-01-12
**************************************************************/


#include <string>

class FileUtil
{
public:
	FileUtil();
	FileUtil(const std::string& filePath);

	~FileUtil();
	void SetPath(const std::string& filePath);
	bool IsExist();
	bool Create();
	bool GetAllData(char*& buffer, int& len);
	bool GetAllData(std::string& str);
	int GetFileSize();

	bool Rename(const std::string& newFileName);
	bool MoveTo(const std::string& newPath);
	bool CopyTo(const std::string& newPath);
	bool Delete();
private:
	void HandleLastError(const std::string& path);
private:
	std::string _path;
	std::wstring _upath;
};

