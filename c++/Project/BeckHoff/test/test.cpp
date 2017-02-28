// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
//#include <windows.h>
#include "Beckhoff.h"

#pragma comment(lib,"Beckhoff.lib")
int _tmain(int argc, _TCHAR* argv[])
{
	BeckHoff robot1;// = new BeckHoff();
	//BeckHoff robot2 = new BeckHoff();
	//const char* ip = "169.254.1.177";
	const char* ip = "172.15.1.234";
	int port = 5000;
	robot1.Connect(ip, port);
	robot1.Start();

	//robot2->Connect(ip, 5001);
	//robot2->Start();


	//robot1->Close(true);

	/*robot1->AbsMoveX(1, 10);
	robot1->AbsMoveY(2, 9);
	robot1->AbsMoveZ(3, 8);
	robot1->AbsMoveTheta(4, 7);*/

	/*double cx;
	double cy;
	double cz;
	double cu;
	robot1->GetCurrentXYZU(&cx, &cy, &cz, &cu);*/


	//robot1->NozzleUpDn(1, true);
	//robot1->NozzleUpDn(1, false);

	//robot1->NozzleSuckOrNot(1, true);
	//robot1->NozzleSuckOrNot(1, false);

	system("pause");
	//delete robot1;
	return 0;
}

