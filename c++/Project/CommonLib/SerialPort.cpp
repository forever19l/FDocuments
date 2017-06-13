#include "SerialPort.h"

#include <cassert>
#ifdef WIN32
#define sprintf sprintf_s
#endif
BaseCom::BaseCom()
{
#ifdef WIN32
    Init();
#endif
}

BaseCom::~BaseCom()
{
	Close();
}

bool BaseCom::Connect(const SerialPortInfo& portinfo)
{
	_portinfo = portinfo;
#ifdef WIN32
	return Connect(portinfo.name, portinfo.baudRate, portinfo.parity, portinfo.dataBits, portinfo.stopBits);
#else
   
   bool result{false};
   result = openLinuxPort(_portinfo);
   if(result)
       result = setLinuxPortOpt(_portinfo);
   return result;
#endif
}

bool BaseCom::Connect(const std::string& portName, unsigned int baudRate, unsigned int parity,
	unsigned int dataBits, unsigned int stopBits)
{
#ifdef WIN32
	int pos = portName.find("COM");
	if (pos == std::string::npos){
		char szError[1024] = { 0 };
		sprintf(szError,  "COM name is error. %s\n", portName.c_str());
		SetError(std::string(szError));
		printf(szError);
		return false;
	}
	std::string sport = portName.substr(pos + 3, portName.size());
	int port = atoi(sport.c_str());
	return Connect(port, baudRate, parity, dataBits, stopBits);
#else    
    _portinfo.name = portName;
    _portinfo.baudRate = baudRate;
    _portinfo.parity = parity;
    _portinfo.dataBits = dataBits;
    _portinfo.stopBits = stopBits;
    return Connect(_portinfo);
#endif
}

#ifdef WIN32
bool BaseCom::Connect(int port, unsigned int baudRate, unsigned int parity, unsigned int dataBits, unsigned int stopBits)
{
	if (port < 1 || port > 1024)
		return false;

	SetComPort(port);

	if (!OpenPort())
		return false;

	if (!SetupPort())
		return false;

	return SetState(baudRate, dataBits, parity, stopBits);
}

bool BaseCom::SetState(int BaudRate, int ByteSize, int Parity, int StopBits)
{
    if (IsOpen())
    {
        if (!GetCommState(_com_handle, &_dcb))
            return false;
        _dcb.BaudRate = BaudRate;
        _dcb.ByteSize = ByteSize;
        _dcb.Parity = Parity;
        _dcb.StopBits = StopBits;
        return SetCommState(_com_handle, &_dcb) == TRUE;
    }
    return false;
}

void BaseCom::Init()
{
    memset(_com_str, 0, 20);
    memset(&_co, 0, sizeof(_co));
    memset(&_dcb, 0, sizeof(_dcb));
    _dcb.DCBlength = sizeof(_dcb);
    _com_handle = INVALID_HANDLE_VALUE;
}

bool BaseCom::SetupPort()
{
    if (!IsOpen())
        return false;

    if (!SetupComm(_com_handle, 8192, 8192))
        return false;

    if (!GetCommTimeouts(_com_handle, &_co))
        return false;
    _co.ReadIntervalTimeout = 0xFFFFFFFF;
    _co.ReadTotalTimeoutMultiplier = 0;
    _co.ReadTotalTimeoutConstant = 0;
    _co.WriteTotalTimeoutMultiplier = 0;
    _co.WriteTotalTimeoutConstant = 2000;
    if (!SetCommTimeouts(_com_handle, &_co))
        return false;

    if (!PurgeComm(_com_handle, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR))
        return false;

    return true;
}

void BaseCom::SetComPort(int port)
{
    char p[12];
    _port = port;
    strcpy_s(_com_str, "\\\\.\\COM");
    _ltoa_s(_port, p, 10);
    strcat_s(_com_str, p);
}

#else
bool BaseCom::openLinuxPort(const SerialPortInfo& portinfo)
{
    _fdSerial = open(portinfo.name.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (-1 == _fdSerial)
    {
		char szError[1024] = { 0 };
		sprintf(szError, "Can't Open %s Serial Port \n", portinfo.name.c_str());
		SetError(std::string(szError));
		printf(szError);
        return(false);
    }
    else
    {
        printf("open %s .....\n", portinfo.name.c_str());
    }

    if(fcntl(_fdSerial, F_SETFL, FNDELAY) < 0)
    {
        printf("set %s noblock failed\n", _portinfo.name.c_str());
    }
    else{
        printf("set %s noblock succ\n", _portinfo.name.c_str());
    }
 
    if (isatty(STDIN_FILENO) == 0)
    {
        printf("%s is not a terminal device\n", portinfo.name.c_str());
    }
    else
    {
        printf("%s is a tty success!\n", portinfo.name.c_str());
    }
    printf("%s is open fdSerial = %d\n", portinfo.name.c_str(), _fdSerial);
    return true;
}

bool BaseCom::setLinuxPortOpt(const SerialPortInfo& portinfo)
{
    int speed_arr[] = {B115200, B57600, B38400, B19200, B9600, B4800, B2400, B1200, B300, B115200, B57600, B38400, B19200, B9600, B4800, B2400, B1200, B300};
    int name_arr[] = {115200, 57600, 38400, 19200, 9600, 4800, 2400, 1200, 300, 115200, 57600, 38400, 19200, 9600, 4800, 2400, 1200, 300};
    struct termios newtio, oldtio;
    if (tcgetattr(_fdSerial, &oldtio) != 0)
    { 
		char szError[1024] = { 0 };
		sprintf(szError, "getSerial %s attr fail", portinfo.name.c_str());
		SetError(std::string(szError));
		printf(szError);
        return false;
    }
    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag |= CLOCAL | CREAD;
    newtio.c_cflag &= ~CSIZE;
        
    switch (portinfo.dataBits)
    {
        case 5:
            newtio.c_cflag |= CS5;
            break;
        case 6:
            newtio.c_cflag |= CS6;
            break;
        case 7:
            newtio.c_cflag |= CS7;
            break;
        case 8:
            newtio.c_cflag |= CS8;
            break;
        default:
            printf("%s bits is not 7 or 8, please check", portinfo.name.c_str());
            newtio.c_cflag |= CS8;
            break;  
    }
        
    switch (portinfo.parity)
    {
        case 'o':
        case 'O':                    
            newtio.c_cflag |= PARENB;
            newtio.c_cflag |= PARODD;
            newtio.c_iflag |= (INPCK);
            break;
        case 'e':
        case 'E':                     
            newtio.c_iflag |= (INPCK | ISTRIP);
            newtio.c_cflag |= PARENB;
            newtio.c_cflag &= ~PARODD;
            break;
        case 'n':
        case 'N':                   
            newtio.c_cflag &= ~PARENB;
            newtio.c_iflag &= ~INPCK;
            break;
        case 's':
        case 'S':
            newtio.c_cflag &= ~PARENB; 
            newtio.c_cflag &= ~CSTOPB;
            newtio.c_iflag |= INPCK;
            break;
        default:
			char szError[1024] = { 0 };
			sprintf(szError, "%s Unsupported parity.\n", portinfo.name.c_str());
			SetError(std::string(szError));
			printf(szError);
            return false;
    }
    
    for(auto i = 0; i < sizeof(speed_arr) / sizeof(int); i++)
    {
        if(portinfo.baudRate == name_arr[i])
        {
            tcflush(_fdSerial, TCIOFLUSH);
            cfsetispeed(&newtio, speed_arr[i]);
            cfsetospeed(&newtio, speed_arr[i]);

            if ((tcsetattr(_fdSerial, TCSANOW, &newtio)) != 0)
            {
				char szError[1024] = { 0 };
				sprintf(szError, "%s set error", portinfo.name.c_str());
				SetError(std::string(szError));
				printf(szError);
                return false;
            }
            tcflush(_fdSerial, TCIOFLUSH);
            break;
        }
    }
        
    if (portinfo.stopBits == 1)
    {
        newtio.c_cflag &= ~CSTOPB;
    }
    else if(portinfo.stopBits == 2)
    {
        newtio.c_cflag |= CSTOPB;
    }
    else
    {
		char szError[1024] = { 0 };
		sprintf(szError, "%s Unsupported stopbits error", portinfo.name.c_str());
		SetError(std::string(szError));
		printf(szError);
        return false;
    }
    
    newtio.c_cflag |= IXON | IXOFF | IXANY;
    
    newtio.c_cflag &= ~CSIZE;
    
    newtio.c_oflag &= ~OPOST;
    
    newtio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    
    newtio.c_cc[VTIME] = 0;
    newtio.c_cc[VMIN] = 0;
    if( tcflush(_fdSerial, TCIOFLUSH) == -1)
    {
        printf(" %s  tcflush failed\n", portinfo.name.c_str());
    }
    printf("%s set done!\n", portinfo.name.c_str());
    _isOpen = true;
    return true;

}

#endif

void BaseCom::Close()
{
#ifdef WIN32
	if (IsOpen())
	{
		CloseHandle(_com_handle);
		_com_handle = INVALID_HANDLE_VALUE;
	}
#else
    if (_fdSerial > 0)
        close(_fdSerial);
#endif

}

bool BaseCom::IsOpen()
{
#ifdef WIN32
    return _com_handle != INVALID_HANDLE_VALUE;
#else
    return _isOpen;
#endif
}



#ifdef WIN32
SyncCom::SyncCom()
{
}

bool SyncCom::OpenPort()
{
	if (IsOpen())
		Close();

	_com_handle = CreateFileA(
		_com_str,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);

	return IsOpen();
}

int SyncCom::Read(char* buf, int buf_len)
{
	if (!IsOpen())
		return 0;

	buf[0] = '\0';

	COMSTAT  stat;
	DWORD error;

	if (ClearCommError(_com_handle, &error, &stat) && error > 0) 
	{
		PurgeComm(_com_handle, PURGE_RXABORT | PURGE_RXCLEAR);
		return 0;
	}

	unsigned long r_len = 0;

	buf_len = min(buf_len - 1, (int)stat.cbInQue);
	if (!ReadFile(_com_handle, buf, buf_len, &r_len, NULL))
		r_len = 0;
	buf[r_len] = '\0';

    return r_len;
}

int SyncCom::SendData(const char* buf, int buf_len)
{
	if (!IsOpen() || !buf)
		return 0;

	DWORD    error;
	if (ClearCommError(_com_handle, &error, NULL) && error > 0) 
		PurgeComm(_com_handle, PURGE_TXABORT | PURGE_TXCLEAR);

	unsigned long w_len = 0;
	if (!WriteFile(_com_handle, buf, buf_len, &w_len, NULL))
		w_len = 0;

	return w_len;
}

int SyncCom::SendData(const char* buf)
{
	return SendData(buf, strlen(buf));
}
 #endif

ASynCom::ASynCom()
{
#ifdef WIN32
	memset(&_ro, 0, sizeof(_ro));
	memset(&_wo, 0, sizeof(_wo));

	_ro.hEvent = CreateEvent(NULL, true, false, NULL);
	assert(_ro.hEvent != INVALID_HANDLE_VALUE);

	_wo.hEvent = CreateEvent(NULL, true, false, NULL);
	assert(_wo.hEvent != INVALID_HANDLE_VALUE);


	memset(&_wait_o, 0, sizeof(_wait_o));
	_wait_o.hEvent = CreateEvent(NULL, true, false, NULL);
	assert(_wait_o.hEvent != INVALID_HANDLE_VALUE);
#endif
}


ASynCom::~ASynCom()
{
    Close();
#ifdef WIN32


	if (_ro.hEvent != INVALID_HANDLE_VALUE)
		CloseHandle(_ro.hEvent);

	if (_wo.hEvent != INVALID_HANDLE_VALUE)
		CloseHandle(_wo.hEvent);

	if (_wait_o.hEvent != INVALID_HANDLE_VALUE)
		CloseHandle(_wait_o.hEvent);
#endif
    Stop();
}

#ifdef WIN32
bool ASynCom::OpenPort()
{
#ifdef WIN32
	if (IsOpen())
		Close();

	_com_handle = CreateFileA(
		_com_str,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 
		NULL
		);
	return IsOpen();
 #endif
    return false;
}
#endif

int ASynCom::Read(char* buf, int buf_len)
{
#ifdef WIN32
	if (!IsOpen())
		return 0;

	buf[0] = '\0';

	COMSTAT  stat;
	DWORD error;

	if (ClearCommError(_com_handle, &error, &stat) && error > 0) 
	{
		PurgeComm(_com_handle, PURGE_RXABORT | PURGE_RXCLEAR); 
		return 0;
	}

	if (!stat.cbInQue)
		return 0;

	unsigned long r_len = 0;

	buf_len = min((int)(buf_len - 1), (int)stat.cbInQue);

	if (!ReadFile(_com_handle, buf, buf_len, &r_len, &_ro)) 
	{
		if (GetLastError() == ERROR_IO_PENDING) 
		{
			if (!GetOverlappedResult(_com_handle, &_ro, &r_len, false))
			{
				if (GetLastError() != ERROR_IO_INCOMPLETE)
					r_len = 0;
			}
		}
		else
			r_len = 0;
	}

	buf[r_len] = '\0';
	return r_len;
 #else
    return (int)readDataTty(_fdSerial, buf, 20, buf_len); //100 没有数据就等待100毫秒
 #endif
	
}

bool ASynCom::SendData(const char* buf, int buf_len)
{
#ifdef WIN32
	if (!IsOpen())
		return false;

	DWORD error;
	if (ClearCommError(_com_handle, &error, NULL) && error > 0) 
		PurgeComm(_com_handle, PURGE_TXABORT | PURGE_TXCLEAR);

	unsigned long w_len = 0, o_len = 0;
	if (!WriteFile(_com_handle, buf, buf_len, &w_len, &_wo)){
		if (GetLastError() != ERROR_IO_PENDING)
			return false;
	}

	return true;
#else
   ssize_t sendlen = sendDataTty(_fdSerial, buf, buf_len);
   return sendlen != -1;
#endif
}

int ASynCom::Send(const char* buf, size_t buf_len)
{
#ifdef WIN32
	if (!IsOpen())
		return -1;

	DWORD error;
	if (ClearCommError(_com_handle, &error, NULL) && error > 0)
		PurgeComm(_com_handle, PURGE_TXABORT | PURGE_TXCLEAR);

	unsigned long w_len = 0, o_len = 0;
	if (!WriteFile(_com_handle, buf, buf_len, &w_len, &_wo)){
		if (GetLastError() != ERROR_IO_PENDING)
			return false;
	}

	return buf_len;
#else
	ssize_t sendlen = sendDataTty(_fdSerial, buf, buf_len);
	return sendlen;
#endif
}

bool ASynCom::SendData(const std::string& buf)
{
#ifdef WIN32
	return SendData(buf.c_str(), buf.size());
#else
    ssize_t sendlen = sendDataTty(_fdSerial, buf.c_str(), (int)buf.size());
    return sendlen != -1;
#endif
}

void ASynCom::SetRecvDataCallBack(RecvDataCallBack callback, void* context)
{
	_recvDataCallback = callback;
	_recvDataContext = context;
}

void ASynCom::Start()
{
	_isExit = false;
	_workThread = new std::thread(ASynCom::OnProcRecvData, this);
}

void ASynCom::Stop()
{
	_isExit = false;
	if (_workThread != NULL)
	{
#ifdef WIN32
		TerminateThread(_workThread->native_handle(), 1);
#endif
		if (_workThread->joinable())
			_workThread->join();
		delete _workThread;
		_workThread = NULL;
	}
}

void ASynCom::OnProcRecvData(void* context)
{
	ASynCom *pcom = (ASynCom *)context;
#ifdef WIN32
	if (!SetCommMask(pcom->_com_handle, EV_RXCHAR | EV_ERR))
		return;

	COMSTAT  stat;
	DWORD error;
	DWORD mask;
	DWORD length;
	while (!pcom->_isExit){
		if (!WaitCommEvent(pcom->_com_handle, &mask, &pcom->_wait_o))
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				GetOverlappedResult(pcom->_com_handle, &pcom->_wait_o, &length, true);
			}
		}

		if (mask & EV_ERR) 
			ClearCommError(pcom->_com_handle, &error, &stat);

		if (mask & EV_RXCHAR) 
		{
			ClearCommError(pcom->_com_handle, &error, &stat);
			if (stat.cbInQue > 0){
				char buf[1024] = { 0 };
				int buflen = sizeof(buf);
				int r = pcom->Read(buf, buflen);
				if (r > 0){
					std::string msg;
					msg.assign(buf, r);
					if (pcom->_recvDataCallback != NULL && !msg.empty()){
						pcom->_recvDataCallback(msg, pcom->_recvDataContext);
					}
					else{
						printf("SerialPort recv data:%s\n", msg.c_str());
					}
				}
			}
		}
	}
#else
    char buf[1024] = { 0 };
    int ibuflen = sizeof(buf)/ sizeof(char);
    while (!pcom->_isExit)
    {
        memset(buf,0,ibuflen);        
        int r = pcom->Read(buf, ibuflen);
        if (r > 0)
        {
            std::string msg;
            msg.assign(buf, r);
            if (pcom->_recvDataCallback != NULL && !msg.empty())
            {
                pcom->_recvDataCallback(msg, pcom->_recvDataContext);
            }
            else{
                if(!msg.empty())
                    printf("%s recv data:%s\n", pcom->_portinfo.name.c_str(), msg.c_str());
            }
         }
        
    }
#endif
}

#ifndef WIN32
ssize_t ASynCom::readDataTty(int fd, char *rcv_buf, int TimeOut, int Len)
{
    if(!_isOpen)
    {
        if(Connect(_portinfo))
        {
            fd = _fdSerial;
        }
        else
        {
            return -1;
        }
    }
    int retval;
    fd_set rfds;
    struct timeval tv;
    ssize_t ret = 0, pos = 0;
    tv.tv_sec = TimeOut / 1000;  //set the rcv wait time
    tv.tv_usec = TimeOut % 1000 * 1000;  //100000us = 0.1s
    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);
    while (FD_ISSET(fd, &rfds))
    {
        FD_ZERO(&rfds);
        FD_SET(fd, &rfds);
        retval = select(fd + 1, &rfds, NULL, NULL, &tv);
        if (retval == -1)
        {
            break;
        }
        else if (retval)
        {
            ret = read(fd, rcv_buf + pos, Len-pos);
            if (-1 == ret)
            {
                close(_fdSerial);
                _isOpen = false;
                break;
            }

            if (ret == 0) {
                break;
            }
            pos += ret;
            if (Len <= pos)
            {
                break;
            }

            FD_ZERO(&rfds);
            FD_SET(fd,&rfds);
            retval = select(fd+1,&rfds,NULL,NULL,&tv);
            if(retval <= 0)
            {
                break;
            }
        }
        else
        {
            
            break;
        }
    }
    return pos;
}

ssize_t ASynCom::sendDataTty(int fd, const char *send_buf, int Len)
{
    ssize_t ret;
    if(!_isOpen)
    {
        if(Connect(_portinfo))
            fd = _fdSerial;
        else
            return -1;
    }
    std::string sbuf;
    sbuf.assign(send_buf, Len);
    int pos = 0, ilen = (int)sbuf.size();
    while(pos != ilen)
    {
        ret = write(fd, sbuf.c_str()+ pos, ilen-pos);
        if (ret == -1)
        {
			char szError[1024] = { 0 };
			sprintf(szError, "%s write %s error \n",  _portinfo.name.c_str(), sbuf.c_str());
			SetError(std::string(szError));
			printf(szError);
            return -1;
        }
        pos += ret;
    }
    return pos;
}
#endif


std::string BaseCom::GetError()
{
	std::unique_lock<std::mutex> lock(_mutex);
	return _error;
}

void BaseCom::SetError(const std::string& msg)
{
	std::unique_lock<std::mutex> lock(_mutex);
	_error = msg;
}
