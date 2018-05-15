#!/usr/bin/python
# -*- coding: utf-8 -*-
# example for client-client mode
from sock_client import sock_client
from sock_server import sock_server
from data_center import c_s_center
from eqc_log import eqc_log

from ip_list import SERVER_ADDR
from ip_list import CLIENT1_ADDR

if __name__ == "__main__":
	l = eqc_log()
	server4Plc = sock_server(SERVER_ADDR,"PLC_Server")#here is server, MES Run as client
	client2Mes = sock_client(CLIENT1_ADDR,"MES_Client")#here is client, PLC Run as server
	c_s_center(server4Plc,client2Mes)
	client2Mes.run()
	server4Plc.run()