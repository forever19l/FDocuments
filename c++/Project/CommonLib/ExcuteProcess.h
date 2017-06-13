#pragma once

/***************************************************************
* Purpose:   跨平台进程启动类，能获取标准输出。
* Author:    wuzx ()
* Created:   2017-01-18
**************************************************************/


#include <string>
#include <thread>

#ifdef WIN32
#include <Windows.h>
#else
#endif
typedef void(*OutputCallback)(const std::string& name, const std::string& text, void* context);

class ExcuteProcess
{
public:
	ExcuteProcess();
	~ExcuteProcess();

	void SetName(const std::string& name) { _name = name; }
	std::string GetName() { return _name; }

	void SetWorkingPath(const std::string& path);
	std::string GetWorkingPath();
	
	void SetOutputCallBack(OutputCallback callback, void* context);

	bool Excute(const std::string& cmd);
	void CloseProcess();

private:
	static void Start(void* context);
	void StartToReadPipe();
private:
	std::string _name;
	std::string _workingPath;
	std::string _cmd;
	
	OutputCallback _outputCallback = NULL;
	void* _context = NULL;

	std::thread* _readThread = NULL;
	bool _isRun = true;
#ifdef WIN32
	HANDLE _readPipe = NULL;
	HANDLE _writePipe = NULL;
	STARTUPINFO _si;
	PROCESS_INFORMATION _pi;
#else
#endif
};

