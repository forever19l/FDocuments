#pragma once

/***************************************************************
* Purpose:   日志封装宏
* Author:    wuzx ()
* Created:   2016-04-27
**************************************************************/

#include "Util.h"

#include <log4cplus/logger.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/property.h>
#include <log4cplus/loggingmacros.h>

using namespace log4cplus;



#define UNINT_LOG() {log4cplus::Logger::shutdown();}

#define SET_LOGLEVEL(levlel) Logger::getInstance(LOG4CPLUS_TEXT("Root")).setLogLevel(levlel)
#define LOG_DEBUG(msg) LOG4CPLUS_DEBUG(Logger::getInstance(LOG4CPLUS_TEXT("Root")), msg)
#define LOG_INFO(msg) LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT("Root")), msg)
#define LOG_WARN(msg) LOG4CPLUS_WARN(Logger::getInstance(LOG4CPLUS_TEXT("Root")), msg)
#define LOG_ERROR(msg) LOG4CPLUS_ERROR(Logger::getInstance(LOG4CPLUS_TEXT("Root")), msg)
#define LOG_FATAL(msg) LOG4CPLUS_FATAL(Logger::getInstance(LOG4CPLUS_TEXT("Root")), msg)

#ifdef WIN32
#define INIT_LOG(path) { \
	log4cplus::initialize(); \
	std::wstring wPath; \
	Util::StringToWString(path, wPath); \
	SharedFileAppenderPtr append_1(new RollingFileAppender(wPath, 10 * 1024 * 1024, 10, true, true)); \
	append_1->setName(LOG4CPLUS_TEXT("Root")); \
	log4cplus::tstring pattern = LOG4CPLUS_TEXT("%D{%Y-%m-%d %H:%M:%S,%q} [%t] [%-5p] %m%n"); \
	append_1->setLayout(std::auto_ptr<Layout>(new PatternLayout(pattern))); \
	Logger::getInstance(LOG4CPLUS_TEXT("Root")).addAppender(SharedAppenderPtr(append_1.get())); \
	Logger::getInstance(LOG4CPLUS_TEXT("Root")).setLogLevel(DEBUG_LOG_LEVEL); \
}

#define INIT_LOG_NAME(name, path) { \
	log4cplus::initialize(); \
	std::wstring wPath; \
	Util::StringToWString(path, wPath); \
	SharedFileAppenderPtr append_1(new RollingFileAppender(wPath, 10 * 1024 * 1024, 10, true, true)); \
	std::wstring wName; \
	Util::StringToWString(name, wName); \
	append_1->setName(wName.c_str()); \
	log4cplus::tstring pattern = LOG4CPLUS_TEXT("%D{%Y-%m-%d %H:%M:%S,%q} [%t] [%-5p] %m%n"); \
	append_1->setLayout(std::auto_ptr<Layout>(new PatternLayout(pattern))); \
	Logger loger = Logger::getInstance(wName.c_str()); \
	loger.addAppender(SharedAppenderPtr(append_1.get())); \
	loger.setLogLevel(DEBUG_LOG_LEVEL); \
}


#define SET_LOGLEVEL_NAME(name,levlel) { \
	std::wstring wName; \
	Util::StringToWString(name, wName); \
    Logger::getInstance(wName.c_str()).setLogLevel(levlel); \
}

#define LOG_DEBUG_NAME(name,msg) { \
	std::wstring wName; \
	Util::StringToWString(name, wName); \
    LOG4CPLUS_DEBUG(Logger::getInstance(wName.c_str()), msg); \
}

#define LOG_INFO_NAME(name,msg) { \
	std::wstring wName; \
	Util::StringToWString(name, wName); \
	LOG4CPLUS_INFO(Logger::getInstance(wName.c_str()), msg); \
}

#define LOG_WARN_NAME(name,msg) { \
	std::wstring wName; \
	Util::StringToWString(name, wName); \
	LOG4CPLUS_WARN(Logger::getInstance(wName.c_str()), msg); \
}

#define LOG_ERROR_NAME(name,msg) { \
	std::wstring wName; \
	Util::StringToWString(name, wName); \
	LOG4CPLUS_ERROR(Logger::getInstance(wName.c_str()), msg); \
}

#define LOG_FATAL_NAME(name,msg) { \
	std::wstring wName; \
	Util::StringToWString(name, wName); \
	LOG4CPLUS_FATAL(Logger::getInstance(wName.c_str()), msg); \
}

#else
#define INIT_LOG(path) { \
    log4cplus::initialize(); \
    SharedFileAppenderPtr append_1(new RollingFileAppender(path, 10 * 1024 * 1024, 10, true, true)); \
    append_1->setName(LOG4CPLUS_TEXT("Root")); \
    log4cplus::tstring pattern = LOG4CPLUS_TEXT("%D{%Y-%m-%d %H:%M:%S,%q} [%t] [%-5p] %m%n"); \
    append_1->setLayout(std::auto_ptr<Layout>(new PatternLayout(pattern))); \
    Logger::getInstance(LOG4CPLUS_TEXT("Root")).addAppender(SharedAppenderPtr(append_1.get())); \
    Logger::getInstance(LOG4CPLUS_TEXT("Root")).setLogLevel(DEBUG_LOG_LEVEL); \
}

#define INIT_LOG_NAME(name, path) { \
    log4cplus::initialize(); \
    SharedFileAppenderPtr append_1(new RollingFileAppender(path, 10 * 1024 * 1024, 10, true, true)); \
    append_1->setName(name); \
    log4cplus::tstring pattern = LOG4CPLUS_TEXT("%D{%Y-%m-%d %H:%M:%S,%q} [%t] [%-5p] %m%n"); \
    append_1->setLayout(std::auto_ptr<Layout>(new PatternLayout(pattern))); \
    Logger loger = Logger::getInstance(name);\
    loger.addAppender(SharedAppenderPtr(append_1.get())); \
    loger.setLogLevel(DEBUG_LOG_LEVEL); \
}

#define SET_LOGLEVEL_NAME(name,msg) { \
    Logger::getInstance(name).setLogLevel(levlel); \
}

#define LOG_DEBUG_NAME(name,msg) { \
    LOG4CPLUS_DEBUG(Logger::getInstance(name), msg); \
}

#define LOG_INFO_NAME(name,msg) { \
    LOG4CPLUS_INFO(Logger::getInstance(name), msg); \
}

#define LOG_WARN_NAME(name,msg) { \
    LOG4CPLUS_WARN(Logger::getInstance(name), msg); \
}

#define LOG_ERROR_NAME(name,msg) { \
    LOG4CPLUS_ERROR(Logger::getInstance(name), msg); \
}

#define LOG_FATAL_NAME(name,msg) { \
    LOG4CPLUS_FATAL(Logger::getInstance(name), msg); \

#endif
//use sample 
//LOG4CPLUS_DEBUG("abc" << 1 << "bdd" << 1.25);

/*
std::string name = "myname";
LOG_DEBUG_NAME(name, "abc" << 1 << "bdd" << 1.25);
*/
