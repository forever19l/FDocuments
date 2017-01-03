# coding: utf-8
import json
import logging
import os
from random import randint
import re
from smart_qq_bot.signals import on_group_message

"""
    data save as two table.
        nametable save the key(name) ,it cann't be del
        msgtable save the value(msg) 

    add_rule :      '！add-(key)--(value)'   ,add to two table
    remove_rule :   '！remove=(value)'       ,remove the value in msgtable
    send_msg : search the key in nametable ,if in ,so match a randommsg in msgtable and send it.
"""

class Randommsg(object):

    def __init__(self, key_file, value_file):
        self.nametable = []
        self.msgtable = []
        self.load(key_file,value_file)
        self.key_file = key_file
        self.value_file = value_file
        self._learn_regex = re.compile("^add=(.*)=(.*)=")
        self._remove_regx = re.compile("^remove=(.*)?")

    def is_learn(self, key):
        result = re.findall(self._learn_regex, key)
        if result:
            return result[0]
        return None

    def is_remove(self, key):
        result = re.findall(self._remove_regx, key)
        if result:
            return result[0]
        return None

    def load(self, key_file, value_file):
        if os.path.isfile(key_file):
            with open(key_file, "r") as f:
                self.nametable = json.load(f)
                logging.info("Randommsg key_file have loaded.")

        if os.path.isfile(value_file):
            with open(value_file, "r") as g:
                self.msgtable = json.load(g)
                logging.info("Randommsg value_file have loaded.")

    def add_rule(self, key, value):
        if key not in self.nametable:
            self.nametable.append(key)

        self.msgtable.append(value)
        # self.nametable[key].append(key)
        self.save()
        logging.info("add key-value sueccess :[%s]" % key)
        logging.info("add value :[%s]" % value)

    def remove_rule(self, value):       #del value in msgtable
        if value in self.msgtable:
            _remove_index = self.msgtable.index(value)
            del self.msgtable[_remove_index]

        self.save()
        logging.info("msgtable [%s] removed" % value)

    def match(self, key):
        if key in self.nametable:
            _numvalue = randint(0,len(self.msgtable)-1)     #match key and return a random value in msgtable
            result = self.msgtable[_numvalue]
        
            if key == "老脏" :
                hah_list = ["强的不行","喊粑粑干嘛","带你飞"]
                result =  hah_list[randint(0,len(hah_list)-1)]

            return result#[randint(0, len(result) - 1)]     #random in match table msg
        return None

    def save(self):
        #need search file,if unexsited,make it.
        with open(self.key_file, "w") as f:
            json.dump(self.nametable, f)
        logging.info("Randommsg key_file saved.")
        with open(self.value_file, "w") as g:
            json.dump(self.msgtable, g)
        logging.info("Randommsg value_file saved.")

randommsg = Randommsg("randommsg_key.json","random_value.json")
logging.info("plugin randommsg had been load")

@on_group_message(name="randommsg")
def send_msg(msg, bot):
    """
    :type bot: smart_qq_bot.bot.QQBot
    :type msg: smart_qq_bot.messages.GroupMsg
    """
    result = randommsg.is_learn(msg.content)
    if result:
        key, value = result
        # logging.info("learn key :[%s]" % key)
        # logging.info("learn value :[%s]" % value)
        randommsg.add_rule(key, value)
    else:
        response = randommsg.match(msg.content)
        # logging.info("mathc messages : [%s]" % response)
        if response:
            bot.send_qun_msg(msg.from_uin, response, msg_id=randint(1, 1000))
    result = randommsg.is_remove(msg.content)
    if result:
        randommsg.remove_rule(result)