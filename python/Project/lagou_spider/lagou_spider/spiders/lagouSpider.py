# -*- coding: utf-8 -*-

import scrapy
import time
from pprint import pprint

from scrapy.selector import HtmlXPathSelector
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

from lagou_spider.items import *
from lagou_spider.utils.select_result import list_first_item,strip_null,deduplication,clean_url


class LagouSpider(CrawlSpider):
    name = "lagou_spider"
    start_urls = (
            'http://www.lagou.com/jobs/list_python?labelWords=&fromSearch=true&suginput=',
    )

    def parse(self,response):
        response_selector = HtmlXPathSelector(response)
        next_link = list_first_item(response_selector.select(u'//div[@class="item_top"]/div/a[text()="下一页"]/@href').extract())
        if next_link:
            next_link = clean_url(response.url,next_link,response.encoding)
            yield Request(url=next_link, callback=self.parse)

        for detail_link in response_selector.select(u'//li[@class="con_list_item"]/div[@class="list_item_top"]/div[@class="position"]/div[@class="p_top"]/a/@href').extract():
            if detail_link:
                detail_link = clean_url(response.url,detail_link,response.encoding)
                yield Request(url=detail_link, callback=self.parse_detail)

    def parse_detail(self, response):
        lagou_item = lagouCrawlerItem()

        response_selector = HtmlXPathSelector(response)
        # lagou_item['author'] = [list_first_item(response_selector.select('//div[@class="xiaoxiao"][1]/text()').extract())[5:].strip(),]
        # lagou_item['book_description'] = list_first_item(response_selector.select('//div[@class="lili"][1]/text()').extract()).strip()
        lagou_item['jobreq'] = list_first_item(response_selector.select('//dd[@class="job_request"]/text()')).extract().strip()
        lagou_item['jobdetail'] = list_first_item(response_selector.select('//dd[@class="job_bt"]/text()')).extract().strip()

        yield lagou_item
