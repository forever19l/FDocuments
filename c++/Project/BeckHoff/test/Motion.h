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
	/*������:Connect
	/*����:����BeckHoff PLC
	/*����:szSvrIp��IP��ַ�� port���˿ں�
	/*���:��
	/*����:״̬�� 0, OK; other, Fail
	/*****************************************************/
	int Connect(const char* szSvrIp, int port);

	/*****************************************************/
	/*������:Close
	/*����:�ر�����BeckHoff PLC
	/*����:��
	/*���:��
	/*����:��
	/*****************************************************/
	void Close();

	/*****************************************************/
	/*������:Start
	/*����:��������BeckHoff PLC
	/*����:��
	/*���:��
	/*����:��
	/*****************************************************/
	void Start();

	/*****************************************************/
	/*������:GetCurrentXYZU
	/*����:��ȡBeckHoff PLC��ǰ�����
	/*����:��
	/*���:cx, cy, cz, cu����ֵ
	/*����:״̬�� 0, OK; other, Fail
	/*****************************************************/
	int GetCurrentXYZU(double* cx, double* cy, double* cz, double* cu);

	/*****************************************************/
	/*������:AbsMoveX
	/*����:�ƶ�BeckHoff PLC X�ᵽ��ǰĿ���
	/*����:cx, X���ƶ���������ֵ��vel, X���ƶ��ٶ�
	/*���:��
	/*����:״̬�� 0, OK; other, Fail
	/*****************************************************/
	int AbsMoveX(double cx, double vel);

	/*****************************************************/
	/*������:AbsMoveY
	/*����:�ƶ�BeckHoff PLC Y�ᵽ��ǰĿ���
	/*����:cy, Y���ƶ���������ֵ��vel, Y���ƶ��ٶ�
	/*���:��
	/*����:״̬�� 0, OK; other, Fail
	/*****************************************************/
	int AbsMoveY(double cy, double vel);

	/*****************************************************/
	/*������:AbsMoveZ
	/*����:�ƶ�BeckHoff PLC Z�ᵽ��ǰĿ���
	/*����:cz, Z���ƶ���������ֵ��vel, Z���ƶ��ٶ�
	/*���:��
	/*����:״̬�� 0, OK; other, Fail
	/*****************************************************/
	int  AbsMoveZ(double cz, double vel);

	/*****************************************************/
	/*������:AbsMoveTheta
	/*����:�ƶ�BeckHoff PLC U�ᵽ��ǰĿ���
	/*����:cu, U���ƶ���������ֵ��vel, U����ת�ٶ�
	/*���:��
	/*����:״̬�� 0, OK; other, Fail
	/*****************************************************/
	int  AbsMoveTheta(double cu, double vel);

	/*****************************************************/
	/*������:RelMoveX
	/*����:�ƶ�BeckHoff PLC X�ᵱǰ����
	/*����:cx, X���ƶ���Ծ��룻vel, X���ƶ��ٶ�
	/*���:��
	/*����:״̬�� 0, OK; other, Fail
	/*****************************************************/
	int  RelMoveX(double cx, double vel);

	/*****************************************************/
	/*������:RelMoveY
	/*����:�ƶ�BeckHoff PLC Y�ᵱǰ����
	/*����:cy, Y���ƶ���Ծ��룻vel, Y���ƶ��ٶ�
	/*���:��
	/*����:״̬�� 0, OK; other, Fail
	/*****************************************************/
	int  RelMoveY(double cy, double vel);

	/*****************************************************/
	/*������:RelMoveZ
	/*����:�ƶ�BeckHoff PLC Z�ᵱǰ����
	/*����:cz, Z���ƶ���Ծ��룻vel, Z���ƶ��ٶ�
	/*���:��
	/*����:״̬�� 0, OK; other, Fail
	/*****************************************************/
	int  RelMoveZ(double cz, double vel);

	/*****************************************************/
	/*������:RelMoveTheta
	/*����:�ƶ�BeckHoff PLC U�ᵱǰ�ĽǶ�
	/*����:cu, U���ƶ���ԽǶȣ�vel, U����ת�ٶ�
	/*���:��
	/*����:״̬�� 0, OK; other, Fail
	/*****************************************************/
	int  RelMoveTheta(double cu, double vel);

	/*****************************************************/
	/*������:MoveNozzleUp
	/*����:�ƶ�BeckHoff PLC ��������
	/*����:nozzle, �����ţ�1~4����bUp�� ���¶���
	/*���:��
	/*����:״̬�� 0, OK; other, Fail
	/*****************************************************/
	int  NozzleUpDn(int nozzle, bool bUp);

	/*****************************************************/
	/*������:SuckNozzle
	/*����:BeckHoff PLC ���쿪�����
	/*����:nozzle, �����ţ�1~4����bSuck�� ��/�ر����
	/*���:��
	/*����:״̬�� 0, OK; other, Fail
	/*****************************************************/
	int  NozzleSuckOrNot(int nozzle, bool bSuck);

	/*****************************************************/
	/*������:BottomCameraLightOnOff
	/*����:����BeckHoff PLC ���صײ��궨�����Դ
	/*����:bOn, ��/�ر������Դ
	/*���:��
	/*����:״̬�� 0, OK; other, Fail
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