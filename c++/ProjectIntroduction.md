##2017.1.4    v1.0.0
- [DllDemo](http://blog.csdn.net/forever917/article/details/53780543.) is a c++ domo for dll. 
  - CameraDLL is a sample demo for c++ to make dll.
  - CameraTest2 is a demo, how to import dll and make lua interface.
- TMC is a Test Model Controller . It’s using log4 to print and save log , search redis to init model , communicate with zmq to decide weather change MTMS(another exe for client) model.

##2017.3.10	v1.0.1
- TMC Update
  - 1.modify xml.
  - 2.modify mtms launsh model.
  - 3.fix csv path bug
  - 4.check if XmlNode is valid before get value
  - 5.add log file path
  - 6.handle Console close event to delete zmq object