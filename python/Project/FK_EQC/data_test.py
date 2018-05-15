#!/usr/bin/python
# -*- coding: utf-8 -*-
from protocol_key import MES_FUNC,MES_EQU,MES_SN,MES_PARTCODE,MES_RESULT,MES_DES,MES_ITEMS
from protocol import MES_Protocol,PLC_Protocol

class c_c_center(object):
    def __init__(self):
        pass

    def mes_data_mamager(self,data):
        #数据来源mes，处理后发送给plc
        #处理数据流
        self.mes_protocol = MES_Protocol()
        str_func = data[0:6]
        print str_func
        str_jsong = data[6:].rstrip()
        dic = self.mes_protocol.parse(str_jsong)
        print dic['WO']
        if dic[MES_SN] == 'DS201568974':
            self.mes_protocol.CheckResult(dic,1)
        else:
            self.mes_protocol.CheckResult(dic,0)
        print dic
        print str_func + self.mes_protocol.tostring(dic)
        #dic = self.mes_protocol.CheckResult(dic,1)

if __name__ == "__main__":
    str_a = ""
    str_a = "\"ISR\" {\"WO\":\"WO0231\",\"EquipmentID\":\"L01.OP010\",\"SN\":null,\"PartCode\":null,\"CheckResult\":null,\"Description\":null,\"TestItems\":{}}\r\n"
    c = c_c_center()
    c.mes_data_mamager(str_a)
    str_v = "\"ISR\",\"start,OK\"\r\n"
    print str_v.rstrip()
    print str_v.rstrip()[-3:] 