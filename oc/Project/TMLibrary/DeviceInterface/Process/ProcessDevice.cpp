//
//  ProcessDevice.cpp
//  DevInterface
//
//  Created by Ryan on 1/19/17.
//  Copyright © 2017 ___Intelligent Automation___. All rights reserved.
//

#include "ProcessDevice.hpp"


uint64_t TimeInterval(const struct timeval *start, const struct timeval *end)
{
    if (start->tv_usec > end->tv_usec) {
        return ((end->tv_sec - 1 - start->tv_sec) * 1000 + (1e6 + end->tv_usec - start->tv_usec) / 1000);
    } else {
        return ((end->tv_sec - start->tv_sec) * 1000 + (end->tv_usec - start->tv_usec) / 1000);
    }
}


int popen_rw(const char *cmdString, int inPipe[2])
{
#define SHELL "/bin/bash"
    
    int pipefd_r[2];           //管道描述符
    int pipefd_w[2];
    
    
    if (pipe(pipefd_r) < 0)        //建立管道
    {
        printf("create pipe error:%s/n", strerror(errno));
        return -1;
    }
    
    if (pipe(pipefd_w) < 0)        //建立管道
    {
        printf("create pipe error:%s/n", strerror(errno));
        return -1;
    }
    
    
    pid_t pid = fork();             //建立子进程
    
    if (pid < 0) {
        printf("fork error:%s\n", strerror(errno));
        return -2;
    }
    
    if (0 == pid)            //子进程
    {
        close(pipefd_w[1]);
        dup2(pipefd_w[0], STDIN_FILENO);
        close(pipefd_w[0]);
        
        close(pipefd_r[0]);
        dup2(pipefd_r[1], STDOUT_FILENO);
        close(pipefd_r[1]);
        
        
        execl(SHELL, "sh", "-c", cmdString, NULL);
        _exit(127);
    }
    
    
    close(pipefd_w[0]);
    inPipe[1] = pipefd_w[1];
    
    close(pipefd_r[1]);
    inPipe[0] = pipefd_r[0];
    
    
    return pid;
}




CProcessDevice::CProcessDevice(const char *deviceName, CPublisher *publisher)
                :m_inBuffer(), m_outBuffer(), m_detectStr(), m_userBuffer(),
                m_deviceName(deviceName), m_pid(0), m_tid(0), m_runFlag(true),
                m_regexFlag(false), m_publisher(publisher)
{
    m_pipe[0] = -1;
    m_pipe[1] = -1;
}


CProcessDevice::~CProcessDevice(void) {
    
}

int CProcessDevice::Open(const char *cmdString) {
    
    int ret = 0;
    
    if (NULL == cmdString) {
        printf("cmdString is nil\n");
        return -1;
    }
    
    m_pid = popen_rw(cmdString, m_pipe);
    if (m_pid < 0) {
        ret = -1;
        return ret;
    }
    
    ret = pthread_create(&m_tid, NULL, CProcessDevice::CopyChildProcessOutputToBuffer, this);
    if (ret < 0) {
        printf("create thread CopyOutputToBuffer failure:%s\n", strerror(ret));
        Close();
    }
    
    return ret;
}


int CProcessDevice::Close(void) {
    if (m_pid > 0) {
        kill(m_pid, SIGTERM);
        m_pid = 0;
    }
    
    if (m_tid > 0) {
        m_runFlag = false;
        pthread_cancel(m_tid);
        pthread_join(m_tid, NULL);
        
        m_tid = 0;
    }
    
    ::close(m_pipe[0]);
    ::close(m_pipe[1]);
    
    m_inBuffer.clear();
    m_outBuffer.clear();
    m_userBuffer.clear();
    m_detectStr.clear();
    m_deviceName.clear();
    
    m_publisher = NULL;
    
    return 0;
}


void CProcessDevice::ClearBuffer(void) {
    m_readMutex.lock();
    m_inBuffer.clear();
    m_readMutex.unlock();
}


int CProcessDevice::SetDetectString(const char *regex, int flag) {
    m_detectStr.clear();
    m_detectStr.assign(regex);
    m_regexFlag = flag;
    
    return 0;
}


int CProcessDevice::WaitForString(int timeoutMilliSec) {
    
    struct timeval start, end;
    gettimeofday(&start, NULL);
    
    while (true) {
        gettimeofday(&end, NULL);
        if (TimeInterval(&start, &end) > timeoutMilliSec) {
            printf("Time out wait for string:%s\n", m_detectStr.c_str());
            return -1;
        }

        m_readMutex.lock();
        if (m_inBuffer.find(m_detectStr) != string::npos) {
            m_readMutex.unlock();
            return 1;
        } else {
            m_readMutex.unlock();
        }

        
        usleep(1000);
    }
    
    
}

const char *CProcessDevice::ReadString(void) {
    
    m_readMutex.lock();
    if (m_inBuffer.length() > 0) {
        
        m_userBuffer.assign(m_inBuffer);
        m_inBuffer.clear();
        
    }
    m_readMutex.unlock();
    
    printf("read str:%s\n", m_userBuffer.c_str());
    
    return m_userBuffer.c_str();
}


int CProcessDevice::WriteString(const char *cmd) {
    size_t cmdLen = strlen(cmd);
    
    if (write(m_pipe[1], cmd, cmdLen) != cmdLen) {
        printf("send cmd:%s error:%s\n", cmd, strerror(errno));
        return -1;
    } else {
        return 0;
    }
}


void* CProcessDevice::CopyChildProcessOutputToBuffer(void *parm) {
    CProcessDevice *devPtr = (CProcessDevice*)parm;
    if (NULL == devPtr) {
        return NULL;
    }
    
    int nfds;
    fd_set readfds;
    int inPutFd = devPtr->m_pipe[0];
    char buf[512] = "\0";
    
    
    while (devPtr->m_runFlag) {
        
        FD_ZERO(&readfds);
        FD_SET(inPutFd, &readfds);
        nfds = inPutFd + 1;
        
        int ret = select(nfds, &readfds, NULL, NULL, NULL);
        if (ret < 0) {
            if (errno == EINTR) {
                continue;
            } else {
                printf("call select func failure:%s in thread:%s\n", strerror(errno), __func__);
                printf("thread exit now\n");
                break;
            }
        }
        
        if (FD_ISSET(inPutFd, &readfds)) {
//            bzero(buf, sizeof buf);
            
            ssize_t nbyte = read(inPutFd, buf, sizeof buf);
            if (nbyte > 0) {
                devPtr->m_readMutex.lock();
                devPtr->m_inBuffer.append(buf, nbyte);
                devPtr->m_readMutex.unlock();
            }
        }
        
    }
    
    return NULL;
}

