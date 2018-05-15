# -*- coding: utf-8 -*-
import SocketServer
import logging
import Callback

_client_socket = {}
_client_data = {}
_eof = "\r\n"
_callback = None

class serverHandle(SocketServer.StreamRequestHandler):
	"""docstring for serverHandle"""
	def setup(self):
		#重写此处，将不能够使用rfile.readline()行读取
		# add = (self.client_address[0].strip()+":"+str(self.client_address[1]))
		# name = self.request.getsockname()
		# add = name[0].strip()+":"+str(name[1])
		# print(add)
		add = self.client_address[0].strip()
		logging.info(" :-) "+add + " Connected Success!!!!!!")
		_client_socket[add] = self.request
		
	def handle(self):
		while True:
			self.data = self.request.recv(1024)
			if not self.data:
				return
			# name = self.request.getsockname()
			# add = name[0].strip()+":"+str(name[1])
			add = self.client_address[0].strip()
			if _client_data.has_key(add):
				_client_data[add] += str(self.data)
			else:
				_client_data[add] = str(self.data)
			if(_eof in _client_data[add]):
				if _callback:
					logging.info("< recv from "+str(add)+" > : "+_client_data[add])
					_callback.s_action(add, _client_data[add])
				_client_data[add] = ""
			# self.request.send("Shit")
			#行读取
			# self.data = self.rfile.readline().strip()
			# print("{} wrote:".format(self.client_address))
			# self.wfile.write(self.data)

	def finish(self):
		# add = self.client_address[0].strip()+":"+str(self.client_address[1])
		# name = self.request.getsockname()
		# add = name[0].strip()+":"+str(name[1])
		# print(add)
		add = self.client_address[0].strip()
		logging.error(" :-( "+ add + " Disconnect")
		# print(_client_socket)
		_client_socket.pop(add)


class sock_server:
	"""docstring for socket_server"""
	def __init__(self, arg, name="sock",eof="\r\n"):
		logging.info("Start Server to spy " + str(arg[0]) + ":" +str(arg[1]))
		self.server = SocketServer.ThreadingTCPServer(arg,serverHandle)

	def run(self):
		self.server.serve_forever()

	def set_callback(self, cbk):
		global _callback 
		_callback = cbk

	def get_clients(self):
		return _client_socket

	def get_client(self,address):
		if _client_socket.has_key(address):
			return _client_socket[address]
		else:
			return None

	def send_data(self, address, data):
		s = self.get_client(str(address))
		if s:
			logging.info("< send to "+str(address) + " > : "+str(data))
			s.sendall(data)
		else:
			logging.error("Can't send data due to Socket disconnected " +str(address))

	def send_data_with_eof(self, address, data):
		self.send_data(address, str(data) + _eof)


# s = sock_server(("192.168.67.196",7610))
# s.run()


