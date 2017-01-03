# -*- coding: utf-8 -*-

# Define your item pipelines here
# Don't forget to add your pipeline to the ITEM_PIPELINES setting
# See: http://doc.scrapy.org/en/latest/topics/item-pipeline.html
import json
import codecs
from scrapy import signals

from scrapy.exceptions import DropItem

class JsonWithEncodingPipeline(object):
    def __init__(self):
        self.file = codecs.open('douban.json', 'w', encoding='utf-8')
    def process_item(self, item, spider):
        line = json.dumps(dict(item), ensure_ascii=False,sort_keys = True) + "\n"
        self.file.write(line)
        return item
    def spider_closed(self, spider):
        self.file.close(
)

class DuplicatesPipeline(object):
    def __init__(self):
        self.ids_seen = set()                                   #创建一个新的对象

    def process_item(self, item, spider):
        if item['matitle'][0] in self.ids_seen:                 #id是否在创建的对象中
            raise DropItem("Duplicate item found: %s" % item)
        else:
            self.ids_seen.add(item['matitle'][0])               #不在里面则添加进去
            return item

# import MySQLdb
# import MySQLdb.cursors
# from twisted.enterprise import adbapi
# import logging

# class PipelineByMysql(object):
#     def __init__(self,dbpool):
#         self.dbpool = dbpool

#     @classmethod
#     def from_crawler(cls,crawler):
#         dbargs = dict(
#             host = crawler.settings.get('MYSQL_HOST'),
#             db = crawler.settings.get('MYSQL_DBNAME'),
#             user = crawler.settings.get('MYSQL_USER'),
#             passwd = crawler.settings.get('MYSQL_PASSWORD'),
  
#             charset = 'utf8',
#             cursorclass = MySQLdb.cursors.DictCursor,
#             use_unicode = True,
#         )
#         dbpool = adbapi.ConnectionPool('MySQLdb',**dbargs)
#         return cls(dbpool)

#     def process_item(self, item, spider):
#         query = self.dbpool.runInteraction(self._do_upinsert,item)
#         query.addErrback(self._handle_error,item)
#         query.addBoth(lambda  _:item)
#         return item

#     def _do_upinsert(self,conn,item):
#         conn.execute("select * from movie250 where name= %s",(item['name'][0],))
#         result = conn.fetchone()
#         # log.msg(result,level=log.DEBUG)
#         logging.log(logging.DEBUG,"输出结果 %s" % result)

#         if result:
#             # log.msg("Item already stored in db : %s" %item,level=log.DEBUG)
#             logging.log(logging.DEBUG,"Item already stored in db : %s" %item)
#         else:
#             classification = actor  = director = year =''

#             for i in range(len(item['classification'])):
#                 classification += item['classification'][i]
#                 if i < len(item['classification'])-1:
#                     classification += '/'

#             for i in range(len(item['actor'])):
#                 actor += item['actor'][i]
#                 if i < len(item['actor'])-1:
#                     actor += '/'

#             for i in range(len(item['director'])):
#                 director += item['director'][i]
#                 if i < len(item['director'])-1:
#                     director += '/'
  
#             for i in range(len(item['year'])):
#                 year += item['year'][i]
#                 if i < len(item['year'])-1:
#                     year += '/'

#             conn.execute("insert into movie250(name,year,score,director,classification,actor)"
#                          " VALUES(%s,%s,%s,%s,%s,%s)", \
#                     (item['name'][0],year,item['score'][0],director,classification,actor))
#             # log.msg("Item stroed stored in db : %s" %item,level=log.DEBUG)
#             logging.log(logging.DEBUG,"Item already stored in db : %s" %item)

#     def _handle_error(self,e):
#         # log.err(e)
#         logging.log(e)