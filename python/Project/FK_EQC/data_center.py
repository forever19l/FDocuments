# -*- coding:utf-8 -*-
from Callback import Callback
from protocol import MES_Protocol,PLC_Protocol
from ip_list import CLIENT1_IP


class c_c_center(object):
    """docstring for control"""
    """PLC | server <--> client | EQC | client <--> server | MES """
    #client-client模式，仅有数据中转
    #当有多个client时，还有增加socket
    def __init__(self, mes_client, plc_clientdic):
        #创建对象，设置回调
        self.mes_client = mes_client
        self.plc_clientdic = plc_clientdic
        self.data_parse = MES_Protocol()
        self.data = {}
        self.mes_client.set_callback(Callback(self,self.mes_data_mamager.__name__))
        for k,v in self.plc_clientdic.items():
            v.set_callback(Callback(self,self.plc_data_mamager.__name__))
        #plc_client.set_callback(Callback(self,self.plc_data_mamager.__name__))

    def mes_data_mamager(self,data):
        #数据来源mes，处理后发送给plc
        #解析数据
        str_func = data[:5]
        str_json = data[6:].rstrip()
        ret = self.data_parse.parse(str_json)
        self.data[str_func] = ret
        #处理数据流
        if self.plc_clientdic.has_key(ret[MES_EQU]):    #has_key == __contains__
            self.plc_clientdic[ret[MES_EQU]].send_data_with_eof(str_func + "," + ret[MES_SN])
        else:
            self.mes_client.send_data_with_eof( str_func + " " + self.data_parse.tostring(self.data_parse.CheckResult(ret,0)) )

    def plc_data_mamager(self,data):
        #数据来源plc，处理后发送给mes 
        #处理数据流
        str_func = data[:5]
        data_send = self.data[str_func]
        if ("OK" in data):
            data_send =  str_func + " " +  self.data_parse.tostring(self.data_parse.CheckResult(data_send,1))
        else:
            data_send =  str_func + " " +  self.data_parse.tostring(self.data_parse.CheckResult(data_send,0))
        self.mes_client.send_data_with_eof( data_send)



class c_s_center(object):
	"""docstring for s_center"""
	"""PLC | client <--> server | EQC | server <--> client | MES"""
	#client-server模式，此处为plc为client，可能存在多个plc客户端，数据发送也可能是 PLC1->EQC->MES->EQC-PLC2，所以需要依据客户端地址选择连接发送数据
	def __init__(self, server, client):
		self.server = server
		self.client = client
		server.set_callback(Callback(self, self.server_data_mamager.__name__))
		client.set_callback(Callback(self, self.client_data_mamager.__name__))

	def server_data_mamager(self,add, data):
		#数据来源mes，处理后发送给plc
		#处理数据流
		self.client.send_data_with_eof(data)

	def client_data_mamager(self,data):
		#数据来源plc，处理后发送给mes 
		#处理数据流
		self.server.send_data_with_eof(CLIENT1_IP, data)
		