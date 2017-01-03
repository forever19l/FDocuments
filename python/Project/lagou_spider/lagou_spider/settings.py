# -*- coding: utf-8 -*-

# Scrapy settings for lagou_spider project
#
# For simplicity, this file contains only settings considered important or
# commonly used. You can find more settings consulting the documentation:
#
#     http://doc.scrapy.org/en/latest/topics/settings.html
#     http://scrapy.readthedocs.org/en/latest/topics/downloader-middleware.html
#     http://scrapy.readthedocs.org/en/latest/topics/spider-middleware.html

import os
# PROJECT_DIR = os.path.abspath(os.path.dirname(__file__))

BOT_NAME = 'lagou_spider'

SPIDER_MODULES = ['lagou_spider.spiders']
NEWSPIDER_MODULE = 'lagou_spider.spiders'


DOWNLOAD_DELAY = 1
CONCURRENT_ITEMS = 100
CONCURRENT_REQUESTS = 16
#The maximum number of concurrent (ie. simultaneous) requests that will be performed to any single domain.
CONCURRENT_REQUESTS_PER_DOMAIN = 8
CONCURRENT_REQUESTS_PER_IP = 0
DEPTH_LIMIT = 0
DEPTH_PRIORITY = 0
DNSCACHE_ENABLED = True
#DUPEFILTER_CLASS = 'scrapy.dupefilter.RFPDupeFilter'
#SCHEDULER = 'scrapy.core.scheduler.Scheduler'

#AutoThrottle extension
AUTOTHROTTLE_ENABLED = True
AUTOTHROTTLE_START_DELAY = 3.0
AUTOTHROTTLE_CONCURRENCY_CHECK_PERIOD = 10#How many responses should pass to perform concurrency adjustments.

#XXX:scrapy's item pipelines have orders!!!!!,it will go through all the pipelines by the order of the list;
#So if you change the item and return it,the new item will transfer to the next pipeline.
#XXX:notice:
#if you want to use shard mongodb,you need MongodblagouBookFile and ShardMongodbPipeline
#if you want to use single mongodb,you need lagouBookFile and SingleMongodbPipeline

# Configure item pipelines
# See http://scrapy.readthedocs.org/en/latest/topics/item-pipeline.html
ITEM_PIPELINES = [
#    'lagou_spider.pipelines.bookfile.lagouBookFile',
    # 'lagou_spider.pipelines.mongodb_book_file.MongodblagouBookFile',
    'lagou_spider.pipelines.drop_none_download.DropNoneBookFile',
#    'lagou_spider.pipelines.mongodb.SingleMongodbPipeline',
    'lagou_spider.pipelines.mongodb.ShardMongodbPipeline',
    'lagou_spider.pipelines.final_test.FinalTestPipeline',]


# IMAGES_STORE = os.path.join(PROJECT_DIR,'media/book_covor_image')
IMAGES_EXPIRES = 30
IMAGES_THUMBS = {
     'small': (50, 50),
     'big': (270, 270),
}

IMAGES_MIN_HEIGHT = 0
IMAGES_MIN_WIDTH = 0

COOKIES_ENABLED = False

USER_AGENT = 'Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.31 (KHTML, like Gecko) Chrome/26.0.1410.43 Safari/537.31'

# Enable or disable spider middlewares
# See http://scrapy.readthedocs.org/en/latest/topics/spider-middleware.html
#SPIDER_MIDDLEWARES = {
#    'lagou_spider.middlewares.MyCustomSpiderMiddleware': 543,
#}

# Enable or disable downloader middlewares
# See http://scrapy.readthedocs.org/en/latest/topics/downloader-middleware.html
#DOWNLOADER_MIDDLEWARES = {
#    'lagou_spider.middlewares.MyCustomDownloaderMiddleware': 543,
#}

DOWNLOADER_MIDDLEWARES = {
#    'lagou_spider.contrib.downloadmiddleware.google_cache.GoogleCacheMiddleware':50,
    'scrapy.contrib.downloadermiddleware.useragent.UserAgentMiddleware': None,
    'lagou_spider.contrib.downloadmiddleware.rotate_useragent.RotateUserAgentMiddleware':400,
}

GOOGLE_CACHE_DOMAINS = ['www.lagou.com',]

#To make RotateUserAgentMiddleware enable.
USER_AGENT = ''

FILE_EXPIRES = 30
BOOK_FILE_EXPIRES = 30
# FILE_STORE = os.path.join(PROJECT_DIR,'media/files')
# BOOK_FILE_STORE = os.path.join(PROJECT_DIR,'media/book_files')

#For more mime types about file,you can visit:
#http://mimeapplication.net/
BOOK_FILE_CONTENT_TYPE = ['application/file',
    'application/zip',
    'application/octet-stream',
    'application/x-zip-compressed',
    'application/x-octet-stream',
    'application/gzip',
    'application/pdf',
    'application/ogg',
    'application/vnd.oasis.opendocument.text',
    'application/vnd.openxmlformats-officedocument.wordprocessingml.document',
    'application/x-dvi',
    'application/x-rar-compressed',
    'application/x-tar',
    'multipart/x-zip',
    'application/x-zip',
    'application/x-winzip',
    'application/x-compress',
    'application/x-compressed',
    'application/x-gzip',
    'zz-application/zz-winassoc-arj',
    'application/x-stuffit',
    'application/arj',
    'application/x-arj',
    'multipart/x-tar',
    'text/plain',]

# URL_GBK_DOMAIN = ['www.paofuu.com',
#         'down.wmtxt.com',
#         'www.txt163.com',
#         'down.txt163.com',
#         'down.sjtxt.com:8199',
#         'file.txtbook.com.cn',
#         'www.yyytxt.com',
#         'www.27xs.org',
#         'down.dusuu.com:8199',
#         'down.txtqb.cn']
ATTACHMENT_FILENAME_UTF8_DOMAIN = []
FILE_EXTENTION = ['.doc','.txt','.docx','.rar','.zip','.pdf']
Drop_NoneBookFile = True
LOG_FILE = "logs/scrapy.log"

# STATS_CLASS = 'lagou_spider.statscol.graphite.RedisGraphiteStatsCollector'
# GRAPHITE_HOST = '127.0.0.1'
# GRAPHITE_PORT = 2003
# GRAPHITE_IGNOREKEYS = []

SingleMONGODB_SERVER = "localhost"
SingleMONGODB_PORT = 27017
SingleMONGODB_DB = "books_fs"

ShardMONGODB_SERVER = "localhost"
ShardMONGODB_PORT = 27017
ShardMONGODB_DB = "books_mongo"
GridFs_Collection = "book_file"

SCHEDULER = "lagou_spider.scrapy_redis.scheduler.Scheduler"
SCHEDULER_PERSIST = False
SCHEDULER_QUEUE_CLASS = 'lagou_spider.scrapy_redis.queue.SpiderPriorityQueue'

# Crawl responsibly by identifying yourself (and your website) on the user-agent
#USER_AGENT = 'lagou_spider (+http://www.yourdomain.com)'

# Configure maximum concurrent requests performed by Scrapy (default: 16)
#CONCURRENT_REQUESTS=32

# Configure a delay for requests for the same website (default: 0)
# See http://scrapy.readthedocs.org/en/latest/topics/settings.html#download-delay
# See also autothrottle settings and docs
#DOWNLOAD_DELAY=3
# The download delay setting will honor only one of:
#CONCURRENT_REQUESTS_PER_DOMAIN=16
#CONCURRENT_REQUESTS_PER_IP=16

# Disable cookies (enabled by default)
#COOKIES_ENABLED=False

# Disable Telnet Console (enabled by default)
#TELNETCONSOLE_ENABLED=False

# Override the default request headers:
#DEFAULT_REQUEST_HEADERS = {
#   'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8',
#   'Accept-Language': 'en',
#}

# Enable or disable extensions
# See http://scrapy.readthedocs.org/en/latest/topics/extensions.html
#EXTENSIONS = {
#    'scrapy.telnet.TelnetConsole': None,
#}


# Enable and configure the AutoThrottle extension (disabled by default)
# See http://doc.scrapy.org/en/latest/topics/autothrottle.html
# NOTE: AutoThrottle will honour the standard settings for concurrency and delay
#AUTOTHROTTLE_ENABLED=True
# The initial download delay
#AUTOTHROTTLE_START_DELAY=5
# The maximum download delay to be set in case of high latencies
#AUTOTHROTTLE_MAX_DELAY=60
# Enable showing throttling stats for every response received:
#AUTOTHROTTLE_DEBUG=False

# Enable and configure HTTP caching (disabled by default)
# See http://scrapy.readthedocs.org/en/latest/topics/downloader-middleware.html#httpcache-middleware-settings
#HTTPCACHE_ENABLED=True
#HTTPCACHE_EXPIRATION_SECS=0
#HTTPCACHE_DIR='httpcache'
#HTTPCACHE_IGNORE_HTTP_CODES=[]
#HTTPCACHE_STORAGE='scrapy.extensions.httpcache.FilesystemCacheStorage'
