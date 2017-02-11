#include "Util.h"
#include "XmlHelper.h"
#include "LogHelper.h"

#include <string>
#include <algorithm>
#include <stdarg.h>

#ifdef _WIN32
#include <windows.h>
#include <tlhelp32.h>
#include <objbase.h>
#include <io.h>
#else
#include <pthread.h>
#include <uuid/uuid.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#endif

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef WIN32
#define sprintf sprintf_s
#define fopen fopen_s
#endif

const char FlowChart_Str[] = "FlowChart";
const char Items_Str[] = "Items";
const char Item_Str[] = "Item";
const char type_Str[] = "type";
const char name_Str[] = "name";
const char xpos_Str[] = "xpos";
const char ypos_Str[] = "ypos";
const char width_Str[] = "width";
const char height_Str[] = "height";
const char lineType_Str[] = "lineType";
const char Script_Str[] = "Script";

#ifdef WIN32
GUID CreateGuid_()
{
	GUID guid;
	CoCreateGuid(&guid);
	return guid;
}

std::string GuidToString_(const GUID &guid)
{
	char buf[64] = { 0 };
	_snprintf_s(
		buf,
		sizeof(buf),
		"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1],
		guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5],
		guid.Data4[6], guid.Data4[7]);
	return std::string(buf);
}
#endif

std::string Util::CreateGuid()
{
#ifdef WIN32
	GUID guid = CreateGuid_();
	return GuidToString_(guid);
#else
    uuid_t uu;
    uuid_generate(uu);
    char str[36];
    uuid_unparse(uu,str);
    std::string uuid = str;

    return uuid;
#endif
}

std::string Util::GetWorkDirectory()
{
	std::string path = "";
#ifdef _WIN32
	char file_path[1024];
	::GetModuleFileNameA(NULL, file_path, sizeof(file_path)); // 得到当前执行文件的文件名(包含路径)
	path = file_path;
	int pos = path.rfind("\\");
	if (pos > 0)
	{
		path = path.substr(0, pos);
	}
#else
	char file_path[260];
	getcwd(file_path, sizeof(file_path));
    path = file_path;
#endif
	return path;
}


std::string Util::GetProcessName()
{
	std::string proName = "";
#ifdef _WIN32
	char file_path[1024];
	::GetModuleFileNameA(NULL, file_path, sizeof(file_path)); // 得到当前执行文件的文件名(包含路径)
	proName = file_path;
	int pos = proName.rfind("\\");
	if (pos > 0)
	{
		proName = proName.substr(pos + 1, proName.size() - pos - 1);
		pos = proName.find(".exe");
		if (pos > 0)
		{
			proName = proName.substr(0, pos);
		}
	}
#else
	
#endif
	return proName;
}

int Util::GetPid()
{
	int pid = -9999;
#ifdef _WIN32
	pid = GetCurrentProcessId();
#else
	pid = getpid();
#endif 

	return pid;
}

void Util::Split(const std::string& src, const std::string& split, std::vector<std::string>& results)
{
	std::string tSrc = src;
	while (tSrc.length() > 0)
	{
		int pos = tSrc.find_first_of(split.c_str());
		if (pos != std::string::npos)
		{
			std::string str = tSrc.substr(0, pos);
			results.push_back(str);

			tSrc = tSrc.substr(pos + 1, tSrc.length() - pos - 1);
		}
		else
		{
			if (!tSrc.empty())
				results.push_back(tSrc);

			break;
		}
	}
}

bool Util::StringToWString(const std::string &str, std::wstring &wstr)
{
#ifdef _WIN32
	int nLen = (int)str.length();
	wstr.resize(nLen, L' ');

	int nResult = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)str.c_str(), nLen, (LPWSTR)wstr.c_str(), nLen);

	if (nResult == 0)
	{
		return false;
	}
#endif
	return true;
}

bool Util::WStringToString(const std::wstring &wstr, std::string &str)
{
#ifdef _WIN32
	int nLen = (int)wstr.length();
	str.resize(nLen, ' ');

	int nResult = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wstr.c_str(), nLen, (LPSTR)str.c_str(), nLen, NULL, NULL);

	if (nResult == 0)
	{
		return false;
	}
#endif
	return true;
}

//mode:替换模式，0：只替换第一个找到的。1：全部替换找到的。
int Util::Replace(std::string& str, const std::string& pattern, const std::string& newpat, int mode)
{
    int count = 0;
    const size_t nsize = newpat.size();
    const size_t psize = pattern.size();

    for (size_t pos = str.find(pattern, 0);
		pos != std::string::npos;
        pos = str.find(pattern, pos + nsize))
	{
        str.replace(pos, psize, newpat);
		count++;

		if (mode == 0)
			break;
	}

	return count;
}

int Util::MakeNDir(const std::string& fullPath)
{
#ifdef _WIN32
	std::string path = fullPath;
	Replace(path, "\\", "/", 1);

	std::string strCompletePath = "";
	std::vector<std::string> vecPath;
	Split(path, "/", vecPath);

	std::vector<std::string>::iterator iter = vecPath.begin();
	for (; iter != vecPath.end(); ++iter)
	{
		strCompletePath += *iter + "/";
		if (-1 == _access(strCompletePath.c_str(), 0))
		{
			if (!CreateDirectoryA((strCompletePath.c_str()), NULL))
			{
				int err = GetLastError();
				LOG_ERROR("Could not create new directory. file:\n" << strCompletePath.c_str() << "error:" << GetLastError());
				return 1;
			}
		}
	}
#else
    std::string strCmd;
    strCmd = "mkdir -p ";
    strCmd += fullPath;
    if (-1 == system(strCmd.c_str()))
    {
        return 1;
    }
#endif
	return 0;
}

bool Util::IsFileExist(const std::string& fullPath)
{
#ifdef _WIN32
	if (-1 == _access(fullPath.c_str(), 0))
		return false;
#else
    struct stat statDir;
    if(0 != stat(fullPath.c_str(), &statDir))
    {
        return false;
    }

    if(!S_ISREG(statDir.st_mode))
    {
        return false;
    }
#endif

	return true;
}

std::string Util::GetFileName(const std::string& fullPath)
{
	std::string path = fullPath;
	Replace(path, "\\", "/", 1);

	std::string na;
	int pos = path.find_last_of("/");
	if (pos != std::string::npos)
	{
		na = path.substr(pos + 1, path.size() - pos - 1);
	}

	return na;
}

std::string Util::GetTime()
{
	std::string strNowTime;
#ifdef _WIN32
	char szTemp[24];
	SYSTEMTIME nowTime;
	GetLocalTime(&nowTime);
	sprintf(szTemp, "%d-%02d-%02d %02d:%02d:%02d.%03d", nowTime.wYear, nowTime.wMonth, nowTime.wDay,
		nowTime.wHour, nowTime.wMinute, nowTime.wSecond, nowTime.wMilliseconds);
	strNowTime = szTemp;
#else
	struct timeval tmVal;
	tm *pTm;
	time_t tmSecond;
	time_t tmMicroSecond;

	if (0 == gettimeofday(&tmVal, NULL))
	{
		tmSecond = tmVal.tv_sec;
		tmMicroSecond = tmVal.tv_usec;
	}
	else
	{
		tmSecond = time(NULL);
		tmMicroSecond = 0;
	}

	pTm = localtime(&tmSecond);
	char szTemp[24];
	sprintf(szTemp, "%d-%02d-%02d %02d:%02d:%02d.%03d",
		pTm->tm_year + 1900, pTm->tm_mon + 1, pTm->tm_mday,
		pTm->tm_hour, pTm->tm_min, pTm->tm_sec, (int)tmMicroSecond / 1000);

	strNowTime = szTemp;
#endif
	return strNowTime;
}

int Util::GetPidByPort(int port)
{
#ifdef WIN32
	int res = -9999;
	std::string resultFile = GetWorkDirectory() + "/log/processport1.txt";
	char szBuf[1024];
	sprintf(szBuf, "/C netstat -ano | findstr LISTENING | findstr %d > %s", port, resultFile.c_str());

	SHELLEXECUTEINFOA ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = "cmd.exe";
	ShExecInfo.lpParameters = szBuf;
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;
	if (!ShellExecuteExA(&ShExecInfo))
	{
		LOG_ERROR("Shell excute to get pid by port failed,  may be no privilage to excute the command");
		return -1;
	}
		
	WaitForSingleObject(ShExecInfo.hProcess, 3000);

	FILE* fp = NULL;
	errno_t err = fopen(&fp, resultFile.c_str(), "r");
	if (err != 0)
	{
		LOG_ERROR("Can not Get pid by port, may be no privilage to get it.");
		return -1;
	}

	int length = GetFileLength(fp);
	if (length <= 0)
	{
		fclose(fp);
		remove(resultFile.c_str());
		return -1;
	}

	char* szContent = new char[length]{0};
	int size = fread(szContent, length, 1, fp);

	char szTmpData[32];
	char* szTmp = szContent + length - 1;
	int index = 0;
	while (szTmp != NULL && *szTmp != ' ' && index < length - 1)
	{
		if (*szTmp >= '0' && *szTmp <= '9')
		{
			szTmpData[index++] = *szTmp;
		}

		--szTmp;
	}

	char szNumber[32] = { 0 };
	int k = 0;
	for (int i = index - 1; i >= 0; --i)
	{
		szNumber[k++] = szTmpData[i];
	}

	res = atoi(szNumber);
	delete[] szContent;

	fclose(fp);
	remove(resultFile.c_str());
	return res;

#else
	char* szResult = NULL;
	size_t length = 0;
	char szCommand[1024];
	sprintf(szCommand, "lsof -i:%d| grep 'LISTEN' | awk '{print $2}'", port);
	int ret = ExecCommand(szCommand, szResult, length);
	if (ret == -1)
		return -1;

	if (length > 0)
	{
		int res = atoi(szResult);
		delete[] szResult;
		return res;
	}

	return -1;
#endif
}

bool Util::KillProcess(int pid)
{
#ifdef WIN32
	HANDLE hd = ::OpenProcess(PROCESS_TERMINATE, false, pid);
	if (hd == NULL)
		return false;
	bool flag = ::TerminateProcess(hd, 0);
	return flag;

#else
	char* szResult = NULL;
	size_t length = 0;
	char szCommand[1024];
	sprintf(szCommand, "kill %d", pid);
	int ret = ExecCommand(szCommand, szResult, length);
	if (ret == -1)
		return false;

	if (length > 0)
	{
		delete[] szResult;
		return false;
	}

	return true;
#endif
}

int Util::GetFileLength(FILE* fp)
{
	int ret = fseek(fp, 0L, SEEK_END);
	if (ret != 0)
	{
		return -1;
	}

	int length = ftell(fp);
	if (length == 0)
	{
		return 0;
	}

	ret = fseek(fp, 0L, SEEK_SET);
	if (ret != 0)
	{
		return -1;
	}

	return length;
}

int Util::ExecCommand(const char* szCommand, char *&szResult, size_t &sSize)
{
#ifdef WIN32
	return -1;
#else
	if (NULL == szCommand)
	{
		return -1;
	}

	FILE * fp = NULL;
	int ch = 0, iIndex = 0;

	if (NULL == (fp = popen(szCommand, "r")))
	{
		pclose(fp);
		return -1;
	}

	char szBuf[8096] = { 0 };
	while ((ch = fgetc(fp)) != EOF)
	{
		if (ch != '\r' && ch != '\n')
		{
			szBuf[iIndex] = (char)ch;
			iIndex++;
		}
	}

	if (iIndex > 0)
	{
		sSize = iIndex;
		szResult = new char[sSize + 1];
		strcpy(szResult, szBuf);
		szResult[iIndex] = '\0';
	}

	pclose(fp);
	return 0;
#endif
}

std::string Util::Format(const char* format, ...)
{
	char szBuf[8192];
	va_list arglist;
	va_start(arglist, format);
	vsprintf_s(szBuf, format, arglist);
	va_end(arglist);

	return szBuf;
}

std::vector<unsigned long> Util::KillProcessByName(const std::string& procName)
{
	std::vector<unsigned long> pidList;
#ifdef WIN32
	HANDLE handle = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (handle != NULL)
	{
		PROCESSENTRY32 pe32 = { 0 };
		pe32.dwSize = sizeof(pe32);
		BOOL more = ::Process32First(handle, &pe32);
		while (more)
		{
			std::string str;
			bool flag = WStringToString(pe32.szExeFile, str);
			if (_stricmp(procName.c_str(), str.c_str()) == 0 || 
				_stricmp((procName + ".exe").c_str(), str.c_str()) == 0) 
			{
				pidList.push_back(pe32.th32ProcessID);
			}
			more = ::Process32Next(handle, &pe32);
		}
		for (std::vector<unsigned long>::iterator it = pidList.begin(); it != pidList.end(); it++)
		{
			::TerminateProcess(::OpenProcess(PROCESS_TERMINATE, false, *it), 0);
		}

		::CloseHandle(handle);
	}
#else
#endif
	return pidList;
}

std::vector<unsigned long> Util::GetProcessByName(const std::string& procName)
{
	std::vector<unsigned long> pidList;
#ifdef WIN32
	HANDLE handle = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (handle != NULL)
	{
		PROCESSENTRY32 pe32 = { 0 };
		pe32.dwSize = sizeof(pe32);
		BOOL more = ::Process32First(handle, &pe32);
		while (more)
		{
			std::string str;
			bool flag = WStringToString(pe32.szExeFile, str);
			if (_stricmp(procName.c_str(), str.c_str()) == 0 ||
				_stricmp((procName + ".exe").c_str(), str.c_str()) == 0)
			{
				pidList.push_back(pe32.th32ProcessID);
			}
			more = ::Process32Next(handle, &pe32);
		}

		::CloseHandle(handle);
	}
#else
#endif
	return pidList;
}