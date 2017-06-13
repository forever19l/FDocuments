#-------------------------------------------------
#
# Project created by QtCreator 2016-08-31T19:39:38
#
#-------------------------------------------------

QT       -= core gui

TARGET = CommonLib
TEMPLATE = lib

DEFINES += COMMONLIB_LIBRARY
CONFIG += staticlib c++11


INCLUDEPATH += $$PWD/../Contrib/Mac/redis-3.0.7/deps/hiredis \
            /usr/local/include \

CONFIG +=debug_and_release
CONFIG(debug,debug|release){
DESTDIR = $$PWD/../TestManager/bin/Debug
}else{
DESTDIR = $$PWD/../TestManager/bin/Release
}

LIBS += $$PWD/../Contrib/Mac/redis-3.0.7/deps/hiredis/libhiredis.a \
        /usr/local/lib/liblog4cplus.dylib \
        /usr/local/lib/libzmq.dylib \

SOURCES += \
    ConvertHelper.cpp \
    CRedis.cpp \
    JsonHelper.cpp \
    Util.cpp \
    XmlHelper.cpp \
    ZmqReplyer.cpp \
    ZmqRequester.cpp \
    ZmqSubscriber.cpp \
    CTcpClient.cpp \
    CTcpServer.cpp \
    Exception.cpp \
    ExcuteProcess.cpp \
    FileUtil.cpp \
    ZmqPublisher.cpp \
    SerialPort.cpp

HEADERS += \
    ConvertHelper.h \
    CRedis.h \
    GuiCallFunctionDef.h \
    JsonHelper.h \
    LogHelper.h \
    Message.h \
    Mgr.h \
    AutoMachineCommandDefine.h \
    SceneNode.h \
    Util.h \
    XmlHelper.h \
    rapidxml/rapidxml.hpp \
    rapidxml/rapidxml_iterators.hpp \
    rapidxml/rapidxml_print.hpp \
    rapidxml/rapidxml_utils.hpp \
    ZmqReplyer.h \
    ZmqRequester.h \
    ZmqSubscriber.h \
    CTcpClient.h \
    CTcpServer.h \
    Exception.h \
    ExcuteProcess.h \
    FileUtil.h \
    ZmqPublisher.h \
    SerialPort.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

