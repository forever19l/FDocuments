#ifndef UTIL_H
#define UTIL_H

/***************************************************************
 * Purpose:   功能函数定义类，支持跨平台。
 * Author:    wuzx ()
 * Created:   2016-04-27
 **************************************************************/

#include <string>
#include <vector>


typedef void (*ConsoleQuitHandlerFun)();

class Util
{
public:
    static std::string GetWorkDirectory(); //获取当前运行程序目录

	static std::string GetProcessName(); //获取当前进程名称

	static int GetPid(); //获取pid

	static std::string CreateGuid(); //生成guid

	static void Split(const std::string& src, const std::string& split, std::vector<std::string>& results);

	static std::wstring StringToWString(const std::string &str);

	static std::string WStringToString(const std::wstring &wstr);

	//mode:替换模式，0：只替换第一个找到的。1：全部替换找到的。
	static int Replace(std::string& str, const std::string& pattern, const std::string& newpat, int mode);

	static int MakeNDir(const std::string& fullPath);//递归创建目录

	static bool IsFileExist(const std::string& fullPath); //文件是否存在
	static std::string GetFileName(const std::string& fullPath); //从路径中截取到文件名

	static std::string GetTime(); //获取当前时间戳

	static int GetPidByPort(int port); //根据端口号找到pid
	static bool KillProcess(int pid); //杀死进程根据pid
	static int GetFileLength(FILE* fp); //获取文件长度
	static int ExecCommand(const char* szCommand, char *&szResult, size_t &sSize);
	static std::vector<unsigned long> KillProcessByName(const std::string& procName); //杀掉名称为 name 的进程.
	static std::vector<unsigned long> GetProcessByName(const std::string& procName);

	static std::string Format(const char* format, ...); //格式化

	static void SleepForMilliSeconds(int milliseconds); //sleep

	static void SetupConsoleQuitHandler(ConsoleQuitHandlerFun handler);//控制台退出信号处理

	static char* basename(const char *path);
private:
    Util();
};

//删除list vector set等单元素容器类
template <class T>
void DELETE_LIST(T& list)
{
	for (int i = 0; i < list.size(); ++i)
	{
		delete list[i];
	}

	list.clear();
}

//删除map，multimap等key、vale容器类
template <class T>
void DELETE_MAP(T& map, bool delSecond = true, bool delFirst = false)
{
	auto iter = map.begin();
	for (; iter != map.end(); ++iter)
	{
		if (delSecond)
		{
			delete iter->second;
		}

		if (delFirst)
		{
			delete iter->first;
		}
	}

	map.clear();
}


#define SAFE_NEW(p, type) \
{ \
   p = new type; \
}

#define SAFE_DELETE(p) \
{ \
    if (NULL != p) \
	{ \
        delete p; \
        p = NULL; \
	} \
}

#define SAFE_DELETE_A(p) \
{ \
	if (NULL != p) \
	{ \
       delete[] p; \
       p = NULL; \
	} \
}

#endif // UTIL_H
