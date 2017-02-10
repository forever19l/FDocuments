#include <stdio.h>
#include <tchar.h>
#include <direct.h>
//#include <objbase.h>
#include "ZmqReplyer.h"
#include "ZmqRequester.h"
#include "CRedis.h"
#include "LogHelper.h"
#include "XmlHelper.h"
#include "JsonHelper.h"
#include "rapidjson\document.h"

//for gethostname
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")  
#include <IPHlpApi.h>
#pragma comment(lib,"Iphlpapi.lib")

using namespace std;
using namespace log4cplus;

void * g_context = NULL;
ZmqReplyer * REP = new ZmqReplyer(g_context);
ZmqRequester * REQ = new ZmqRequester();

/*global parameter*/
string g_station;
string g_csv_pre;
string g_csv_post;
string g_mtms_pre;
string g_mtms_post;
string g_model;

string g_address_req;
int g_port_req;
string g_address_rep;
int g_port_rep;
string g_mtms_add;
int g_mtms_port;

void Destory()
{
	if (REP)
	{
		delete REP;
	}
	if (REQ)
	{
		delete REQ;
	}
}

BOOL WINAPI ConsoleHandler(DWORD msgType)
{
	if (msgType == CTRL_C_EVENT)
	{
		printf("Ctrl-C!\n");
		Destory();
		return TRUE;
	}
	else if (msgType == CTRL_CLOSE_EVENT)
	{
		printf("Close console window!\n");
		Destory();
		/* Note: The system gives you very limited time to exit in this condition */
		return TRUE;
	}

	/*
	Other messages:
	CTRL_BREAK_EVENT         Ctrl-Break pressed
	CTRL_LOGOFF_EVENT        User log off
	CTRL_SHUTDOWN_EVENT      System shutdown
	*/

	return FALSE;
}

void TMCLOG(string log){
	SYSTEMTIME st;
	GetLocalTime(&st);
	string time = to_string(st.wYear) + to_string(st.wMonth) + to_string(st.wDay) + "-" + to_string(st.wHour) + ":" + to_string(st.wMinute) + ":" + to_string(st.wSecond) ;
	LOG_DEBUG(log.c_str());
	printf_s("time->%s ;log->%s\n", time.c_str(), log.c_str());
}

std::string CreateLoad(string csv)
{
	rapidjson::Document document;
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	rapidjson::Value root(rapidjson::kObjectType);
	char uuid_buffer[64] = { 0 };
	GUID guid;

	if (CoCreateGuid(&guid))
	{
		fprintf(stderr, "create guid error\n");
	}
	_snprintf_s(uuid_buffer, sizeof(uuid_buffer),
		"{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2],
		guid.Data4[3], guid.Data4[4], guid.Data4[5],
		guid.Data4[6], guid.Data4[7]);

	root.AddMember("function", "load", allocator);
	root.AddMember("id", rapidjson::Value(uuid_buffer, allocator), allocator);
	root.AddMember("jsonrpc", "1.0", allocator);

	rapidjson::Value msgParam(rapidjson::kArrayType);
	msgParam.PushBack(rapidjson::StringRef(csv.c_str()) , allocator);
	root.AddMember("params", msgParam, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	root.Accept(writer);
	std::string resultStr = buffer.GetString();
	return resultStr;
}

bool ParseLoad(std::string data)
{
	rapidjson::Document doc;
	doc.Parse<0>(data.c_str());
	if (!doc.IsObject()){
		return false;
	}
	std::string funName;
	if (doc.HasMember("function")){
		funName = doc["function"].GetString();
	}

	std::string param;
	if (doc.HasMember("params") && doc["params"].IsArray()){
		rapidjson::Value array = doc["params"].GetArray();
		int size = array.Size();
		for (int i = 0; i < size; ++i){
			param = array[i].GetString();
		}
	}

	return true;
}

LPCWSTR stringToLPCWSTR(std::string orig)
{
	size_t origsize = orig.length() + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t *wcstring = (wchar_t *)malloc(sizeof(wchar_t)*(orig.length() - 1));
	mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);
	return wcstring;
}
string string_replace(string str, const string old, const string sub)
{
	string::size_type pos = 0;
	string::size_type a = old.size();
	string::size_type b = sub.size();
	while ((pos = str.find(old, pos)) != string::npos)
	{
		str.replace(pos, a, sub);
		pos += b;
	}
	return str;
}

bool launce_MTMS(string k){						//launce exe  
	/*	//launce MTMS in TMC 
	char path[MAX_PATH];
	_getcwd(path, MAX_PATH);
	string c = path;
	string cd = "cd /d " + c;
	system(cd.c_str());
	system(k.c_str());
	*/
	SHELLEXECUTEINFO shell = { sizeof(shell) };
	shell.fMask = SEE_MASK_FLAG_DDEWAIT;
	shell.lpVerb = L"open";

	char p[MAX_PATH];
	_getcwd(p, MAX_PATH);
	string p1 = p ;
	string path = p1 + +"\\MTMS.exe";
	LPCWSTR lpfile;
	shell.lpFile = stringToLPCWSTR(path);
	LPCUWSTR lpparameter;
	shell.lpParameters = stringToLPCWSTR(k);
	//shell.lpParameters = L"169.154.1.111 61086";
	shell.nShow = SW_HIDE;
	BOOL ret = ShellExecuteEx(&shell);
	if (ret)
	{
		TMCLOG("launce exe success -->>" + k);
		return true;
	}
	else{	
		TMCLOG("launce exe failed -->>" + k);
		return false;
	}
}

bool kill_MTMS(string k){
	//system("start taskkill /F /IM c.exe");	//会有个一闪而过的弹框
	Util::KillProcessByName("MTMS");
	return true;
}
bool GetAddNetAddress(std::vector<std::string>& address)
{
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	if (ERROR_BUFFER_OVERFLOW == nRel)
	{
		delete pIpAdapterInfo;
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
		nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	}
	if (ERROR_SUCCESS == nRel)
	{
		while (pIpAdapterInfo)
		{
			IP_ADDR_STRING *pIpAddrString = &(pIpAdapterInfo->IpAddressList);
			do
			{
				std::string addr = pIpAddrString->IpAddress.String;
				if (addr != "0.0.0.0"){
					address.push_back(addr);
				}
				pIpAddrString = pIpAddrString->Next;
			} while (pIpAddrString);
			pIpAdapterInfo = pIpAdapterInfo->Next;
		}
	}
	else{
		LOG_ERROR("Get Ip address info failed.");
		printf("Get Ip address info failed.\n");
	}
	return true;
}
void init_rep(){
	//ZmqReplyer * rep = new ZmqReplyer(g_context);
	if (!g_address_rep.empty() && g_port_rep != NULL)
	{
		int ret = REP->Bind(g_address_rep.c_str(), g_port_rep);
		TMCLOG("REP Bind" + g_address_rep + ":" + to_string(g_port_rep) + " result=" + to_string(ret));
	}
}
void init_req(){
	if (!g_address_req.empty() && g_port_req != NULL)
	{
		int ret = REQ->Connect(g_address_req.c_str(), g_port_req);
		TMCLOG("REQ Connect" + g_address_req + ":" + to_string(g_port_req) + " result=" + to_string(ret));
	}
}
string req_send_and_recv(string send){
	const int BUFFER_SIZE = 4 * 1024 * 1024 - 1;
	char * buffer = NULL;
//	buffer[BUFFER_SIZE] = 0;
	size_t _size =0;
	int ret = 0;
	string msg = CreateLoad(send);
	string csv = string_replace(msg,"\\\\","\/");
	if (!send.empty())
	{
		try{
			//REQ->Send(s, strlen(s));
			//ParseLoad(CreateLoad(send))
			ret = REQ->SendString(csv);
			REQ->Recv(buffer, _size);
		}
		catch (exception x){
			LOG_WARN("zmq exception %s", x);
			return "exception error";
		}
	}
	std::string sbuffer;

	if (buffer != NULL && _size > 0){
		sbuffer.assign(buffer, _size);
	}
	string logSend = "REQ Send->" + csv + " result=" + to_string(ret);
	string logRecv = "REQ Recv->" + sbuffer;
	TMCLOG(logSend);
	TMCLOG(logRecv);
	return sbuffer;
}

int dispatch_request_proc(char * data, int length, int timeout)
{
	//if model change , kill MTMS and start a new MTMS
	//req sequencer a msg to load csv
	string buffer = "";
	int ret = 100;
	string recv = data;
	if (recv.find("pre") != string::npos || recv.find("Pre") != string::npos || recv.find("PRE") != string::npos)
	{
		if (g_model=="post")
		{
			kill_MTMS("MTMS");
			launce_MTMS(g_mtms_pre);
			g_model = "pre";
		}
		if (!g_station.empty() && !g_csv_pre.empty()){		//req sequencer a msg to load csv
			buffer = req_send_and_recv(g_csv_pre);
		}
		else
		{
			TMCLOG("get pre from mainUI and pre_model_get_non_csv");
		}
	}
	else if (recv.find("post") != string::npos || recv.find("Post") != string::npos || recv.find("POST") != string::npos){
		if (g_model == "pre")
		{
			kill_MTMS("MTMS");
			launce_MTMS(g_mtms_post);
			g_model = "post";
		}
		if (!g_station.empty() && !g_csv_post.empty()){		//req sequencer a msg to load csv
			buffer = req_send_and_recv( g_csv_post);
		}
		else
		{
			TMCLOG("get post from mainUI and post_model_get_non_csv");
		}
	}

	return ret;
}

bool rep_poller(int timeout){
	zmq_pollitem_t items[1];
	void * socket = REP->GetSocket();
	items[0].socket = socket;
	items[0].events = ZMQ_POLLIN;

	int rc = zmq_poll(items, 1, timeout);
	if (rc <= 0)
	{
		return false;
	}
	return true;
}

int rep_thread_proc(){
	const int BUFFER_SIZE = 4 * 1024 * 1024 - 1;
	char * buffer = new char[BUFFER_SIZE + 1];
	buffer[BUFFER_SIZE] = 0;
	if (g_address_rep.empty() || g_port_rep == NULL)
	{
		TMCLOG("get none zmq config information , thread exit!");
		return -1;
	}

	try{
		//while (true)
		{
			int rcv_cnt = REP->recv_n(buffer, BUFFER_SIZE);
			if (rcv_cnt < 0)
			{
				TMCLOG("server error!!!!\n");
				REP->Close();
				init_rep();
				//continue;
			}
			else if (0 == rcv_cnt)
			{
				Sleep(500);
				//continue;
			}
			buffer[rcv_cnt] = 0;
			string pri = buffer;
			string log = "rep recv-->>" + pri;
			TMCLOG(log);
			REP->send_n("OK",2);
			dispatch_request_proc(buffer, rcv_cnt, 2000);
		};
	}
	catch (exception x){
		LOG_WARN("zmq exception %s", x);
		return -1;
	}
	
	delete[] buffer;

	return 0;
}

string get_local_ip()
{
	// get local ip  
	WSADATA wsa;
	int ret = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (ret != 0)
	{
		return "127.0.0.1";
	}
	
	char hostname[MAX_PATH] = { 0 };
	gethostname(hostname, MAX_PATH);
	
	struct hostent FAR* lpHostEnt = gethostbyname(hostname);
	if (lpHostEnt == NULL)
	{
		return "127.0.0.1";
	}

	// get first ip  
	LPSTR lpAddr = lpHostEnt->h_addr_list[0];

	// tostring  
	struct in_addr inAddr;
	memmove(&inAddr, lpAddr, 4);
	string ip = inet_ntoa(inAddr);
	WSACleanup();
	return ip;//inet_ntoa(inAddr);
}

void station_search(string path_xmlconfig, string c, string ip_local){
	XmlNode xml;
	bool flag = XmlNode::ParseXml(path_xmlconfig, xml);
	if (!flag){
		string log = "parse xml failed from " + path_xmlconfig;
		TMCLOG(log.c_str());
	}
	std::vector<XmlNode> tabc1 = xml.GetChilds("TMCCONFIG.STATIONIP."+c+".IP");
	string c1[10];
	for (int i = 0; i < tabc1.size(); i++)
	{
		XmlNode node = tabc1.at(i);
		c1[i] = node.Value<std::string>();
		if (0 == strcmp(c1[i].c_str(), ip_local.c_str()))
		{
			g_station = c;
		}
	}
}

string path_auto(string p){
	if (p.find("..") != string::npos)
	{
		char p1[MAX_PATH];
		_getcwd(p1, MAX_PATH);
		string p2 = p1;
		int x = p2.find_last_of("\\",string::npos);
		if (x>0)
		{
			p2 = p2.substr(0, x);
		}
		string p4 = p.substr(2, strlen(p.c_str()) - 2);
		return p2 + p4;
	}
	else
	{
		return p;
	}
}

void read_xml_config(string path_xmlconfig){
	//string path_xmlconfig = "C:\\Project\\TMC\\Debug\\tmcconfig.xml";
	XmlNode xml ;
	bool flag = XmlNode::ParseXml(path_xmlconfig, xml);
	if (!flag){
		string log = "parse xml failed from " + path_xmlconfig ;
		TMCLOG(log.c_str());
	}

	g_address_rep = xml.Child("TMCCONFIG.TMCREPAddress").Value<string>();
	g_address_req = xml.Child("TMCCONFIG.TMCREQAddress").Value<string>();
	g_port_rep = xml.Child("TMCCONFIG.TMCREPPort").Value<int>();
	g_port_req = xml.Child("TMCCONFIG.TMCREQPort").Value<int>();
	g_mtms_pre = xml.Child("TMCCONFIG.MODEL.PRE.MTMS").Value<string>();
	g_mtms_post = xml.Child("TMCCONFIG.MODEL.POST.MTMS").Value<string>();
	g_mtms_add = xml.Child("TMCCONFIG.MTMSAddress").Value<string>();
	g_mtms_port = xml.Child("TMCCONFIG.MTMSPort").Value<int>();

	//get local ip and compare
	//string ip_local = get_local_ip();
	std::vector<std::string> add;
	if (!GetAddNetAddress(add))
	{
		TMCLOG("get local ip config error!");
	}
	else
	{
		string station_name[4] = { "C1", "C2", "C3", "C4" };
		for (unsigned int j = 0; j < add.size(); j++)
		{
			if (g_station.empty())
			{
				for (unsigned int i = 0; i < 4; i++)
				{
					if (g_station.empty())
					{
						station_search(path_xmlconfig, station_name[i], add[j]);
					}
				}
			}
		}
	}

	if (!g_station.empty())
	{
		XmlNode preNode = xml.Child("TMCCONFIG.MODEL.PRE." + g_station + ".CSV");
		if (preNode.isValid())
		{
			g_csv_pre = path_auto(preNode.Value<std::string>());
		}
		else
		{
			TMCLOG("TMCCONFIG.MODEL.PRE." + g_station + ".CSV is NULL");
		}
		XmlNode postNode = xml.Child("TMCCONFIG.MODEL.POST." + g_station + ".CSV");
		if (postNode.isValid())
		{
			g_csv_post = path_auto(postNode.Value<std::string>());
		}
		else
		{
			TMCLOG("TMCCONFIG.MODEL.PRE." + g_station + ".CSV is NULL");
		}
	}
	string log = "GSTATION=" + g_station + " ;CSV-pre=" + g_csv_pre + " CSV-post=" + g_csv_post;
	TMCLOG(log);
}

int _tmain(int argc, _TCHAR* argv[])
{
	SetConsoleCtrlHandler(ConsoleHandler, TRUE);
	SYSTEMTIME st;
	GetLocalTime(&st);
	char filename[100] = { 0 };
	sprintf_s(filename, "\\Log\\TMC_%d%02d%02d.txt", st.wYear, st.wMonth, st.wDay);
	setvbuf(stdout, NULL, _IONBF, 0);

	char p[MAX_PATH];
	_getcwd(p, MAX_PATH);
	string p1 = p;
	string path = p1 +"\\tmcconfig.xml";
	read_xml_config(path);//get station , model and so on
	
	string filepath = p1.append(filename);
	INIT_LOG(filepath);

	init_rep();
	init_req();

	//search redis for model
	CRedis * redis = new CRedis();
	redis->Connect(g_mtms_add.c_str(), g_mtms_port);
	std::string lotid;
	string model;
	redis->ReadElement("CurrentLotID", lotid);
	redis->ReadHashElement(lotid, "StepName", model);
	redis->Quit();
	model = "Pre";
	//start MTMS 61806/61807
	if (model.find("Pre") != string::npos || model.find("pre") != string::npos || model.find("PRE") != string::npos)
	{
		kill_MTMS("MTMS");
		launce_MTMS(g_mtms_pre);
		g_model = "pre";
		if (!g_csv_pre.empty())
		{
			req_send_and_recv( g_csv_pre);
		}
	}
	else if (model.find("Post") != string::npos || model.find("post") != string::npos || model.find("POST") != string::npos)
	{
		kill_MTMS("MTMS");
		launce_MTMS(g_mtms_post);
		g_model = "post";
		if (!g_csv_post.empty())
		{
			req_send_and_recv( g_csv_post);
		}
	}
	else
	{
		TMCLOG("ERROR:get model from redis wrong -->>" + model);
	}	

	//start a thread to listen msg from mainUI(ghb)
	//std::thread  rep_mt(rep_thread_proc);
	//rep_mt.join();
	while (true)
	{
		if (rep_poller(500)){
			rep_thread_proc();
		}
	}

	Sleep(2000);
	return 0;
}