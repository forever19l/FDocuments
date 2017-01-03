# -*- coding: utf-8 -*-

# Define here the models for your scraped items
#
# See documentation in:
# http://doc.scrapy.org/en/latest/topics/items.html

import scrapy


class fztestItem(scrapy.Item):
    # define the fields for your item here like:
    # name = scrapy.Field()
    matitle = scrapy.Field()      #电影名
    melink = scrapy.Field()       #电影详情页面地址
    mfpic = scrapy.Field()        #图片地址
    mdret = scrapy.Field()        #电影时间，演员等信息
    mcdesc = scrapy.Field()       #电影简介
    mbfen = scrapy.Field()        #评分

    pass
