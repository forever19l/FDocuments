#include "BeckhoffDll.h"

BeckHoff BH;

extern "C" int am_connect(const char* szSvrIp, int port){
	int ret = BH.Connect(szSvrIp, port);
	BH.Start();
	return ret;
}
extern "C" void am_close(){
	 BH.Close(true);
}
extern "C" int am_exec(int funcid, int devcid, double value1, double value2){
	return BH.gvm_exec(funcid, devcid, value1, value2);
}
extern "C" int am_exec_two(int f1, int d1, double v1, double v2, int f2, int d2, double v3, double v4){
	return BH.gvm_exec_two(f1, d1, v1, v2, f2, d2, v3, v4);
}
extern "C" int am_exec_th(int f1, int d1, double v1, double v2, int f2, int d2, double v3, double v4, int f3, int d3, double v5, double v6){
	return BH.gvm_exec_th(f1, d1, v1, v2, f2, d2, v3, v4, f3, d3, v5, v6);
}
extern "C" void am_add_data(int index, int funcid, int devicid, double v1, double v2){
	return BH.gvm_add_data(index, funcid, devicid, v1, v2);
}
extern "C" int am_send_data(int count){
	return BH.gvm_send_data(count);
}
extern "C" void am_delay(int time){
	Sleep(time);
}