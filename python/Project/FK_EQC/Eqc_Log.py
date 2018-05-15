# -*- coding: utf-8 -*-
import logging
import os
import time

class eqc_log(object):
	"""docstring for Eqc_Log"""
	def __init__(self, path=None):
		p = time.strftime('%Y-%m-%d',time.localtime(time.time()))+".log"
		if path:
			p = path
		else:
			p = os.path.curdir+os.sep+"log"+os.sep+p
		p = os.path.abspath(p)
		folder = os.path.dirname(p)
		if not os.path.exists(folder):
			os.makedirs(folder)

		logging.basicConfig(level=logging.DEBUG,
			format="%(asctime)s [ %(levelname)s ]   %(message)s",
			filename=p,
			filemode='a')

		console = logging.StreamHandler()
		console.setLevel(logging.ERROR)

		logging.getLogger('').addHandler(console)

