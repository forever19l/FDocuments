#include "Util.h"
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
#include <signal.h>
#include <unistd.h>
#include <dirent.h>
#endif

#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef WIN32
#define sprintf sprintf_s
#define fopen fopen_s
#endif

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
    #if defined(_WIN32)
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
    #elif defined(__APPLE__)
        char buf[0];
        uint32_t size = 0;
        int res = _NSGetExecutablePath(buf,&size);

        char* path = (char*)malloc(size+1);
        path[size] = 0;
        res = _NSGetExecutablePath(path,&size);

        char *strProcessName = basename(path);
        proName.assign(strProcessName);
    #else
        char strProcessPath[1024] = {0};


        if (readlink("/proc/self/exe", strProcessPath, 1024) <= 0)
        {
            return "";
        }

        char *strProcessName = strrchr(strProcessPath, '/');

        if (!strProcessName)
        {
            return "";
        }
        else
        {
            proName.assign(++strProcessName);
        }
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
		int pos = tSrc.find(split.c_str());
		if (pos != std::string::npos)
		{
			std::string str = tSrc.substr(0, pos);
			results.push_back(str);

			tSrc = tSrc.substr(pos + split.size(), tSrc.length() - pos - split.size());
		}
		else
		{
			if (!tSrc.empty())
				results.push_back(tSrc);

			break;
		}
	}
}

std::wstring Util::StringToWString(const std::string &str)
{
	size_t convertedChars = 0;
	std::string curLocale = setlocale(LC_ALL, NULL);   //curLocale="C"
	setlocale(LC_ALL, "chs");
	const char* source = str.c_str();
	size_t charNum = sizeof(char)*str.size() + 1;
	wchar_t* dest = new wchar_t[charNum];
#ifdef WIN32
    mbstowcs_s(&convertedChars, dest, charNum, source, _TRUNCATE);
#else
    mbstowcs(dest, source, str.size());
#endif
	std::wstring result = dest;
	delete[] dest;
	setlocale(LC_ALL, curLocale.c_str());
	return result;
}

std::string Util::WStringToString(const std::wstring &wstr)
{
	std::string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";  

	setlocale(LC_ALL, "chs");

    const wchar_t* source = wstr.c_str();
    size_t size = 2 * wstr.size() + 1;
    char* dest = new char[size];
    memset(dest, 0, size);
#ifdef WIN32
	size_t convertedChars = 0;
    wcstombs_s(&convertedChars, dest, size, source, _TRUNCATE);
#else
    wcstombs(dest,source, size);
#endif
    std::string result = dest;
    delete[] dest;

	setlocale(LC_ALL, curLocale.c_str());

	return result;
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
	std::string path = fullPath;
	Replace(path, "\\", "/", 1);
	if (fullPath.rfind('.') != std::string::npos)
	{
		int pos = fullPath.rfind("/");
		if (pos != std::string::npos)
		{
			path = fullPath.substr(0, pos);
		}
	}
#ifdef _WIN32
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
				//LOG_ERROR("Could not create new directory. file:\n" << strCompletePath.c_str() << "error:" << GetLastError());
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
		//LOG_ERROR("Shell excute to get pid by port failed,  may be no privilage to excute the command");
		return -1;
	}
		
	WaitForSingleObject(ShExecInfo.hProcess, 3000);

	FILE* fp = NULL;
	errno_t err = fopen(&fp, resultFile.c_str(), "r");
	if (err != 0)
	{
		//LOG_ERROR("Can not Get pid by port, may be no privilage to get it.");
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
#ifdef _WIN32
	vsprintf_s(szBuf, format, arglist);
#else
    vsprintf(szBuf, format, arglist);
#endif
	va_end(arglist);

	return szBuf;
}

std::vector<unsigned long> Util::KillProcessByName(const std::string& procName)
{
    std::vector<unsigned long> pidList;
  #if defined(__APPLE__)
      char szCommand[256] = {0};
      char* pname = Util::basename(procName.c_str());
      sprintf(szCommand, "kill -9 $(ps -ef|grep %s |awk '$0 !~/grep/ {print $2}' |tr -s '\n' ' ')", pname);
      system(szCommand);
  #else
      pidList = Util::GetProcessByName(procName);
      for(unsigned long pid : pidList)
      {
          Util::KillProcess(pid);
      }

  #endif
      return pidList;
}

char* Util::basename(const char *path)
{
    const char *s;
    const char *p;

	p = s = path;

	while (*s) {
		if (*s++ == '/') {
			p = s;
		}
	}
	return (char *)p;
}

std::vector<unsigned long> Util::GetProcessByName(const std::string& procName)
{
    std::vector<unsigned long> pidList;
    #if defined(WIN32)
        HANDLE handle = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (handle != NULL)
        {
            //PROCESSENTRY32 pe32 = { 0 };
            tagPROCESSENTRY32W pe32 = { 0 };
            pe32.dwSize = sizeof(pe32);
            BOOL more = ::Process32FirstW(handle, &pe32);
            while (more)
            {
                std::string str = WStringToString(pe32.szExeFile);
                if (_stricmp(procName.c_str(), str.c_str()) == 0 ||
                    _stricmp((procName + ".exe").c_str(), str.c_str()) == 0)
                {
                    pidList.push_back(pe32.th32ProcessID);
                }
                more = ::Process32NextW(handle, &pe32);
            }

            ::CloseHandle(handle);
        }
    #elif defined(__APPLE__)
        return pidList;
    #else

    #ifndef MAX_BUF_SIZE
    #define  MAX_BUF_SIZE       256
    #endif
        DIR *dir;
        struct dirent *next;
        pid_t pid;
        FILE *fp;
        char *base_pname = NULL;
        char *base_fname = NULL;
        char cmdline[MAX_BUF_SIZE];
        char path[MAX_BUF_SIZE];

        if (procName.empty())
            return pidList;

        base_pname = Util::basename(procName.c_str());
        if (strlen(base_pname) <= 0)
            return pidList;

        dir = opendir("/proc");
        if (!dir)
        {
            return pidList;
        }
        while ((next = readdir(dir)) != NULL) {
            /* skip non-number */
            if (!isdigit(*next->d_name))
                continue;

            pid = strtol(next->d_name, NULL, 0);
            sprintf(path, "/proc/%u/cmdline", pid);
            fp = fopen(path, "r");
            if (fp == NULL)
                continue;

            memset(cmdline, 0, sizeof(cmdline));
            if (fread(cmdline, MAX_BUF_SIZE - 1, 1, fp) < 0){
                fclose(fp);
                continue;
            }
            fclose(fp);
            base_fname = Util::basename(cmdline);

            if (strcmp(base_fname, base_pname) == 0)
            {
                pidList.push_back(pid);
            }
        }
        closedir(dir);
    #endif
        return pidList;
}

void Util::SleepForMilliSeconds(int milliseconds)
{
#ifdef _WIN32
	Sleep(milliseconds);
#else
	usleep(milliseconds * 1000); // takes microseconds
#endif
}


ConsoleQuitHandlerFun ConsoleHandler = NULL;
#ifdef WIN32
BOOL CtrlHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType)
	{
	case CTRL_C_EVENT:
		printf("CTRL_C_EVENT \n");
		if (ConsoleHandler != NULL){
			ConsoleHandler();
		}
		break;
	case CTRL_CLOSE_EVENT:
		printf("CTRL_CLOSE_EVENT \n");
		if (ConsoleHandler != NULL){
			ConsoleHandler();
		}
		break;
	case CTRL_SHUTDOWN_EVENT:
		printf("CTRL_SHUTDOWN_EVENT \n");
		if (ConsoleHandler != NULL){
			ConsoleHandler();
		}
		break;
	default:
		return FALSE;
	}

	return (TRUE);
}
#else
void SigProcss(int sig)
{
	if (SIGINT == sig){
		printf("signal SIGINT\n");
		if (ConsoleHandler != NULL){
			ConsoleHandler();
		}
	}
	else if (SIGQUIT == sig){
		printf("signal SIGQUIT\n");
		if (ConsoleHandler != NULL){
			ConsoleHandler();
		}
	}
}
#endif


void Util::SetupConsoleQuitHandler(ConsoleQuitHandlerFun handler)
{
	ConsoleHandler = handler;
#ifdef WIN32
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE);
#else
	signal(SIGINT, SigProcss);
	signal(SIGQUIT, SigProcss);
#endif
}
