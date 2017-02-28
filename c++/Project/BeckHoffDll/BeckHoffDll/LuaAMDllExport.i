%module BeckHoffDll
%inline %{
#include "BeckhoffDll.h"
%}

int  am_connect(const char* szSvrIp, int port);
void  am_close();
int am_exec(int funcid, int devcid, double value1, double value2);
int am_exec_two(int f1, int d1, double v1, double v2, int f2, int d2, double v3, double v4);
int am_exec_th(int f1, int d1, double v1, double v2, int f2, int d2, double v3, double v4, int f3, int d3, double v5, double v6);
void am_add_data(int index, int funcid, int devicid, double v1, double v2);
int am_send_data(int count);
void am_delay(int time);