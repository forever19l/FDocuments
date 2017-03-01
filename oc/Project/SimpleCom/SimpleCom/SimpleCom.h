//
//  SimpleCom.h
//  SimpleCom
//
//  Created by Liang on 15-1-8.
//  Copyright (c) 2015年 Liang. All rights reserved.
//
#import "SimpleComModule.h"
#import "SimpleComFactory.h"


class CSimpleCom{
public:
    CSimpleCom();
    ~CSimpleCom();
public:
    SimpleComFactory *getInterface(NSString *Interfacename);
    NSString* CheckResource(NSString *Interfacename);
    int CheckInterface(NSString *Interfacename);
    int Create(SimpleComSetting* simpleComSetting);
    int Refresh(NSString *name);
    void Remove(char* name);
    void RemoveAll();
    bool WriteString(const char*szDeviceName,const char *szCMD);
    void AddLabelString(const char*szDeviceName,const char*LabelStr);
    const char* ReadStringRegex(const char*szDeviceName,const char*Regex,int timeout);
    const char* ReadString(const char*szDeviceName);
    int SetDetectString(const char*szDeviceName,const char *regex);
    int WaitForString(const char*szDeviceName,int iTimeout);
    void ClearBuffer(char*szDeviceName);
    char* GetDeviceNames();
    const char* GetDeviceName(int index);
    NSUInteger GetDeviceCount();
protected:
    NSMutableDictionary* m_DicSimpleComs;
};
