/********************************************

*/
#ifndef __CLOG__H__
#define __CLOG__H__

#pragma once
//#include "lock.h"

#define NO_WIN32_LEAN_AND_MEAN   
#include <ShlObj.h>

//MAX file 4M
static const int g_nMaxSize = 1024 * 1024 * 4;




class CLog
{
public:
	static CLog*	Instance()
	{
		static CLog log;
		return &log;
	}
	bool	WriteLog(const char* lpLog);
	bool	WriteLog(const wchar_t* lpLog);
	bool	WriteLog(const char* lpFile, long lLine, const char* lpLog);
	bool	WriteLog(const char* lpFile, long lLine, const wchar_t* lpLog);
	bool	WriteJoinLog(const wchar_t* lpText, const wchar_t* lpLog);
	bool	WriteJoinLog(const char* lpFile, long lLine, const wchar_t* lpText, const wchar_t* lpLog);

protected:
	CLog();
	~CLog();
	bool	InitLogFile();
	char*	WcharToChar(const wchar_t* lpSource);

private:
	wchar_t	m_szLog[MAX_PATH];
	int		m_nWriteSize;
	//CLock	m_lock;
};



#endif