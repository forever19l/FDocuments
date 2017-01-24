//
//  RS232.m
//  RS232
//
//  Created by IvanGan on 16/4/9.
//  Copyright © 2016年 IvanGan. All rights reserved.
//

#import "SerialPort.h"

#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <iostream>

#include <sys/ioctl.h>

CSerialPort::CSerialPort()
{
    fd = -1;
    bReadDataInBackGround = false;
}

CSerialPort::~CSerialPort()
{
    this->close();
    fd = -1;
    bReadDataInBackGround = false;
}

int CSerialPort::connect(const char * dev)
{
    printf("CSerialPort::connect");
    fd = open(dev, O_RDWR|O_NOCTTY|O_NDELAY);
    if (-1 == fd)
    {
        perror("Can't Open Serial Port");
        return(-1);
    }
    else
    {
        printf("open %s .....\n",dev);
    }
    
    if(fcntl(fd, F_SETFL, 0)<0)
    {
        printf("fcntl failed!\n");
    }
    else
    {
        printf("fcntl=%d\n",fcntl(fd, F_SETFL,0));
    }
    if(isatty(STDIN_FILENO)==0)
    {
        printf("standard input is not a terminal device\n");
    }
    else
    {
        printf("isatty success!\n");
    }
    printf("fd-open=%d\n",fd);
    
    bReadDataInBackGround = true;
    pthread_create(&m_thread, nullptr, ReadDataInBackGround, this);
    return fd;
}

int CSerialPort::set_opt(int nSpeed, int nBits, char nEvent, int nStop)
{
    struct termios newtio,oldtio;
    if  ( tcgetattr( fd,&oldtio)  !=  0)
    {
        perror("SetupSerial 1");
        return -1;
    }
    bzero( &newtio, sizeof( newtio ) );
    newtio.c_cflag  |=  CLOCAL | CREAD;
    newtio.c_cflag &= ~CSIZE;
    
    switch( nBits )
    {
        case 7:
            newtio.c_cflag |= CS7;
            break;
        case 8:
            newtio.c_cflag |= CS8;
            break;
    }
    
    switch( nEvent )
    {
        case 'O':                  //奇校验
        case 'o':
            newtio.c_cflag |= PARENB;
            newtio.c_cflag |= PARODD;
            newtio.c_iflag |= (INPCK | ISTRIP);
            break;
        case 'E':                     //偶校验
        case 'e':
            newtio.c_iflag |= (INPCK | ISTRIP);
            newtio.c_cflag |= PARENB;
            newtio.c_cflag &= ~PARODD;
            break;
        case 'N':                    //无校验
        case 'n':
            newtio.c_cflag &= ~PARENB;
            break;
    }
    
    switch( nSpeed )
    {
        case 2400:
            cfsetispeed(&newtio, B2400);
            cfsetospeed(&newtio, B2400);
            break;
        case 4800:
            cfsetispeed(&newtio, B4800);
            cfsetospeed(&newtio, B4800);
            break;
        case 9600:
            cfsetispeed(&newtio, B9600);
            cfsetospeed(&newtio, B9600);
            break;
//        case 19200:
//            cfsetispeed(&newtio, B19200);
//            cfsetispeed(&newtio, B19200);
//            break;
        case 115200:
            cfsetispeed(&newtio, B115200);
            cfsetospeed(&newtio, B115200);
            break;
        default:
            cfsetispeed(&newtio, B19200);
            cfsetospeed(&newtio, B19200);
            break;
    }
    if( nStop == 1 )
    {
        newtio.c_cflag &=  ~CSTOPB;
    }
    else if ( nStop == 2 )
    {
        newtio.c_cflag |=  CSTOPB;
    }
    newtio.c_cc[VTIME]  = 0;
    newtio.c_cc[VMIN] = 0;
    tcflush(fd,TCIFLUSH);
    if((tcsetattr(fd,TCSANOW,&newtio))!=0)
    {
        perror("com set error");
        return -1;
    }
    printf("set done!\n");
    return 0;
}

int CSerialPort::close()
{
    bReadDataInBackGround = false;
    void * p;
    pthread_join(m_thread, &p);
    if(fd>0)
        return ::close(fd);
    return 0;
}

int CSerialPort::setCallBack(OnReceiveData pf, void * context)
{
    m_CallBack = pf;
    m_pCallBackContext = context;
    return 0;
}

void * CSerialPort::didReceiveData(void * data, long len)
{
    if(m_CallBack)
        m_CallBack(data, len, m_pCallBackContext);
    return 0;
}

void * CSerialPort::ReadDataInBackGround(void * arg)
{
    CSerialPort * pThis = (CSerialPort *)arg;
    while (pThis -> bReadDataInBackGround) {
        pthread_testcancel();
        int sk = pThis->fd;
        fd_set readset, writeset;
        FD_ZERO(&readset);
        FD_ZERO(&writeset);
        FD_SET(sk, &readset);
        FD_SET(sk, &writeset);
        
        int maxfd = sk + 1;
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;
        
        int ret = select(maxfd, &readset, NULL, NULL, &timeout);
        switch(ret)
        {
            case -1:
            case 0:
                break;
            default:
                if (FD_ISSET(pThis->fd,&readset))
                {
                    int bytes;
                    int n = ioctl(sk, FIONREAD, & bytes);
                    if(n==-1)
                    {
                        std::cout<<"[IO CTRL],get FIONREAD failed,with return:"<<errno<<std::endl;
                        continue;
                    }
                    if(bytes <= 0)
                        break;
                    unsigned char * pBuffer = new unsigned char[bytes];
                    memset(pBuffer, 0, bytes);
                    size_t recvbytes = ::read(pThis->fd, pBuffer, bytes);
                    if (recvbytes <= 0)
                    {
                        delete[] pBuffer;
                        break;
                    }
                    pThis->didReceiveData(pBuffer, recvbytes);
                    delete [] pBuffer;
                }
        }
        timeout.tv_sec = 0;
        usleep(1000);
    }
    return nullptr;
}

int CSerialPort::write(void * buffer, long len)
{
    if(fd>0)
    {
        int ret =  ::write(fd, buffer, len);
        if(ret < 0)
            std::cout<<"[TCPCLIENT]:Writ socket faile,with return error : "<<errno<<std::endl;
        return ret;
    }
    return -1;
}

int CSerialPort::read(void * buffer, int len)
{
    if(fd>0)
    {
        return ::read(fd, buffer, len);
    }
    return -1;
}

//int main(void)
//{
//    int fd;
//    int nread,i;
//    char buff[]="Hello\n";
//    
//    if((fd=open_port(fd,1))<0)
//    {
//        perror("open_port error");
//        return;
//    }
//    if((i=set_opt(fd,115200,8,'N',1))<0)
//    {
//        perror("set_opt error");
//        return;
//    }
//    printf("fd=%d\n",fd);
//    
//    nread=read(fd,buff,8);
//    printf("nread=%d,%s\n",nread,buff);
//    close(fd);
//    return;
//}

