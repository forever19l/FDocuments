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
	/*������:Connect
	/*����:����BeckHoff PLC
	/*����:szSvrIp��IP��ַ�� port���˿ں�
	/*���:��
	/*����:״̬�� 0, OK; other, Fail
	/*****************************************************/
	int  Connect(const char* szSvrIp, int port);

	/*****************************************************/
	/*������:Close
	/*����:�ر�����BeckHoff PLC
	/*����:��
	/*���:��
	/*����:��
	/*****************************************************/
	void  Close(bool bIO);

	/*****************************************************/
	/*������:Start
	/*����:��������BeckHoff PLC
	/*����:��
	/*���:��
	/*����:��
	/*****************************************************/
	void  Start();

	/*****************************************************/
	/*������:gvm_exec
	/*����:���͵���cmd
	/*����:Ĭ��functionid, deviceid, value1, value2
	/*���:��
	/*����:״̬�� true, OK; false, Fail
	/*****************************************************/
	int gvm_exec(int funcid, int devcid, double value1, double value2);

	/*****************************************************/
	/*������:GetCurrentXYZU
	/*����:��ȡBeckHoff PLC��ǰ�����
	/*����:��
	/*���:cx, cy, cz, cu����ֵ
	/*����:״̬�� 0, OK; other, Fail
	/*****************************************************/
	int __stdcall GetCurrentXYZU(double* cx, double* cy, double* cz, double* cu);

	/*****************************************************/
	/*������:AbsMoveX
	/*����:�ƶ�BeckHoff PLC X�ᵽ��ǰĿ���
	/*����:cx, X���ƶ���������ֵ��vel, X���ƶ��ٶ�
	/*���:��
	/*����:״̬�� 0, OK; other, Fail
	/*****************************************************/
	int __stdcall AbsMoveX(double cx, double vel);

	/*****************************************************/
	/*������:AbsMoveY
	/*����:�ƶ�BeckHoff PLC Y�ᵽ��ǰĿ���
	/*����:cy, Y���ƶ���������ֵ��vel, Y���ƶ��ٶ�
	/*���:��
	/*����:״̬�� 0, OK; other, Fail
	/*****************************************************/
	int __stdcall AbsMoveY(double cy, double vel);

	/*****************************************************/
	/*������:AbsMoveZ
	/*����:�ƶ�BeckHoff PLC Z�ᵽ��ǰĿ���
	/*����:cz, Z���ƶ���������ֵ��vel, Z���ƶ��ٶ�
	/*���:��
	/*����:״̬�� 0, OK; other, Fail
	/*****************************************************/
	int __stdcall AbsMoveZ(double cz, double vel);

	/*****************************************************/
	/*������:AbsMoveTheta
	/*����:�ƶ�BeckHoff PLC U�ᵽ��ǰĿ���
	/*����:cu, U���ƶ���������ֵ��vel, U����ת�ٶ�
	/*���:��
	/*����:״̬�� 0, OK; other, Fail
	/*****************************************************/
	int __stdcall AbsMoveTheta(double cu, double vel);

	/*****************************************************/
	/*������:RelMoveX
	/*����:�ƶ�BeckHoff PLC X�ᵱǰ����
	/*����:cx, X���ƶ���Ծ��룻vel, X���ƶ��ٶ�
	/*���:��
	/*����:״̬�� 0, OK; other, Fail
	/*****************************************************/
	int __stdcall RelMoveX(double cx, double vel);

	/*****************************************************/
	/*������:RelMoveY
	/*����:�ƶ�BeckHoff PLC Y�ᵱǰ����
	/*����:cy, Y���ƶ���Ծ��룻vel, Y���ƶ��ٶ�
	/*���:��
	/*����:״̬�� 0, OK; other, Fail
	/*****************************************************/
	int __stdcall RelMoveY(double cy, double vel);

	/*****************************************************/
	/*������:RelMoveZ
	/*����:�ƶ�BeckHoff PLC Z�ᵱǰ����
	/*����:cz, Z���ƶ���Ծ��룻vel, Z���ƶ��ٶ�
	/*���:��
	/*����:״̬�� 0, OK; other, Fail
	/*****************************************************/
	int __stdcall RelMoveZ(double cz, double vel);

	/*****************************************************/
	/*������:RelMoveTheta
	/*����:�ƶ�BeckHoff PLC U�ᵱǰ�ĽǶ�
	/*����:cu, U���ƶ���ԽǶȣ�vel, U����ת�ٶ�
	/*���:��
	/*����:״̬�� 0, OK; other, Fail
	/*****************************************************/
	int __stdcall RelMoveTheta(double cu, double vel);

	/*****************************************************/
	/*������:MoveNozzleUp
	/*����:�ƶ�BeckHoff PLC ��������
	/*����:nozzle, �����ţ�1~4����bUp�� ���¶���
	/*���:��
	/*����:״̬�� 0, OK; other, Fail
	/*****************************************************/
	int __stdcall NozzleUpDn(int nozzle, bool bDN);

	/*****************************************************/
	/*������:SuckNozzle
	/*����:BeckHoff PLC ���쿪�����
	/*����:nozzle, �����ţ�1~4����bSuck�� ��/�ر����
	/*���:��
	/*����:״̬�� 0, OK; other, Fail
	/*****************************************************/
	int __stdcall NozzleSuckOrNot(int nozzle, bool bSuck);

	/*****************************************************/
	/*������:BottomCameraLightOnOff
	/*����:����BeckHoff PLC ���صײ��궨�����Դ
	/*����:bOn, ��/�ر������Դ
	/*���:��
	/*����:״̬�� 0, OK; other, Fail
	/*****************************************************/
	int __stdcall BottomCameraLightOnOff(bool bOff);

	/*****************************************************/
	/*������:TopCameraLightOnOff
	/*����:����BeckHoff PLC ���ض����궨�����Դ
	/*����:bOn, ��/�ر������Դ
	/*���:��
	/*����:״̬�� 0, OK; other, Fail
	/*****************************************************/
	int __stdcall TopCameraLightOnOff(bool bOff);

	/*****************************************************/
	/*������:CheckStatus
	/*����:���BeckHoff PLC����״̬
	/*����:��
	/*���:��
	/*����:״̬�� true, OK; false, Fail
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