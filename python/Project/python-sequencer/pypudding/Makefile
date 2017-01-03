SWIG       = swig
SRCS       = 
TARGET_IP  = pypudding_w
CFLAGS = -arch i386 -arch x86_64
LDFLAGS = 


all::
	$(MAKE) -f Makefile_SWIG SRCS='$(SRCS)' CFLAGS='$(CFLAGS)' SWIG='$(SWIG)' \
	TARGET='$(TARGET_IP)' INTERFACE='pypudding_w.i' LIBS='-L/usr/local/lib -lInstantPudding' python_cpp 


clean::
	$(MAKE) -f Makefile_SWIG python_clean
	rm -f $(TARGET_IP).py

