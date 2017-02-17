extern "C" __declspec(dllexport) int  Connect(const char* szSvrIp, int port);
extern "C" __declspec(dllexport) void  Start();
extern "C" __declspec(dllexport) void  Close(bool bIO);
extern "C" __declspec(dllexport) int gvm_exec(int funcid, int devcid, double value1, double value2);
