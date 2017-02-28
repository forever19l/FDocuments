#include "Beckhoff.h"

extern "C" __declspec(dllexport) int  am_connect(const char* szSvrIp, int port);
extern "C" __declspec(dllexport) void  am_close();
extern "C" __declspec(dllexport) int am_exec(int funcid, int devcid, double value1, double value2);
extern "C" __declspec(dllexport) int am_exec_two(int f1, int d1, double v1, double v2, int f2, int d2, double v3, double v4);
extern "C" __declspec(dllexport) int am_exec_th(int f1, int d1, double v1, double v2, int f2, int d2, double v3, double v4, int f3, int d3, double v5, double v6);
extern "C" __declspec(dllexport) void am_add_data(int index, int funcid, int devicid, double v1, double v2);
extern "C" __declspec(dllexport) int am_send_data(int count);
extern "C" __declspec(dllexport) void am_delay(int time);
