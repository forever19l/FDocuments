#pragma once

/***************************************************************
* Purpose:   日志封装宏
* Author:    wuzx ()
* Created:   2016-04-27
**************************************************************/

#include "LogHelper.h"

void UNINT_LOG()
{
	log4cplus::Logger::shutdown(); 
}

void INIT_LOG(const std::string& path, int type)
{
	log4cplus::initialize(); 
	SharedFileAppenderPtr append_1;
	if (type == 0){
#ifdef WIN32
		std::wstring wPath = Util::StringToWString(path);
		append_1 = new DailyRollingFileAppenderEx(wPath, DAILY, true, 10, true, true);
#else
		append_1 = new DailyRollingFileAppender(path, DAILY, true, 10, true, true);
#endif
	}
	else{
#ifdef WIN32
		std::wstring wPath = Util::StringToWString(path);
		append_1 = new RollingFileAppender(wPath, 10 * 1024 * 1024, 10, true, true);
#else
		append_1 = new RollingFileAppender(path, 10 * 1024 * 1024, 10, true, true));
#endif
	}
	append_1->setName(LOG4CPLUS_TEXT("Root")); 
	log4cplus::tstring pattern = LOG4CPLUS_TEXT("%D{%Y-%m-%d %H:%M:%S,%q} [%t] [%-5p] %m%n"); 
	append_1->setLayout(std::auto_ptr<Layout>(new PatternLayout(pattern))); 
	Logger::getInstance(LOG4CPLUS_TEXT("Root")).addAppender(SharedAppenderPtr(append_1.get())); 
	Logger::getInstance(LOG4CPLUS_TEXT("Root")).setLogLevel(DEBUG_LOG_LEVEL); 
}

void INIT_LOG_NAME(const std::string& name, const std::string& path, int type)
{
	log4cplus::initialize(); 
	SharedFileAppenderPtr append_1;
	if (type == 0){
#ifdef WIN32
		std::wstring wPath = Util::StringToWString(path);
		append_1 = new DailyRollingFileAppenderEx(wPath, DAILY, true, 10, true, true);

		std::wstring wName = Util::StringToWString(name); 
		append_1->setName(wName.c_str());
		log4cplus::tstring pattern = LOG4CPLUS_TEXT("%D{%Y-%m-%d %H:%M:%S,%q} [%t] [%-5p] %m%n");
		append_1->setLayout(std::auto_ptr<Layout>(new PatternLayout(pattern)));
		Logger loger = Logger::getInstance(wName.c_str());
		loger.addAppender(SharedAppenderPtr(append_1.get()));
		loger.setLogLevel(DEBUG_LOG_LEVEL);
#else
		append_1 = new DailyRollingFileAppender(path, DAILY, true, 10, true, true);

		append_1->setName(name.c_str());
		log4cplus::tstring pattern = LOG4CPLUS_TEXT("%D{%Y-%m-%d %H:%M:%S,%q} [%t] [%-5p] %m%n");
		append_1->setLayout(std::auto_ptr<Layout>(new PatternLayout(pattern)));
		Logger loger = Logger::getInstance(name.c_str());
		loger.addAppender(SharedAppenderPtr(append_1.get()));
		loger.setLogLevel(DEBUG_LOG_LEVEL);
#endif
	}
	else{
#ifdef WIN32
		std::wstring wPath = Util::StringToWString(path);
		append_1 = new RollingFileAppender(wPath, 10 * 1024 * 1024, 10, true, true);

		std::wstring wName = Util::StringToWString(name);
		append_1->setName(wName.c_str());
		log4cplus::tstring pattern = LOG4CPLUS_TEXT("%D{%Y-%m-%d %H:%M:%S,%q} [%t] [%-5p] %m%n");
		append_1->setLayout(std::auto_ptr<Layout>(new PatternLayout(pattern)));
		Logger loger = Logger::getInstance(wName.c_str());
		loger.addAppender(SharedAppenderPtr(append_1.get()));
		loger.setLogLevel(DEBUG_LOG_LEVEL);
#else
		append_1 = new RollingFileAppender(path, 10 * 1024 * 1024, 10, true, true));

		append_1->setName(name.c_str());
		log4cplus::tstring pattern = LOG4CPLUS_TEXT("%D{%Y-%m-%d %H:%M:%S,%q} [%t] [%-5p] %m%n");
		append_1->setLayout(std::auto_ptr<Layout>(new PatternLayout(pattern)));
		Logger loger = Logger::getInstance(name.c_str());
		loger.addAppender(SharedAppenderPtr(append_1.get()));
		loger.setLogLevel(DEBUG_LOG_LEVEL);
#endif
	}
}

void SET_LOGLEVEL(int level)
{
	Logger::getInstance(LOG4CPLUS_TEXT("Root")).setLogLevel(level);
}

void SET_LOGLEVEL_NAME(const std::string& name, int level)
{
#ifdef WIN32
	std::wstring wName = Util::StringToWString(name); 
	Logger::getInstance(wName.c_str()).setLogLevel(level);
#else
	Logger::getInstance(name.c_str()).setLogLevel(level);
#endif
}

std::string VForm(const char* format, va_list args)
{
	size_t size = 2048;
	char* buffer = new char[size];

	while (1)
	{
		va_list args_copy;
		va_copy(args_copy, args);
#ifdef WIN32
		int n = _vsnprintf(buffer, size, format, args_copy);
#else
		int n = vsnprintf(buffer, size, format, args_copy);
#endif
		va_end(args_copy);

		if ((n > -1) && (static_cast<size_t>(n) < size)) {
			std::string s(buffer);
			delete[] buffer;
			return s;
		}

		// twice the old size
		size = size * 2;

		delete[] buffer;
		buffer = new char[size];
	}
}

void LOG_DEBUG(const char* msg, ...)
{
	va_list va;
	va_start(va, msg);
	std::string str = VForm(msg, va);
	va_end(va);
	LOG4CPLUS_DEBUG(Logger::getInstance(LOG4CPLUS_TEXT("Root")), str.c_str());
}

void LOG_INFO(const char* msg, ...)
{
	va_list va;
	va_start(va, msg);
	std::string str = VForm(msg, va);
	va_end(va);
	LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("Root")), str.c_str());
}

void LOG_WARN(const char* msg, ...)
{
	va_list va;
	va_start(va, msg);
	std::string str = VForm(msg, va);
	va_end(va);
	LOG4CPLUS_WARN(Logger::getInstance(LOG4CPLUS_TEXT("Root")), str.c_str());
}

void LOG_ERROR(const char* msg, ...)
{
	va_list va;
	va_start(va, msg);
	std::string str = VForm(msg, va);
	va_end(va);
	LOG4CPLUS_ERROR(Logger::getInstance(LOG4CPLUS_TEXT("Root")), str.c_str());
}

void LOG_FATAL(const char* msg, ...)
{
	va_list va;
	va_start(va, msg);
	std::string str = VForm(msg, va);
	va_end(va);
	LOG4CPLUS_FATAL(Logger::getInstance(LOG4CPLUS_TEXT("Root")), str.c_str());
}

//////////////////////////////////////////////////////////////////////////////////////////////
void LOG_DEBUG_NAME(const char* name, const char* msg, ...)
{
	va_list va;
	va_start(va, msg);
	std::string str = VForm(msg, va);
	va_end(va);
#ifdef WIN32
	std::wstring wName = Util::StringToWString(name);
	LOG4CPLUS_DEBUG(Logger::getInstance(wName.c_str()), str.c_str());
#else
	LOG4CPLUS_DEBUG(Logger::getInstance(name), str.c_str());
#endif
}

void LOG_INFO_NAME(const char* name, const char* msg, ...)
{
	va_list va;
	va_start(va, msg);
	std::string str = VForm(msg, va);
	va_end(va);
#ifdef WIN32
	std::wstring wName = Util::StringToWString(name);
	LOG4CPLUS_INFO(Logger::getInstance(wName.c_str()), str.c_str());
#else
	LOG4CPLUS_INFO(Logger::getInstance(name), str.c_str());
#endif
}

void LOG_WARN_NAME(const char* name, const char* msg, ...)
{
	va_list va;
	va_start(va, msg);
	std::string str = VForm(msg, va);
	va_end(va);
#ifdef WIN32
	std::wstring wName = Util::StringToWString(name);
	LOG4CPLUS_WARN(Logger::getInstance(wName.c_str()), str.c_str());
#else
	LOG4CPLUS_WARN(Logger::getInstance(name), str.c_str());
#endif
}

void LOG_ERROR_NAME(const char* name, const char* msg, ...)
{
	va_list va;
	va_start(va, msg);
	std::string str = VForm(msg, va);
	va_end(va);
#ifdef WIN32
	std::wstring wName = Util::StringToWString(name);
	LOG4CPLUS_ERROR(Logger::getInstance(wName.c_str()), str.c_str());
#else
	LOG4CPLUS_ERROR(Logger::getInstance(name), str.c_str());
#endif
}

void LOG_FATAL_NAME(const char* name, const char* msg, ...)
{
	va_list va;
	va_start(va, msg);
	std::string str = VForm(msg, va);
	va_end(va);
#ifdef WIN32
	std::wstring wName = Util::StringToWString(name);
	LOG4CPLUS_FATAL(Logger::getInstance(wName.c_str()), str.c_str());
#else
	LOG4CPLUS_FATAL(Logger::getInstance(name), str.c_str());
#endif
}