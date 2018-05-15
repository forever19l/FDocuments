from protocol_key import MES_FUNC,MES_EQU,MES_SN,MES_PARTCODE,MES_RESULT,MES_DES,MES_ITEMS

import json

class MES_Protocol(object):
	"""docstring for MES_Protocol"""
	def __init__(self):
		self.instance = self
	def parse(self,par):
		try:
			j = json.loads(par)
			return j
		except Exception as e:
			raise e
	def tostring(self,par):
		try:
			j = json.dumps(par)
			return j
		except Exception as e:
			raise e
	def CheckResult(self,data,flag):
		try:
			if flag:
				data[MES_RESULT] = 'OK'
			else:
				data[MES_RESULT] = 'NG'
		except Exception as e:
			raise e


class PLC_Protocol(object):
	"""docstring for PLC_Protocol"""
	def __init__(self, arg):
		self.arg = arg
		