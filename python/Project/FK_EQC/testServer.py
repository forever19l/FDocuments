#!/usr/bin/python
# -*- coding: utf-8 -*-
# example for client-client mode
from sock_server import sock_server
from Eqc_Log import eqc_log
from data_center import c_s_center
from ip_list import SERVER_MES
from ip_list import SERVER_PLC

if __name__ == "__main__":
    l = eqc_log()
    server_plc = sock_server(SERVER_PLC,"PLC_Server")#here is server, MES Run as client
    server_mes = sock_server(SERVER_MES,"MES_Server")#here is client, PLC Run as server
    #c_s_center(server_plc,server_mes)
    server_plc.run()
    server_mes.run()
    data1 = "\"ISR\" {\"WO\":\"WO0231\",\"EquipmentID\":\"L01.OP020\",\"SN\":null,\"PartCode\":null,\"CheckResult\":null,\"Description\":null,\"TestItems\":{}}\r\n"  
    server_mes.send_data(SERVER_MES,data1)