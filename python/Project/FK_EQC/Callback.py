class Callback(object):
	"""docstring for Callback"""
	def __init__(self, instance, function_name):
		self.instance = instance
		self.function_name = function_name
#client
	def c_action(self,params):
		try:
			self.instance.__getattribute__(self.function_name)(params)
		# except Exception as e:
		# 	# raise
		# else:
		# 	pass
		finally:
			pass
		# self.instance.__getattribute__(self.function_name)(params)
#server
	def s_action(self,add,data):
		try:
			self.instance.__getattribute__(self.function_name)(add,data)
		# except Exception as e:
		# 	# raise
		# else:
		# 	pass
		finally:
			pass
		# self.instance.__getattribute__(self.function_name)(params)