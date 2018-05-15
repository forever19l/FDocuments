#!/usr/bin/python
# -*- coding: utf-8 -*-
# example for client-client mode
from sock_client import sock_client
from data_center import c_c_center
from eqc_log import eqc_log

from ip_list import CLIENT_MES
from ip_list import CLIENT_PLC_DIC

if __name__ == "__main__":
    l = eqc_log()
    client_mes = sock_client(CLIENT_MES,"MES")#here is client, MES Run as server
    client_plc = {}
    for k,v in CLIENT_PLC_DIC.items()
        client_plc[k] = sock_client(v,"PLC_"+k)
    c_c_center(client_mes,client_plc)
    client_mes.run()
    for k,v in client_plc.items():
        v.run()
