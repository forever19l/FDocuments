# -*- coding: utf-8 -*-

import scrapy

from scrapy.selector import Selector 
try:
    from scrapy.spiders import Spider
except:
    from scrapy.spiders import BaseSpider as Spider
# from scrapy.utils.url import urljoin_rfc                  #获取url时使用
#     base_url = get_base_url(response)
#     relative_url = site.css('.l.square a').xpath('@href').extract()[0]
#     item['detailLink'] = urljoin_rfc(base_url, relative_url)
from scrapy.spiders import CrawlSpider, Rule
from scrapy.linkextractors import LinkExtractor
from scrapy.http import Request
from fztest.items import *

class FztestSpider(CrawlSpider):
    name = "fztest"
    allowed_admins = ["fztest.org"]
    allowed_domains = ["movie.douban.com"]
    start_urls = [
        # "http://www.dmoz.org/Computers/Programming/Languages/Python/",
        "https://movie.douban.com/top250"
    ]
    rules = [
            #豆瓣top250有第一页有两个地址，start=25 和 没有后缀 ；添加pipeline函数去重。
            Rule(LinkExtractor(allow=("https://movie.douban.com/top250")), #此处要注意?号的转换，复制过来需要对?号进行转义。
            follow=True,
            callback='parse_item')]

    def parse_item(self,response):                              #scrapy父类已定义parse，此处用parse做callback的话，则crawl spider运行失败。
        # filename = response.url.split("/")[-1]                #获取filename，截取url字符串内-n个后面的名字
        # open(filename,'wb').write(response.body)              #将scrapy crawl fztest爬取得内容写入文件
        postTitle = Selector(response).css('ol.grid_view div.item')

        for selec in range(len(postTitle)):
            item = fztestItem()
            item['matitle'] = postTitle[selec].css("div.info div.hd a span:nth-child(1)").xpath('text()').extract()
            item['mbfen'] = postTitle[selec].css('div.bd div.star span').xpath('text()').extract()
            item['melink'] = postTitle[selec].css("div.info a").xpath('@href').extract()
            item['mfpic'] = postTitle[selec].css("div.pic a img").xpath('@src').extract()
            item['mdret'] = postTitle[selec].css("div.bd p:nth-child(1)").xpath("text()".strip()).extract()
            item['mdret'] = item['mdret'][0].strip()
            item['mcdesc'] = postTitle[selec].css("div.bd p.quote span.inq").xpath('text()').extract()
            # item['mcdesc'] = postTitle[selec].xpath('//span[@class="inq"]/text()').extract()      #一次取整个页面内的数据
            yield item

        # nexturl = Selector(response).css('div.paginator span.next a').xpath('@href').extract()
        # nexturl = "https://movie.douban.com/top250/" + nexturl[0]
        # yield Request(nexturl,callback = self.parse)

