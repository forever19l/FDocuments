// BeckHoff.cpp : 定义 DLL 应用程序的导出函数。
//
#include "stdafx.h"
#include "Beckhoff.h"

static CTcpClient* _IO = NULL;
static PlcIO plcIO;
static BOOL m_isConnectIO = FALSE;


BeckHoff::BeckHoff()
{
	_client = new CTcpClient();
	_IO = new CTcpClient();

	_client->SetRecvDataCallBack(BeckHoffRecvDataCallBack, this);
	_client->SetCloseClientCallBack(BeckHoffCloseCallBack, this);
	_IO->SetRecvIOCallBack(BeckHoffIOCallBack, this);
	//_IO->SetCloseClientCallBack(BeckHoffCloseIOCallBack, this);

	cmdHeader.m_Version1 = 1;
	cmdHeader.m_Version2 = 2;
	cmdHeader.m_TransId = 0;
	cmdHeader.m_CmdType = 0;
	cmdHeader.m_BlockId = 0;
	cmdHeader.m_GroupId = 0;
	cmdHeader.m_CMD_Cnt = 1;
}

BeckHoff::~BeckHoff()
{
	_client->Close();
	_IO->CloseIO();

	delete _client;
	delete _IO;
}

int BeckHoff::Connect(const char* szSvrIp, int port)
{
	_port = port;

	if (!m_isConnectIO)
	{
		if (_IO->Connect(szSvrIp, 4999) != 0) return -1;
		_IO->StartIO();
		m_isConnectIO = TRUE;
	}

	Sleep(500);
	int ret = _client->Connect(szSvrIp, port);
	if (ret == 0) m_isConnect = true;
	return 0;
}

extern "C" void BeckHoff::Close(bool bIO)
{
	if (bIO)
	{
		_IO->CloseIO();
		m_isConnectIO = FALSE;
	}
	_client->Close();
}

int BeckHoff::Send(CmdHeader data)
{
	t1 = GetTickCount();
	int len = sizeof(data);
	return _client->Send((char*)&data, len);
}

void BeckHoff::Start()
{
	_client->Start();
}

void BeckHoff::BeckHoffRecvDataCallBack(RetCmdHeader& callback, void* context)
{
	BeckHoff* ctrl = reinterpret_cast<BeckHoff*>(context);
	memset(&ctrl->retCmdHeader, 0, sizeof(ctrl->retCmdHeader));
	ctrl->retCmdHeader = callback;
	if (ctrl->retCmdHeader.m_CmdBody[0].m_Status_Code == 2)
	{
		ctrl->m_isReady = true;
	}
	else if(ctrl->retCmdHeader.m_CmdBody[0].m_Status_Code < 0)
	{
		ctrl->retPlcCode = ctrl->retCmdHeader.m_CmdBody[0].m_Status_Code;
		ctrl->m_isError = true;
	}
	
}

void BeckHoff::BeckHoffIOCallBack(PlcIO& callback, void* context)
{
	//BeckHoff* ctrl = reinterpret_cast<BeckHoff*>(context);
	plcIO = callback;
}

void BeckHoff::BeckHoffCloseCallBack(void* context)
{
	BeckHoff* ctrl = reinterpret_cast<BeckHoff*>(context);
	ctrl->m_isConnect = false;
}

void BeckHoff::BeckHoffCloseIOCallBack(void* context)
{
	BeckHoff* ctrl = reinterpret_cast<BeckHoff*>(context);
	ctrl->m_isConnectIO = false;
}

bool BeckHoff::CheckStatus()
{
	return m_isConnect && m_isConnectIO;
}

bool BeckHoff::IntervalTimeout()
{
	t2 = GetTickCount();
	int m_timeout = (t2 - t1)*1.0 / 1000;
	if (m_timeout > timeout)
	{
		m_isTimeout = true;
		return true;
	}
	return false;
}

int BeckHoff::gvm_exec(int funcid, int devcid, double value1, double value2)
{
	gvm_add_data(0, funcid, devcid, value1, value2);
	return gvm_send_data(1);
}

int BeckHoff::gvm_exec_two(int f1, int d1, double v1, double v2, int f2, int d2, double v3, double v4)
{
	gvm_add_data(0, f1, d1, v1, v2);
	gvm_add_data(1, f2, d2, v3, v4);
	return gvm_send_data(2);
}

int BeckHoff::gvm_exec_th(int f1, int d1, double v1, double v2, int f2, int d2, double v3, double v4, int f3, int d3, double v5, double v6)
{
	gvm_add_data(0, f1, d1, v1, v2);
	gvm_add_data(1, f2, d2, v3, v4);
	gvm_add_data(2, f3, d3, v5, v6);
	return gvm_send_data(3);
}

void  BeckHoff::gvm_add_data(int index, int funcid, int devicid, double v1, double v2){
	cmdHeader.m_CmdBody[index].m_DeviceId = devicid;
	cmdHeader.m_CmdBody[index].m_Function_Code = funcid;
	cmdHeader.m_CmdBody[index].m_Opvalue1 = v1;
	cmdHeader.m_CmdBody[index].m_Opvalue2 = v2;
}

void  BeckHoff::gvm_clear_data(){
	for (size_t i = 0; i < 10; i++)
	{
		cmdHeader.m_CmdBody[i].m_DeviceId = 0;
		cmdHeader.m_CmdBody[i].m_Function_Code = 0;
		cmdHeader.m_CmdBody[i].m_Opvalue1 = 0;
		cmdHeader.m_CmdBody[i].m_Opvalue2 = 0;
	}
	cmdHeader.m_CMD_Cnt = 0;
}

int BeckHoff::gvm_send_data(int count){
	cmdHeader.m_CMD_Cnt = count;
	if (Send(cmdHeader) < 1) return -1;
	while (!m_isReady && !m_isError && !IntervalTimeout())
	{
		Sleep(10);
	}

	if (m_isError)
	{
		m_isError = false;
		return retPlcCode;
	}
	if (m_isTimeout == true) return -1;
	m_isReady = false;
	gvm_clear_data();
	return 0;
}

int BeckHoff::GetCurrentXYZU(double* cx, double* cy, double* cz, double* cu)
{
	Sleep(100);
	if (_port == 5000)
	{
		*cx = plcIO.Lreal[20];
		*cy = plcIO.Lreal[21];
		*cz = plcIO.Lreal[22];
		*cu = plcIO.Lreal[23];
	}
	else if (_port == 5001)
	{
		*cx = plcIO.Lreal[25];
		*cy = plcIO.Lreal[26];
		*cz = plcIO.Lreal[27];
		*cu = plcIO.Lreal[28];
	}	
	return 0;
}

int BeckHoff::AbsMoveX(double cx, double vel)
{
	cmdHeader.m_CmdBody[0].m_Function_Code = 2;
	cmdHeader.m_CmdBody[0].m_Opvalue1 = cx;
	cmdHeader.m_CmdBody[0].m_Opvalue2 = vel;

	if (_port == 5000)
		cmdHeader.m_CmdBody[0].m_DeviceId = 0;
	else if (_port == 5001)
		cmdHeader.m_CmdBody[0].m_DeviceId = 3;

	if(Send(cmdHeader) < 1) return -1;
	while (!m_isReady && !m_isError && !IntervalTimeout())
	{
		Sleep(10);
	}

	if (m_isError)
	{
		m_isError = false;
		return retPlcCode;
	}
	if (m_isTimeout == true) return -1;
	m_isReady = false;
	return 0;
}

int BeckHoff::AbsMoveY(double cy, double vel)
{
	cmdHeader.m_CmdBody[0].m_Function_Code = 2;
	cmdHeader.m_CmdBody[0].m_Opvalue1 = cy;
	cmdHeader.m_CmdBody[0].m_Opvalue2 = vel;

	if (_port == 5000)
		cmdHeader.m_CmdBody[0].m_DeviceId = 1;
	else if (_port == 5001)
		cmdHeader.m_CmdBody[0].m_DeviceId = 4;

	if (Send(cmdHeader) < 1) return -1;
	while (!m_isReady && !m_isError && !IntervalTimeout())
	{
		Sleep(10);
	}
	if (m_isError)
	{
		m_isError = false;
		return retPlcCode;
	}
	if (m_isTimeout == true) return -1;
	m_isReady = false;
	return 0;
}

int BeckHoff::AbsMoveZ(double cz, double vel)
{
	cmdHeader.m_CmdBody[0].m_Function_Code = 2;
	cmdHeader.m_CmdBody[0].m_Opvalue1 = cz;
	cmdHeader.m_CmdBody[0].m_Opvalue2 = vel;

	if (_port == 5000)
		cmdHeader.m_CmdBody[0].m_DeviceId = 2;
	else if (_port == 5001)
		cmdHeader.m_CmdBody[0].m_DeviceId = 5;

	if (Send(cmdHeader) < 1) return -1;
	while (!m_isReady && !m_isError && !IntervalTimeout())
	{
		Sleep(10);
	}
	if (m_isError)
	{
		m_isError = false;
		return retPlcCode;
	}
	if (m_isTimeout == true) return -1;
	m_isReady = false;
	return 0;
}

int BeckHoff::AbsMoveTheta(double cu, double vel)
{
	cmdHeader.m_CmdBody[0].m_Function_Code = 2;
	cmdHeader.m_CmdBody[0].m_Opvalue1 = cu;
	cmdHeader.m_CmdBody[0].m_Opvalue2 = vel;

	if (_port == 5000)
		cmdHeader.m_CmdBody[0].m_DeviceId = 6;
	else if (_port == 5001)
		cmdHeader.m_CmdBody[0].m_DeviceId = 7;
		
	if (Send(cmdHeader) < 1) return -1;
	while (!m_isReady && !m_isError && !IntervalTimeout())
	{
		Sleep(10);
	}
	if (m_isError)
	{
		m_isError = false;
		return retPlcCode;
	}
	if (m_isTimeout == true) return -1;
	m_isReady = false;
	return 0;
}

int BeckHoff::RelMoveX(double cx, double vel)
{
	cmdHeader.m_CmdBody[0].m_Function_Code = 3;
	cmdHeader.m_CmdBody[0].m_Opvalue1 = cx;
	cmdHeader.m_CmdBody[0].m_Opvalue2 = vel;

	if (_port == 5000)
		cmdHeader.m_CmdBody[0].m_DeviceId = 0;
	else if (_port == 5001)
		cmdHeader.m_CmdBody[0].m_DeviceId = 3;

	if (Send(cmdHeader) < 1) return -1;
	while (!m_isReady && !m_isError && !IntervalTimeout())
	{
		Sleep(10);
	}
	if (m_isError)
	{
		m_isError = false;
		return retPlcCode;
	}
	if (m_isTimeout == true) return -1;
	m_isReady = false;
	return 0;
}

int BeckHoff::RelMoveY(double cy, double vel)
{
	cmdHeader.m_CmdBody[0].m_Function_Code = 3;
	cmdHeader.m_CmdBody[0].m_Opvalue1 = cy;
	cmdHeader.m_CmdBody[0].m_Opvalue2 = vel;

	if (_port == 5000)
		cmdHeader.m_CmdBody[0].m_DeviceId = 1;
	else if (_port == 5001)
		cmdHeader.m_CmdBody[0].m_DeviceId = 4;

	if (Send(cmdHeader) < 1) return -1;
	while (!m_isReady && !m_isError && !IntervalTimeout())
	{
		Sleep(10);
	}
	if (m_isError)
	{
		m_isError = false;
		return retPlcCode;
	}
	if (m_isTimeout == true) return -1;
	m_isReady = false;
	return 0;
}

int BeckHoff::RelMoveZ(double cz, double vel)
{
	cmdHeader.m_CmdBody[0].m_Function_Code = 3;
	cmdHeader.m_CmdBody[0].m_Opvalue1 = cz;
	cmdHeader.m_CmdBody[0].m_Opvalue2 = vel;

	if (_port == 5000)
		cmdHeader.m_CmdBody[0].m_DeviceId = 2;
	else if (_port == 5001)
		cmdHeader.m_CmdBody[0].m_DeviceId = 5;

	if (Send(cmdHeader) < 1) return -1;
	while (!m_isReady && !m_isError && !IntervalTimeout())
	{
		Sleep(10);
	}
	if (m_isError)
	{
		m_isError = false;
		return retPlcCode;
	}
	if (m_isTimeout == true) return -1;
	m_isReady = false;
	return 0;
}

int BeckHoff::RelMoveTheta(double cu, double vel)
{
	cmdHeader.m_CmdBody[0].m_Function_Code = 3;
	cmdHeader.m_CmdBody[0].m_Opvalue1 = cu;
	cmdHeader.m_CmdBody[0].m_Opvalue2 = vel;

	if (_port == 5000)
		cmdHeader.m_CmdBody[0].m_DeviceId = 6;
	else if (_port == 5001)
		cmdHeader.m_CmdBody[0].m_DeviceId = 7;

	if (Send(cmdHeader) < 1) return -1;
	while (!m_isReady && !m_isError && !IntervalTimeout())
	{
		Sleep(10);
	}
	if (m_isError)
	{
		m_isError = false;
		return retPlcCode;
	}
	if (m_isTimeout == true) return -1;
	m_isReady = false;
	return 0;
}

int BeckHoff::NozzleUpDn(int nozzle, bool bDN)
{
	cmdHeader.m_CmdBody[0].m_Function_Code = 0;
	cmdHeader.m_CmdBody[0].m_Opvalue2 = 0;

	if (bDN)
		cmdHeader.m_CmdBody[0].m_Opvalue1 = 1;
	else
		cmdHeader.m_CmdBody[0].m_Opvalue1 = 0;

	if (_port == 5000)
	{
		switch (nozzle)
		{
		case 1:
			cmdHeader.m_CmdBody[0].m_DeviceId = 48; break;
		case 2:
			cmdHeader.m_CmdBody[0].m_DeviceId = 49; break;
		case 3:
			cmdHeader.m_CmdBody[0].m_DeviceId = 50; break;
		case 4:
			cmdHeader.m_CmdBody[0].m_DeviceId = 51; break;
		}
	}
	else if (_port == 5001)
	{
		switch (nozzle)
		{
		case 1:
			cmdHeader.m_CmdBody[0].m_DeviceId = 52; break;
		case 2:
			cmdHeader.m_CmdBody[0].m_DeviceId = 53; break;
		case 3:
			cmdHeader.m_CmdBody[0].m_DeviceId = 54; break;
		case 4:
			cmdHeader.m_CmdBody[0].m_DeviceId = 55; break;
		}
	}
	if (Send(cmdHeader) < 1) return -1;
	while (!m_isReady && !m_isError && !IntervalTimeout())
	{
		Sleep(10);
	}
	if (m_isError)
	{
		m_isError = false;
		return retPlcCode;
	}
	if (m_isTimeout == true) return -1;
	m_isReady = false;
	return 0;
}

int BeckHoff::NozzleSuckOrNot(int nozzle, bool bSuck)
{
	cmdHeader.m_CmdBody[0].m_Function_Code = 0;
	cmdHeader.m_CmdBody[0].m_Opvalue2 = 0;

	if (bSuck)
		cmdHeader.m_CmdBody[0].m_Opvalue1 = 1;
	else
		cmdHeader.m_CmdBody[0].m_Opvalue1 = 0;

	if (_port == 5000)
	{
		switch (nozzle)
		{
		case 1:
			cmdHeader.m_CmdBody[0].m_DeviceId = 56; break;
		case 2:
			cmdHeader.m_CmdBody[0].m_DeviceId = 57; break;
		case 3:
			cmdHeader.m_CmdBody[0].m_DeviceId = 58; break;
		case 4:
			cmdHeader.m_CmdBody[0].m_DeviceId = 59; break;
		}
	}
	else if (_port == 5001)
	{
		switch (nozzle)
		{
		case 1:
			cmdHeader.m_CmdBody[0].m_DeviceId = 60; break;
		case 2:
			cmdHeader.m_CmdBody[0].m_DeviceId = 61; break;
		case 3:
			cmdHeader.m_CmdBody[0].m_DeviceId = 62; break;
		case 4:
			cmdHeader.m_CmdBody[0].m_DeviceId = 63; break;
		}
	}
	if (Send(cmdHeader) < 1) return -1;
	while (!m_isReady && !m_isError && !IntervalTimeout())
	{
		Sleep(10);
	}
	if (m_isError)
	{
		m_isError = false;
		return retPlcCode;
	}
	if (m_isTimeout == true) return -1;
	m_isReady = false;
	return 0;
}

int BeckHoff::BottomCameraLightOnOff(bool bOn)
{

	return 0;
}

int BeckHoff::TopCameraLightOnOff(bool bOn)
{
	cmdHeader.m_CmdBody[0].m_Function_Code = 0;
	cmdHeader.m_CmdBody[0].m_Opvalue2 = 0;

	if (bOn)
		cmdHeader.m_CmdBody[0].m_Opvalue1 = 1;
	else
		cmdHeader.m_CmdBody[0].m_Opvalue1 = 0;

	if (_port == 5000)
		cmdHeader.m_CmdBody[0].m_DeviceId = 64; 
	else if (_port == 5001)
		cmdHeader.m_CmdBody[0].m_DeviceId = 65; 

	if (Send(cmdHeader) < 1) return -1;
	while (!m_isReady && !m_isError && !IntervalTimeout())
	{
		Sleep(10);
	}
	if (m_isError)
	{
		m_isError = false;
		return retPlcCode;
	}
	if (m_isTimeout == true) return -1;
	m_isReady = false;
	return 0;
}

