#pragma once
#include <string>
#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif
typedef void (*RecvDataCallBack)(const std::string& data, void* context);
typedef void (*CloseClientCallBack)(void* context);
typedef std::string (*ReplyCallBack)(void * buf, long len, void * context);
typedef void * (*SubCallBack)(void * buf, long len, void * context);
typedef void (*FDCallBack)(int type, unsigned int fd, const std::string& ip, int port, void* context);
typedef void(*RecvClientDataCallBack)(unsigned int fd, const std::string& buf, void* context);


typedef RecvDataCallBack OnRecvDataCallBack;
typedef CloseClientCallBack OnCloseClientCallBack;

