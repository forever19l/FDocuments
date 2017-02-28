#ifndef _MOTION_H_
#define _MOTION_H_

#include "CTcpClient.h"

class Robot
{
public:
	Robot();
	~Robot();

	/*int Connect(const char* szSvrIp, int port);
	void Start();
	void Close();
	int Send(const char* data, size_t len);*/

	void SetRecvDataCallBack(RecvDataCallBack callback, void* context);
	//void SetCloseClientCallBack(CloseClientCallBack callback, void* context); 
	

	/*****************************************************/
	/*函数名:Connect
	/*描述:连接BeckHoff PLC
	/*输入:szSvrIp，IP地址； port，端口号
	/*输出:无
	/*返回:状态码 0, OK; other, Fail
	/*****************************************************/
	int Connect(const char* szSvrIp, int port);

	/*****************************************************/
	/*函数名:Close
	/*描述:关闭连接BeckHoff PLC
	/*输入:无
	/*输出:无
	/*返回:无
	/*****************************************************/
	void Close();

	/*****************************************************/
	/*函数名:Start
	/*描述:启动监听BeckHoff PLC
	/*输入:无
	/*输出:无
	/*返回:无
	/*****************************************************/
	void Start();

	/*****************************************************/
	/*函数名:GetCurrentXYZU
	/*描述:获取BeckHoff PLC当前坐标点
	/*输入:无
	/*输出:cx, cy, cz, cu坐标值
	/*返回:状态码 0, OK; other, Fail
	/*****************************************************/
	int GetCurrentXYZU(double* cx, double* cy, double* cz, double* cu);

	/*****************************************************/
	/*函数名:AbsMoveX
	/*描述:移动BeckHoff PLC X轴到当前目标点
	/*输入:cx, X轴移动绝对坐标值；vel, X轴移动速度
	/*输出:无
	/*返回:状态码 0, OK; other, Fail
	/*****************************************************/
	int AbsMoveX(double cx, double vel);

	/*****************************************************/
	/*函数名:AbsMoveY
	/*描述:移动BeckHoff PLC Y轴到当前目标点
	/*输入:cy, Y轴移动绝对坐标值；vel, Y轴移动速度
	/*输出:无
	/*返回:状态码 0, OK; other, Fail
	/*****************************************************/
	int AbsMoveY(double cy, double vel);

	/*****************************************************/
	/*函数名:AbsMoveZ
	/*描述:移动BeckHoff PLC Z轴到当前目标点
	/*输入:cz, Z轴移动绝对坐标值；vel, Z轴移动速度
	/*输出:无
	/*返回:状态码 0, OK; other, Fail
	/*****************************************************/
	int  AbsMoveZ(double cz, double vel);

	/*****************************************************/
	/*函数名:AbsMoveTheta
	/*描述:移动BeckHoff PLC U轴到当前目标点
	/*输入:cu, U轴移动绝对坐标值；vel, U轴旋转速度
	/*输出:无
	/*返回:状态码 0, OK; other, Fail
	/*****************************************************/
	int  AbsMoveTheta(double cu, double vel);

	/*****************************************************/
	/*函数名:RelMoveX
	/*描述:移动BeckHoff PLC X轴当前距离
	/*输入:cx, X轴移动相对距离；vel, X轴移动速度
	/*输出:无
	/*返回:状态码 0, OK; other, Fail
	/*****************************************************/
	int  RelMoveX(double cx, double vel);

	/*****************************************************/
	/*函数名:RelMoveY
	/*描述:移动BeckHoff PLC Y轴当前距离
	/*输入:cy, Y轴移动相对距离；vel, Y轴移动速度
	/*输出:无
	/*返回:状态码 0, OK; other, Fail
	/*****************************************************/
	int  RelMoveY(double cy, double vel);

	/*****************************************************/
	/*函数名:RelMoveZ
	/*描述:移动BeckHoff PLC Z轴当前距离
	/*输入:cz, Z轴移动相对距离；vel, Z轴移动速度
	/*输出:无
	/*返回:状态码 0, OK; other, Fail
	/*****************************************************/
	int  RelMoveZ(double cz, double vel);

	/*****************************************************/
	/*函数名:RelMoveTheta
	/*描述:移动BeckHoff PLC U轴当前的角度
	/*输入:cu, U轴移动相对角度；vel, U轴旋转速度
	/*输出:无
	/*返回:状态码 0, OK; other, Fail
	/*****************************************************/
	int  RelMoveTheta(double cu, double vel);

	/*****************************************************/
	/*函数名:MoveNozzleUp
	/*描述:移动BeckHoff PLC 吸嘴上下
	/*输入:nozzle, 吸嘴编号（1~4）；bUp， 上下动作
	/*输出:无
	/*返回:状态码 0, OK; other, Fail
	/*****************************************************/
	int  NozzleUpDn(int nozzle, bool bUp);

	/*****************************************************/
	/*函数名:SuckNozzle
	/*描述:BeckHoff PLC 吸嘴开关真空
	/*输入:nozzle, 吸嘴编号（1~4）；bSuck， 打开/关闭真空
	/*输出:无
	/*返回:状态码 0, OK; other, Fail
	/*****************************************************/
	int  NozzleSuckOrNot(int nozzle, bool bSuck);

	/*****************************************************/
	/*函数名:BottomCameraLightOnOff
	/*描述:控制BeckHoff PLC 开关底部标定相机光源
	/*输入:bOn, 打开/关闭相机光源
	/*输出:无
	/*返回:状态码 0, OK; other, Fail
	/*****************************************************/
	int  BottomCameraLightOnOff(bool bOn);

public:
	CTcpClient* _client = NULL;
	static void MotionRecvDataCallBack(RetCmdHeader& callback, void* context);
	void SetPlcIO(PlcIO& callback);

private:
	CmdHeader cmdHeader;
	RetCmdHeader retCmdHeader;
	PlcIO io;

	int _port;
	int retPlcCode = 0;
	bool m_isReady = false;
	bool m_isError = false;

	int Send(CmdHeader data);
	
};


#endif