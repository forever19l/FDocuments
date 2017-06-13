#pragma once

/***************************************************************
* Purpose:   ������
* Author:    wuzx ()
* Created:   2017-03-05
**************************************************************/

#include <thread>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <mutex>
#include "NetCallBackDefine.hpp"

#ifdef WIN32
#include <windows.h>
#else
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#endif


/* #include <winbase.h>
#define NOPARITY            0
#define ODDPARITY           1
#define EVENPARITY          2
#define MARKPARITY          3
#define SPACEPARITY         4

#define ONESTOPBIT          0
#define ONE5STOPBITS        1
#define TWOSTOPBITS         2
*/

struct SerialPortInfo{
	std::string name;
    unsigned int baudRate{9600};
    unsigned int parity{'e'};
    unsigned int dataBits{7};
    unsigned int stopBits{1};
};

class  BaseCom
{
public:
	BaseCom();
	virtual ~BaseCom();
public:
    bool Connect(const SerialPortInfo& portinfo);
    void Close();
    bool IsOpen();
	std::string GetError();
#ifdef WIN32
public:
	bool Connect(const std::string& portName, unsigned int baudRate = CBR_9600, unsigned int parity = NOPARITY,
		unsigned int dataBits = 8, unsigned int stopBits = ONESTOPBIT);
	bool Connect(int port, unsigned int baudRate = CBR_9600, unsigned int parity = NOPARITY, unsigned int dataBits = 8, unsigned int stopBits = ONESTOPBIT);
protected:
	virtual bool OpenPort() = 0;

	void Init();
	bool SetupPort();
	void SetComPort(int port);

	bool SetState(int BaudRate, int ByteSize, int Parity, int StopBits);
protected:

	volatile int _port;  
	volatile HANDLE _com_handle;
	char _com_str[20];
	DCB _dcb;     
	COMMTIMEOUTS _co; 
#else
public:
	bool Connect(const std::string& portName, unsigned int baudRate = 9600, unsigned int parity = 'N',
		unsigned int dataBits = 8, unsigned int stopBits = 1);
protected:
    bool openLinuxPort(const SerialPortInfo& portinfo);
    bool setLinuxPortOpt(const SerialPortInfo& portinfo);
protected:
    int _fdSerial{-1};
    bool _isOpen{false};
#endif
protected:
	void SetError(const std::string& msg);
protected:
    SerialPortInfo _portinfo;
	std::string _error;
	std::mutex _mutex;
};

//ͬ������

#ifdef WIN32
class  SyncCom : public BaseCom
{
public:
	SyncCom();

	int Read(char *buf, int buf_len);

	int SendData(const char* buf, int buf_len);
	int SendData(const char* buf);
	
	
protected:
	virtual bool OpenPort();
	
};
#endif

//�첽����

class  ASynCom : public BaseCom
{
public:
	ASynCom();

	virtual ~ASynCom();

	int Read(char* buf, int buf_len);

	bool SendData(const char* buf, int buf_len);
	bool SendData(const std::string& buf);
	//为了满足NetDebugPlug模板要求，增加该函数
	int Send(const char* data, size_t len);

	void Start();
	void Stop();    
	void SetRecvDataCallBack(RecvDataCallBack callback, void* context);

protected:	
	static void OnProcRecvData(void* context);
#ifdef WIN32
protected:
    virtual bool OpenPort();
protected:
	OVERLAPPED _ro, _wo; // �ص�I/O
    OVERLAPPED _wait_o; //WaitCommEvent use
#else
    ssize_t readDataTty(int fd, char *rcv_buf, int TimeOut, int Len);
    ssize_t sendDataTty(int fd, const char *send_buf, int Len);
    void reInit();
#endif
    bool _isExit = false;
	std::thread* _workThread = NULL;
	RecvDataCallBack _recvDataCallback = NULL;
	void* _recvDataContext = NULL;
};




