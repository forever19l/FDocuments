#coding:utf-8
import urllib2
import io
import random
import urllib
from bs4 import BeautifulSoup
import re
import os
import lxml
import requests
from lxml import etree

import sys
reload(sys)
sys.setdefaultencoding('utf8')

def GetHtml(url):
    if len(url) < 1:
        return ''
    print 'the spider work with --->>>' + str(url)
    user_agent = [
        'Mozilla/5.0 (Macintosh; Intel Mac OS X 10.10; rv:50.0) Gecko/20100101 Firefox/50.0'
        # 'Mozilla/5.0 (Windows NT 5.2) AppleWebKit/534.30 (KHTML, like Gecko) Chrome/12.0.742.122 Safari/534.30',
        # 'Mozilla/5.0 (Windows NT 5.1; rv:5.0) Gecko/20100101 Firefox/5.0',
        # 'Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.2; Trident/4.0; .NET CLR 1.1.4322; .NET CLR 2.0.50727; .NET4.0E; .NET CLR 3.0.4506.2152; .NET CLR 3.5.30729; .NET4.0C)',
        # 'Opera/9.80 (Windows NT 5.1; U; zh-cn) Presto/2.9.168 Version/11.50',
        # 'Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN) AppleWebKit/533.21.1 (KHTML, like Gecko) Version/5.0.5 Safari/533.21.1',
        # 'Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0; .NET CLR 2.0.50727; .NET CLR 3.0.04506.648; .NET CLR 3.5.21022; .NET4.0E; .NET CLR 3.0.4506.2152; .NET CLR 3.5.30729; .NET4.0C)'        
    ]
    #set web encoding, get chinese string
    encoding = "gb18030"
    #make http header，set user-agent
    header = {"User-Agent":random.choice(user_agent)}
    #make request with head
    request = urllib2.Request(url,headers=header)
    #get request result
    html = urllib2.urlopen(request).read()
    #use beautifulSoup to deal with html page,like dom
    soup = BeautifulSoup(html,'lxml')
    # print str(soup)
    return soup

#requests post url,get html same with firefox see
def GetRequestPost(url):
    data = requests.post(url)
    soup = BeautifulSoup(data.text,'lxml')
    return soup

def CurrDir():
     path = sys.path[0]
     #判断为脚本文件还是py2exe编译后的文件，如果是脚本文件，则返回的是脚本的目录，如果是py2exe编译后的文件，则返回的是编译后的文件路径
     if os.path.isdir(path):
         return path
     elif os.path.isfile(path):
         return os.path.dirname(path)

#get list and choose one of them
def InputChooice(utype, ulink):
    for y in xrange(0,len(utype)):
        print '分类有：' + utype[y]
        # print '链接为：' + ulink2[y]
    word = raw_input("根据以上分类输入你的选择:\n")
    try:
        for x in xrange(0,len(utype)):
            if word == utype[x]:
                print 'your choice is :' + ulink[x]
                print ulink[x].find('amp')
                if ulink[x].find('amp') < 0:
                    return ulink[x]
                else :
                    return ulink[x].replace('amp;','')
    except:
        print 'Invalid input !!!'
        exit()

#get page info which you are searching
def GetPageInfo(url):
    soup = GetHtml(url)
    num_total = int( re.findall('"pager":{"pageSize":[\d]*,"totalPage":(\d*),"currentPage":[\d]*,"totalCount"',str(soup))[0] )
    num_now = int( re.findall('"pager":{"pageSize":[\d]*,"totalPage":[\d]*,"currentPage":(\d*),"totalCount"',str(soup))[0] )
    num_size = int( re.findall('"pager":{"pageSize":(\d*),"totalPage":[\d]*,"currentPage":[\d]*,"totalCount"',str(soup))[0] )
    print num_size,num_total,num_now
    return num_size,num_total,num_now

#return next page url
def GetNextPage(url, pageSize):
    page_index = url.find('bcoffset=12&s=')
    if page_index < 0:
        return url + '&bcoffset=12&s=0'
    else:
        page_num = re.search('bcoffset=12&s=(\d+)',url)
        return url[:page_index] + 'bcoffset=12&s=' + str(int(page_num.group(1)) + pageSize)

def DownloadImage(name,imageUrl):
    dir = CurrDir() + '/taobao_spider_pic'
    try:  
        if not os.path.exists(dir):  
            os.mkdir(dir)  
    except:  
        print "Failed to create directory in %s"%dir  
        exit()  
    # image = imageUrl.split('/')[-1]
    image = name + '.jpg'
    path = dir+"/"+image
    data = urllib.urlopen(imageUrl).read()  
    f = file(path,"wb")
    f.write(data)
    f.close()

#input pic url, and get the picture
def GetPic(url):
    soup = GetHtml(url)
    g_page_config = re.findall('{"pageName":"mainsrp",(.*)}',str(soup))[0]
    # print g_page_config
    pic_num = re.findall('raw_title',g_page_config)
    print 'this page can get ' + str(len(pic_num)) + " picture !"
    for x in xrange(0,len(pic_num)-1):
        a = g_page_config.find('raw_title')
        b = g_page_config.find('detail_url')
        temp = g_page_config[a:b+6]
        g_page_config = g_page_config[b+6:]
        pic_name = re.findall('raw_title":"(.*)","pic_url"',temp)[0]
        pic_link = re.findall('pic_url":"(.*)","detail',temp)[0]
        if pic_link.find('http') < 0:
            pic_link = pic_link.replace('//','https://')
        DownloadImage(pic_name, pic_link)

if __name__ == "__main__":
    url = 'https://s.taobao.com/'

    ############ get what you want to search, and directly get picture 
    word = raw_input("你想搜索什么？\n")
    word = word.encode('utf8')
    sear = {'search':word}
    link_part = urllib.urlencode(sear)
    link = url + link_part
    link = link[:27] + '?q' + link[27:]
    nsize,ntotal,nnow = GetPageInfo(link)
    num_search = int(raw_input("你想获取多少页的图片？\n"))
    if num_search > ntotal:
        num_search = ntotal
    print '你能获取' + str(num_search) + '页的图片'
    for x in xrange(0, num_search):
        link = GetNextPage(link, nsize)
        GetPic(link)
