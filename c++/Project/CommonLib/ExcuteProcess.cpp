#include "ExcuteProcess.h"
#include "Util.h"

#ifdef WIN32
#include <Windows.h>
#else
#endif

ExcuteProcess::ExcuteProcess()
{
}


ExcuteProcess::~ExcuteProcess()
{
}

void ExcuteProcess::SetWorkingPath(const std::string& path)
{
	_workingPath = path;
}

std::string ExcuteProcess::GetWorkingPath()
{
	return _workingPath;
}

void ExcuteProcess::SetOutputCallBack(OutputCallback callback, void* context)
{
	_outputCallback = callback;
	_context = context;
}

bool ExcuteProcess::Excute(const std::string& cmd)
{
#ifdef WIN32
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	std::string buf;
	if (!CreatePipe(&_readPipe, &_writePipe, &sa, 0))
	{
		printf("CreatePipe failed.\n");
		return false;
	}

	if (!SetHandleInformation(_readPipe, HANDLE_FLAG_INHERIT, 0))
	{
		CloseHandle(_writePipe);
		CloseHandle(_readPipe);
		printf("SetHandleInformation failed.\n");
		return false;
	}

	ZeroMemory(&_pi, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&_si, sizeof(STARTUPINFO));

	_si.cb = sizeof(STARTUPINFO);
	_si.hStdError = _writePipe;
	_si.hStdOutput = _writePipe;
	_si.dwFlags |= STARTF_USESTDHANDLES;

	std::wstring wcmd = Util::StringToWString(cmd);
	wchar_t* pwcmd = _wcsdup(wcmd.c_str());
	std::wstring wpath = Util::StringToWString(_workingPath);
	if (!CreateProcessW(NULL, pwcmd, NULL, NULL, TRUE, 0, NULL, wpath.c_str(), &_si, &_pi))
	{
		CloseHandle(_writePipe);
		CloseHandle(_readPipe);
		CloseHandle(_pi.hProcess);
		CloseHandle(_pi.hThread);
		free(pwcmd);
		
		printf("CreateProcess failed. workpath=%s, cmd=%s\n", _workingPath.c_str(), cmd.c_str());
	}

	printf("CreateProcess success. workpath=%s, cmd=%s\n", _workingPath.c_str(), cmd.c_str());
	free(pwcmd);

	_readThread = new std::thread(Start, this);
	return true;
	
#else


#endif
	
}

void ExcuteProcess::CloseProcess()
{
#ifdef WIN32
	TerminateThread(_readThread->native_handle(), 1);
	_isRun = false;
	if (_readThread->joinable()){
		_readThread->join();
	}
	SAFE_DELETE(_readThread);

	CloseHandle(_writePipe);
	CloseHandle(_readPipe);
	
	TerminateProcess(_pi.hProcess, 1);
	CloseHandle(_pi.hThread);
	CloseHandle(_pi.hProcess);
#else
#endif
	printf("CloseProcess success.\n");
}


void ExcuteProcess::Start(void* context)
{
	ExcuteProcess* excuProcess = static_cast<ExcuteProcess*>(context);
	excuProcess->StartToReadPipe();
}

void ExcuteProcess::StartToReadPipe()
{
#ifdef WIN32
	while (_isRun)
	{
		DWORD bytesRead;
		BOOL ret = false;
		char buffer[1024] = { 0 };
		ret = ReadFile(_readPipe, buffer, sizeof(buffer) - 1, &bytesRead, 0);
		if (ret){
			if (NULL != _outputCallback){
				_outputCallback(_name, buffer, _context);
			}
			else{
				printf("%s", buffer);
			}
		}
		else{
			DWORD dwLastError = GetLastError();
			if (ERROR_BROKEN_PIPE == dwLastError) {
				printf("Read Pipe failed.\n");
				
			}
			else{
				printf("UnHandler read pipe error.errno=%d", dwLastError);
			}
			break;
		}
	}
#else
#endif
}