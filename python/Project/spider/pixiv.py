#coding:utf-8
import random
from bs4 import BeautifulSoup
import re
import os
import lxml
import requests
from lxml import etree

import json
import logging
import cmdtree as cmd
from time import sleep
from functools import wraps
# from tqdm import tqdm
from pprint import pprint
from threading import Thread

import sys
reload(sys)
sys.setdefaultencoding('utf8')
_DEBUG = False
_pixiv = None

_ip = [
    '219.131.237.60',
    # '113.76.222.180',
    '36.23.107.234',
    '192.168.1.83',
]


def curr_dir():
     path = sys.path[0]
     # judge script file or py2exe compiled file
     if os.path.isdir(path):
         return path
     elif os.path.isfile(path):
         return os.path.dirname(path)

def store_folder(path):
    try:  
        if not os.path.exists(path):  
            os.mkdir(path)
    except:  
        print "Failed to create directory in %s"%path  
        exit() 

#get list and choose one of them
def input_chooice(utype, ulink):
    for y in xrange(0,len(utype)):
        print 'your collection list :' + utype[y]
        # print '链接为：' + ulink2[y]
    word = raw_input("which would you want to download:\n")
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

def retry(max_retries=3):
    def wrapper(func):
        @wraps(func)
        def wrapped(*args, **kwargs):
            retries = 0
            while retries <= max_retries:
                retries += 1
                try:
                    return func(*args, **kwargs)
                except Exception:
                    if retries > max_retries:
                        logging.exception("Error occurs while execute function\n")
                        break
                    sleep(1)
            return None
        return wrapped

    return wrapper

@retry()
def do_request(session, url, payload, header):
    # print header
    response = session.post(url, headers=header, data=payload, timeout=2)  #OK
    # response = getattr(requests, 'post')(url, timeout=(2, 10),data=payload ,headers=XHR_HEADERS)
    if _DEBUG:
        print("%s %s" % ('post', str(response)))
    return response

@retry()
def get_page(session, url, header):
    proxy = {'http':random.choice(_ip)}
    try:
        page = session.get(url, headers = header, proxies=proxy, timeout = 3).content
        soup = BeautifulSoup(page,'lxml')
        return soup
    except Exception, e:
        print e
        pass

@retry()
def get_pic(session, url, header):
    proxy = {'http':random.choice(_ip)}
    print proxy
    try:
        page = session.get(url, headers = header, proxies=proxy, timeout = 3).content
        return page
    except Exception, e:
        print e
        pass

class Pixiv(object):
    def __init__(self, pixiv_id, password):
        self.session = requests.Session()
        self.url_store = 'https://www.pixiv.net/bookmark.php'
        self.num_page = 1
        # self.url_login = 'https://accounts.pixiv.net/login?lang=zh&source=pc&view_type=page'
        self.url_login = 'https://accounts.pixiv.net/login?return_to=0&lang=zh&source=pc&view_type=page'
        self.payload = {}
        self.payload['pixiv_id'] = pixiv_id
        self.payload['password'] = password
        self.payload['return_to'] = 'http://www.pixiv.net/'
        agent = [
            'Mozilla/5.0 (Macintosh; Intel Mac OS X 10.12; rv:53.0) Gecko/20100101 Firefox/53',
            # 'Mozilla/5.0 (Windows NT 5.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/49.0.2623.22 Safari/537.36 SE 2.X MetaSr 1.0',
        ]
        self.header = {
            # "X-Requested-With": "XMLHttpRequest",
            'User-Agent':random.choice(agent)
        }
        self.path_store = str(curr_dir())+'/Pixiv'
        store_folder(self.path_store)

    def login(self):
        self.header['Referer'] = self.url_login
        login_page = get_page(self.session, self.url_login, self.header)
        # reg = re.compile(r'<input type="hidden" name="post_key" value="(.+?)">')
        # post_key = re.findall(reg, login_page)[0]
        post_key = login_page.find('input')['value']
        self.payload['post_key'] = post_key
        if _DEBUG:
            print self.payload
            print self.header
        resp = do_request(self.session, self.url_login, self.payload, self.header)
        print resp

    def download_image(self, name, imageUrl):
        image = name + '.jpg'
        ppath = self.path_store +"/"+image
        # data = self.session.get(imageUrl, headers=self.header, proxies=proxy, timeout = 3).content
        data = get_pic(self.session, imageUrl, self.header)
        f = file(ppath,"wb")
        f.write(data)
        f.close()

    def get_next_html(self,url):
        soup = get_page(self.session, url, self.header)
        url_next = soup.find_all('span', attrs={'class','next'})
        href = url_next[0].find_all('a')[0]['href']
        print href
        if not href:
            return False
        else:
            self.num_page = self.num_page + 1
            return url[:-1] + str(self.num_page)

    def get_pic_url(self,url):
        soup = get_page(self.session, url, self.header)
        pic_list = soup.find_all('div', attrs={'class', 'works_display'})
        pic_url = str(pic_list[0].find_all('img')[0]['src'])
        pic_name = str(pic_list[0].find_all('img')[0]['alt'])
        self.download_image(pic_name, pic_url)

    def get_pic_html(self,url):
        soup = get_page(self.session, url, self.header)
        li_list = soup.find_all('li', attrs={'class', 'image-item'})
        for child in li_list:
            pic_html = 'https://www.pixiv.net/' + str(child.find_all('a')[0]['href'])
            self.get_pic_url(pic_html)
            self.header['Referer'] = pic_html

    def work(self):
        self.login()

        #get search html
        soup = get_page(self.session, self.url_store, self.header)
        store_list_str = soup.find('ul', attrs={'class', 'tagCloud'})
        store_list = store_list_str.find_all('a')
        clist_name = []
        clist_href = []
        for child in store_list :
            clist_name.append(str(child.text))
            clist_href.append(str(child['href']).strip('/bookmark.php?'))
            print '[Store List]:'+str(child.text)
        clist_name_num = int(raw_input("<choose the list num>\n<range = 0~"+str(len(clist_name)) +'>\n' )) - 1
        print 'you choose the class = '+ clist_name[clist_name_num]
        clist_name_cla = raw_input("<input show/hide to choose show/hide type>\n")
        url_search_start = 'https://www.pixiv.net/bookmark.php?rest=' + clist_name_cla + '&' + clist_href[clist_name_num] + '&p=1'
        print '《searching》:' + url_search_start

        self.path_store = self.path_store + '/' + clist_name[clist_name_num]
        store_folder(self.path_store)
        print '<store in>:'+self.path_store

        #get search picture html
        self.get_pic_html(url_search_start)     #first
        page_next = self.get_next_html(url_search_start)
        while page_next :
            print '《searching》:' + page_next
            self.get_pic_html(page_next)
            page_next = self.get_next_html(page_next)
        print '《searching over》'


class Worker(Thread):
    def __init__(
            self, queue, target
    ):
        """
        :type queue: Queue.Queue
        """
        super(Worker, self).__init__()
        self.task_func = target
        self.queue = queue
        self.daemon = True
        self._stopped = False

    def run(self):
        while not self._stopped:
            try:
                task = self.queue.get(timeout=60)
            except Queue.Empty:
                break
            else:
                self.task_func(*task)

    def stop(self):
        self._stopped = True


def start_download(pixiv_id, password):
    _pixiv = Pixiv(pixiv_id,password)
    _pixiv.work()


@cmd.argument("password")
@cmd.argument("pixiv_id")
@cmd.option("d", is_flag=True, default=False)
@cmd.command("download",help="example:pyhton pixiv.py download username,password ")
def download(pixiv_id, password, d):
    global _DEBUG
    _DEBUG = d
    start_download(pixiv_id, password)

if __name__ == "__main__":
    cmd.entry()
    # soup = GetHtml(url)
    # print soup



