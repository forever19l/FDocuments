 /* DevInterface.i */
 %module DevInterface
 %{
 /* Put header files here or function declarations like below */
#include "DeviceInterface.hpp"
#include "ProcessDevice.hpp"
#include "Publisher.hpp"
 %}
 
extern int popen_rw(const char *cmdString, int inPipe[2]);
extern uint64_t TimeInterval(const struct timeval *start, const struct timeval *end);

class CDevInterface {
public:
    CDevInterface(void) {
        
    }
    
    CDevInterface(const char *deviceName, CPublisher *publiser = NULL) {
        
    }
    
    virtual ~CDevInterface(void) {
    
    }
    
public:
    //Base
    virtual int Open(const char *cmdString)=0;
    virtual int Close(void)=0;
    virtual int WriteString(const char *cmd)=0;
    virtual const char *ReadString(void)=0;
    virtual int SetDetectString(const char *regex, int flag = 0)=0;
    virtual int WaitForString(int timeoutMilliSec = 30*1000)=0;
    virtual void ClearBuffer(void)=0;

//    virtual int Write(const void *buffer, unsigned long len);
//    virtual int Read(void *buffer, int len);
};

class CPublisher {
    CPublisher();
    virtual ~CPublisher();
    
public:
    virtual int CreatePubliser();
    virtual int ClosePubliser();
    virtual int publish(void * msg,int len,int level=0);
};


class CProcessDevice : public CDevInterface {
public:
    
    CProcessDevice(const char *deviceName, CPublisher *publisher = NULL);
    virtual ~CProcessDevice(void);
    
public:
    virtual int Open(const char *cmdString);
    virtual int Close(void);
    virtual int WriteString(const char * cmd);
    virtual const char *ReadString(void);
    virtual int SetDetectString(const char *regex, int flag = 0);
    virtual int WaitForString(int timeoutMilliSec = 30*1000);
    virtual void ClearBuffer(void);

private:
    static void *CopyChildProcessOutputToBuffer(void *parm);
    
private:
    string m_inBuffer;
    string m_outBuffer;
    string m_detectStr;
    string m_userBuffer;
    
    string m_deviceName;
    
    mutex m_writeMutex;
    mutex m_readMutex;
    
    int m_pipe[2];
    
    pid_t m_pid;
    pthread_t m_tid;
    
    bool m_runFlag;
    
    int m_regexFlag;
    
    CPublisher *m_publisher;
    
};
