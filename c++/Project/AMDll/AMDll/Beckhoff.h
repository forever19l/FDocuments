/*
**  FILENAME            Beckhoff.h
**  PURPOSE             This is BekcHoff library interface that can drive BeckHoff PLC through TCP/IP as Client.
**  CREATION DATE       2/6/2016
**  AUTHOR              Kevin Chen
**
*/

#ifndef _BECKHOFF_H_
#define _BECKHOFF_H_

#include "CTcpClient.h"
#include <string>
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BeckHoff_EXPORTS
#ifdef BeckHoff_EXPORTS
#define BeckHoff_API __declspec(dllexport) 
#else
#define BeckHoff_API __declspec(dllimport) 
#endif

class BeckHoff_API BeckHoff
{
public:
	BeckHoff();
	virtual ~BeckHoff();

public:

	/*****************************************************/
	/*函数名:Connect
	/*描述:连接BeckHoff PLC
	/*输入:szSvrIp，IP地址； port，端口号
	/*输出:无
	/*返回:状态码 0, OK; other, Fail
	/*****************************************************/
	int  Connect(const char* szSvrIp, int port);

	/*****************************************************/
	/*函数名:Close
	/*描述:关闭连接BeckHoff PLC
	/*输入:无
	/*输出:无
	/*返回:无
	/*****************************************************/
	void  Close(bool bIO);

	/*****************************************************/
	/*函数名:Start
	/*描述:启动监听BeckHoff PLC
	/*输入:无
	/*输出:无
	/*返回:无
	/*****************************************************/
	void  Start();

	/*****************************************************/
	/*函数名:gvm_exec
	/*描述:发送单包cmd
	/*输入:默认functionid, deviceid, value1, value2
	/*输出:无
	/*返回:状态码 true, OK; false, Fail
	/*****************************************************/
	int gvm_exec(int funcid, int devcid, double value1, double value2);

	/*****************************************************/
	/*函数名:GetCurrentXYZU
	/*描述:获取BeckHoff PLC当前坐标点
	/*输入:无
	/*输出:cx, cy, cz, cu坐标值
	/*返回:状态码 0, OK; other, Fail
	/*****************************************************/
	int __stdcall GetCurrentXYZU(double* cx, double* cy, double* cz, double* cu);

	/*****************************************************/
	/*函数名:AbsMoveX
	/*描述:移动BeckHoff PLC X轴到当前目标点
	/*输入:cx, X轴移动绝对坐标值；vel, X轴移动速度
	/*输出:无
	/*返回:状态码 0, OK; other, Fail
	/*****************************************************/
	int __stdcall AbsMoveX(double cx, double vel);

	/*****************************************************/
	/*函数名:AbsMoveY
	/*描述:移动BeckHoff PLC Y轴到当前目标点
	/*输入:cy, Y轴移动绝对坐标值；vel, Y轴移动速度
	/*输出:无
	/*返回:状态码 0, OK; other, Fail
	/*****************************************************/
	int __stdcall AbsMoveY(double cy, double vel);

	/*****************************************************/
	/*函数名:AbsMoveZ
	/*描述:移动BeckHoff PLC Z轴到当前目标点
	/*输入:cz, Z轴移动绝对坐标值；vel, Z轴移动速度
	/*输出:无
	/*返回:状态码 0, OK; other, Fail
	/*****************************************************/
	int __stdcall AbsMoveZ(double cz, double vel);

	/*****************************************************/
	/*函数名:AbsMoveTheta
	/*描述:移动BeckHoff PLC U轴到当前目标点
	/*输入:cu, U轴移动绝对坐标值；vel, U轴旋转速度
	/*输出:无
	/*返回:状态码 0, OK; other, Fail
	/*****************************************************/
	int __stdcall AbsMoveTheta(double cu, double vel);

	/*****************************************************/
	/*函数名:RelMoveX
	/*描述:移动BeckHoff PLC X轴当前距离
	/*输入:cx, X轴移动相对距离；vel, X轴移动速度
	/*输出:无
	/*返回:状态码 0, OK; other, Fail
	/*****************************************************/
	int __stdcall RelMoveX(double cx, double vel);

	/*****************************************************/
	/*函数名:RelMoveY
	/*描述:移动BeckHoff PLC Y轴当前距离
	/*输入:cy, Y轴移动相对距离；vel, Y轴移动速度
	/*输出:无
	/*返回:状态码 0, OK; other, Fail
	/*****************************************************/
	int __stdcall RelMoveY(double cy, double vel);

	/*****************************************************/
	/*函数名:RelMoveZ
	/*描述:移动BeckHoff PLC Z轴当前距离
	/*输入:cz, Z轴移动相对距离；vel, Z轴移动速度
	/*输出:无
	/*返回:状态码 0, OK; other, Fail
	/*****************************************************/
	int __stdcall RelMoveZ(double cz, double vel);

	/*****************************************************/
	/*函数名:RelMoveTheta
	/*描述:移动BeckHoff PLC U轴当前的角度
	/*输入:cu, U轴移动相对角度；vel, U轴旋转速度
	/*输出:无
	/*返回:状态码 0, OK; other, Fail
	/*****************************************************/
	int __stdcall RelMoveTheta(double cu, double vel);

	/*****************************************************/
	/*函数名:MoveNozzleUp
	/*描述:移动BeckHoff PLC 吸嘴上下
	/*输入:nozzle, 吸嘴编号（1~4）；bUp， 上下动作
	/*输出:无
	/*返回:状态码 0, OK; other, Fail
	/*****************************************************/
	int __stdcall NozzleUpDn(int nozzle, bool bDN);

	/*****************************************************/
	/*函数名:SuckNozzle
	/*描述:BeckHoff PLC 吸嘴开关真空
	/*输入:nozzle, 吸嘴编号（1~4）；bSuck， 打开/关闭真空
	/*输出:无
	/*返回:状态码 0, OK; other, Fail
	/*****************************************************/
	int __stdcall NozzleSuckOrNot(int nozzle, bool bSuck);

	/*****************************************************/
	/*函数名:BottomCameraLightOnOff
	/*描述:控制BeckHoff PLC 开关底部标定相机光源
	/*输入:bOn, 打开/关闭相机光源
	/*输出:无
	/*返回:状态码 0, OK; other, Fail
	/*****************************************************/
	int __stdcall BottomCameraLightOnOff(bool bOff);

	/*****************************************************/
	/*函数名:TopCameraLightOnOff
	/*描述:控制BeckHoff PLC 开关顶部标定相机光源
	/*输入:bOn, 打开/关闭相机光源
	/*输出:无
	/*返回:状态码 0, OK; other, Fail
	/*****************************************************/
	int __stdcall TopCameraLightOnOff(bool bOff);

	/*****************************************************/
	/*函数名:CheckStatus
	/*描述:检查BeckHoff PLC链接状态
	/*输入:无
	/*输出:无
	/*返回:状态码 true, OK; false, Fail
	/*****************************************************/
	bool __stdcall CheckStatus();

public:
	int timeout = 10;
	
private:
	CTcpClient* _client = NULL;
	CmdHeader cmdHeader;
	RetCmdHeader retCmdHeader;

	int _port;
	int retPlcCode = 0;
	bool m_isReady = false;
	bool m_isError = false;
	bool m_isTimeout = false;
	bool m_isConnect = false;
	bool m_isConnectIO = false;
	DWORD t1, t2;
	
	int Send(CmdHeader data);
	bool IntervalTimeout();
	static void BeckHoffRecvDataCallBack(RetCmdHeader& callback, void* context);
	static void BeckHoffIOCallBack(PlcIO& callback, void* context);
	static void BeckHoffCloseCallBack(void* context);
	static void BeckHoffCloseIOCallBack(void* context);
	
};

#ifdef __cplusplus
}
#endif


#endif