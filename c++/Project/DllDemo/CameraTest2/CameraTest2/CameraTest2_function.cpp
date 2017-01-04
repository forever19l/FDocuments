#include "CameraTest2_function.h"
#include "CameraDLL.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

bool InitCamera(const char* cameraIp){
	printf("for lua InitCamera success\n");
	char ip[100] = {};
	strncpy_s(ip, cameraIp, 100);
	IAInitCamera(ip);
	return true;
}

bool SetCameraReady(const char* saveImagePath, int timeOut){
	printf("for lua SetCameraReady success\n");
	char path[100] = {};
	strncpy_s(path, saveImagePath, 100);
	IASetCameraReady(path, timeOut);
	return true;
}

bool GetCaptureStatus(){
	printf("for lua GetCaptureStatus success\n");
	IAGetCaptureStatus();
	return true;
}

bool CloseCamera(const char* cameraIp){
	printf("for lua CloseCamera success\n");
	char ip[100] = {};
	strncpy_s(ip, cameraIp, 100);
	IACloseCamera(ip);
	return true;
}

int test(int a , int b){
	printf("a = %d ;b = %d \n", a, b);
	int c = iatest(a, b);
	if (c == (a + b)){
		printf("true!!!!!!!!!!!!!!!!!!!");
	}
	else
	{
		printf("error ****************");
	}
	return c;
}