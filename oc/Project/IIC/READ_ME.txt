iic分为两个部分：
	check in和check out
	其各自工作流见图片1

数据格式，bu1雷工定义的，下发命令“BUCK\r”，返回读取epprom内所有的信息，格式可参见“IICCompare内的数据结构”

本次小工具所需要读取的地址见图片2；其中detail flag在图片2末尾，示例见图片3。

1、check in部分：

1.1、需读取0x85处的failure flag，调用了方法getBinaryByhex将16进制转为2进制。
1.2、确认时间是否大于两天，存储在epprom内的时间格式需问雷工，之前说要向客户确认。
1.3、将0x81处的flag设为1，显示sn+date+detail flag。
	设置flag，写入epprom的通讯协议见Protocol For Buck EEPROM A1.0
	这里要和雷工商量下，能否单独写某一个地址。WE会改写所有的buck内epprom的地址内容，现在这样做太麻烦。
	查询detail flag已截取放在iicreset内试用过。


2、check out部分：
2。1、read sn可直接用以前的代码，没有改变，check flag是check0x81处的maintenance flag。
2.2、check share folder内的测试结果，需要问“曾春辉”相关信息
2.3、reset部分，主要问题是we写入，最好是能单独写某一个epprom地址。
	将reset三个部分：a、maintenance flag；b、detail flag；c、detail flag对应部件的cycletime