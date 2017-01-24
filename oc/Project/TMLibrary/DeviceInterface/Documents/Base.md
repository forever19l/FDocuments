# 通用设备接口库

[TOC]

*Version History*

```
V1.1
Author:--
Date:---
Change List:
* TBD

V1.0
Author:Ryan
Date:2017-01-20
Change List:
* 初稿

```

##BaseClass
###CPublisher
消息广播基础类，用来推送设备模型消息

###CDevInterface
*设备接口基础类，所有的设备模型需要从该类继承。*
####Open(char * arg);
*打开一个设备*
####Close()=0;
*关闭一个设备*
####WriteString(const char * cmd);
*写一个字符串到设备*
####ReadString();
*读设备目前的所有的输出缓冲*
####ClearBuffer();
*清空当前设备的缓冲*
####Write(void * buffer,unsigned long len);
*写一段数据到设备*
####Read(void * buffer,int len);
*读设备输出缓冲*
####SetDetectString(const char * regex,int flag = 0);
*设置需要检测设备输出缓冲的字符*
####WaitForString(int iTimeOut=30*1000);
*等待已经设备输出缓冲里出现被设置的监测字符*




