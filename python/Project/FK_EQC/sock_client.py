# -*- coding: utf-8 -*-
import socket
import time
import threading
import select
import Callback
import logging

class sock_client:
	"""docstring for sock"""
	def __init__(self, arg, name="sock",eof="\r\n"):
		#防止主线程socket被close了，子线程还在recv而引发的异常
		self.s_lock = threading.Lock()
		self.name = name
		self.loop = True
		self.eof = eof
		self.status = True
		self.ip = str(name) +"->" +str(arg[0]) +":"+ str(arg[1])

		# socket.setdefaulttimeout(3)
		self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		try:
			self.s.connect(arg)
		except Exception as e:
			logging.error(str(self.ip)+" -> "+str(e))
			self.status = False
		else:
			logging.info(" :-) "+str(self.ip) + "Success!!!!!!")
		finally:
			pass
			
	def set_callback(self,callback):
		self.callback = callback

	def set_eof(self,eof):
		self.eof = eof

	def read_until_eof(self):
		# 使用select监听客户端（这里客户端需要不停接收服务端的数据，所以监听客户端）
        # 第一个参数是要监听读事件列表，因为是客户端，我们只监听创建的一个socket就ok
        # 第二个参数是要监听写事件列表，
        # 第三个参数是要监听异常事件列表，
        # 最后一个参数是监听超时时间，默认永不超时。如果设置了超时时间，过了超时时间线程就不会阻塞在select方法上，会继续向下执行
        # 返回参数 分别对应监听到的读事件列表，写事件列表，异常事件列表
		d = ""
		while self.status:
			rs, _, _ = select.select([self.s], [], [], 10)
			for r in rs:
				if not self.s:
					logging.error("socket not Exist")
					time.sleep(1)
					return
				self.s_lock.acquire()
				t = r.recv(1024)
				if not t:
					self.status = False
					logging.error(":-( socket to ("+str(self.ip)+") disconnect")
					break
				# logging.info(str(t))
				d += t
				self.s_lock.release()
			if self.eof in d:
				logging.info("< recv from "+str(self.ip) +" > : "+str(d))
				return d	

	def _read_thread(self):
		while self.loop:
			if  not self.status:
				break;
			if not self.s:
				break
			tmp = self.read_until_eof()
			if tmp and len(tmp)>0:
				self.callback.c_action(tmp)
		logging.info("Exit read background thread")

	def run(self):
		if not self.status:
			return
		logging.info("start read background thread || "+str(self.name)+" -> " + str(self.ip))
		t1 = threading.Thread(target=self._read_thread, name=self.name)
		t1.start()

	def stop(self):
		self.loop = False
		self.s_lock.acquire()
		self.s.close()
		self.s_lock.release()

	def send_data(self, data):
		if self.status:
			logging.info("< send to "+str(self.ip) + " > : "+str(data))
			self.s.sendall(data)
		else:
			logging.error("Can't send data due to Socket disconnect " +str(self.ip))

	def send_data_with_eof(self, data):
		self.send_data(str(data)+self.eof)
