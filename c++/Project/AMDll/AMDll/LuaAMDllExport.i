%module AMDll
%inline %{
#include "AMDll.h"
%}

int  Connect(const char* szSvrIp, int port);
void  Start();
void  Close(bool bIO);
int gvm_exec(int funcid, int devcid, double value1, double value2);
