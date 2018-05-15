#!/usr/bin/python
# -*- coding: utf-8 -*-
# example for client-client mode
from sock_client import sock_client
from Eqc_Log import eqc_log

from ip_list import CLIENT_MES
from ip_list import CLIENT_PLC_DIC

from Callback import Callback
from protocol import MES_Protocol,PLC_Protocol
from protocol_key import MES_FUNC,MES_EQU,MES_SN,MES_PARTCODE,MES_RESULT,MES_DES,MES_ITEMS

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

    def mes_data_mamager(self,data):
        #数据来源mes，处理后发送给plc
        #解析数据
        str_func = data[:5]
        str_json = data[6:].rstrip()
        ret = self.data_parse.parse(str_json)
        self.data[str_func] = ret
        send_data  = ""
        if ("ISR" in str_func):
            send_data = str_func + ",\"start\""
        elif("SND" in str_func):
            send_data = str_func + ",\"" + ret[MES_SN] +"\",\"type\""

        #处理数据流
        if self.plc_clientdic.has_key(ret[MES_EQU]):    #has_key == __contains__
            self.plc_clientdic[ret[MES_EQU]].send_data_with_eof(send_data)
        else:
            self.mes_client.send_data_with_eof( str_func + " " + self.data_parse.tostring(self.data_parse.CheckResult(ret,0)) )

    def plc_data_mamager(self,data):
        #数据来源plc，处理后发送给mes 
        #处理数据流
        str_func = data[:5]
        data_send = self.data[str_func]
        if("SND" in str_func):
            if ("type" in data):
                data_send[MES_DES] = "type"
            else:
                data_send[MES_DES] = "sn"

        if ("OK" in data.rstrip()[-3:]):
            data_send =  str_func + " " +  self.data_parse.tostring(self.data_parse.CheckResult(data_send,1))
        else:
            data_send =  str_func + " " +  self.data_parse.tostring(self.data_parse.CheckResult(data_send,0))

        self.mes_client.send_data_with_eof( data_send)


if __name__ == "__main__":
    l = eqc_log()
    client_mes = sock_client(CLIENT_MES,"MES")#here is client, MES Run as server
    client_plc = {}
    for k,v in CLIENT_PLC_DIC.items():
        client_plc[k] = sock_client(v,"PLC_"+k)
    c_c_center(client_mes,client_plc)
    client_mes.run()
    for k,v in client_plc.items():
        v.run()
