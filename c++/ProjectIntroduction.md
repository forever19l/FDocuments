##2017.1.4
- [DllDemo](http://blog.csdn.net/forever917/article/details/53780543.) is a c++ domo for dll. 
  - CameraDLL is a sample demo for c++ to make dll.
  - CameraTest2 is a demo, how to import dll and make lua interface.
- TMC is a Test Model Controller . It’s using log4 to print and save log , search redis to init model , communicate with zmq to decide weather change MTMS(another exe for client) model.

##2017.2.10	
- TMC Update
  - 1.modify xml.
  - 2.modify mtms launsh model.
  - 3.fix csv path bug
  - 4.check if XmlNode is valid before get value
  - 5.add log file path
  - 6.handle Console close event to delete zmq object

##2017.2.27
- Add BeckHoff project, it's a TCP client and import lua interface   

##2017.2.28
- change BeckHoff to BeckHoffDll
	- BeckHoff can't make release version, caused by the stdafx.
	- The old BeckHoff.dll need the msvcr120d.dll and msvcp120d in SysWOW64, because it's debug version.  

##2017.6.13
- add CommonLib
