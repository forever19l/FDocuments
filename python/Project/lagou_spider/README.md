lagou_spider
===================

使用scrapy框架+Redis实现分布式爬取网页数据。

--------------------------------------------------------
爬虫的爬取start_urls内的关键词，广度优先搜索
将爬虫爬取到的url request放入（push）到Redis queue内，然后所有的爬虫将从这个queue内获取request（url）
多个爬虫去queue内获取request时，scheduler会优先将前一个爬虫未完成的url pop出来
爬虫是可以同时调度多个的，为分布式爬取

===================
学习自young-hz