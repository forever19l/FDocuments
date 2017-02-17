
#include "Beckhoff.h"

extern "C" __declspec(dllexport) int  BHConnect(const char* szSvrIp, int port);
extern "C" __declspec(dllexport) void  BHClose(bool bIO);
extern "C" __declspec(dllexport) void  BHStart();
extern "C" __declspec(dllexport) int BHgvm_exec(int funcid, int devcid, double value1, double value2);
