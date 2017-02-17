#include "Beckhoff.h"
#include "BeckhoffDll.h"
#include "CTcpClient.h"
#include "AMDll.h"

extern "C" int gvm_exec(int funcid, int devcid, double value1, double value2)
{
	return BHgvm_exec(funcid, devcid, value1, value2);
}

extern "C" int  Connect(const char* szSvrIp, int port){
	return BHConnect(szSvrIp, port);
}

extern "C" void  Start(){
	BHStart();
}

extern "C" void  Close(bool bIO){
	BHClose(true);
}