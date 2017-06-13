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

void UNINT_LOG();

void INIT_LOG(const std::string& path, int type = 0);
void INIT_LOG_NAME(const std::string& name, const std::string& path, int type = 0);

void SET_LOGLEVEL(int level);
void SET_LOGLEVEL_NAME(const std::string& name, int level);

void LOG_DEBUG(const char* msg, ...);

void LOG_INFO(const char* msg, ...);

void LOG_WARN(const char* msg, ...);

void LOG_ERROR(const char* msg, ...);

void LOG_FATAL(const char* msg, ...);


void LOG_DEBUG_NAME(const char* name, const char* msg, ...);

void LOG_INFO_NAME(const char* name, const char* msg, ...);

void LOG_WARN_NAME(const char* name, const char* msg, ...);

void LOG_ERROR_NAME(const char* name, const char* msg, ...);

void LOG_FATAL_NAME(const char* name, const char* msg, ...);
