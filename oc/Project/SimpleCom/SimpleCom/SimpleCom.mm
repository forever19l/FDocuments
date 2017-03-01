//
//  SimpleCom.cpp
//  SimpleCom
//
//  Created by Liang on 15-1-8.
//  Copyright (c) 2015年 Liang. All rights reserved.
//

#include "SimpleCom.h"
#import "SimpleComWinDelegate.h"
#import "SerialportFactory.h"
#import <CoreLib/TestEngine.h>
#import <CoreLib/SerialportEx.h>
#import <CoreLib/ScriptEngine.h>
#import <CoreLib/SerialPortView.h>
#import "SocketFactory.h"
#import <CoreLib/Common.h>


void SimpleComLog(const char* msg);

extern SimpleComWinDelegate* SimpleComWinDelegateObj;


CSimpleCom::CSimpleCom()
{
    m_DicSimpleComs = [[NSMutableDictionary alloc] init];
}
CSimpleCom::~CSimpleCom()
{
    if (m_DicSimpleComs) {
        [m_DicSimpleComs release];
        m_DicSimpleComs = nil;
    }
}
SimpleComFactory *CSimpleCom::getInterface(NSString *Interfacename)
{
    return [m_DicSimpleComs objectForKey:Interfacename];
}
int CSimpleCom::Refresh(NSString *Interfacename)
{
    SerialportFactory* mDUTComm=[m_DicSimpleComs objectForKey:Interfacename];
    [mDUTComm OpenSimpleCom];
    return 0;
}
int CSimpleCom::CheckInterface(NSString *Interfacename)
{
    for (NSString *key in [m_DicSimpleComs allKeys])
    {
        if ([key isEqualToString:Interfacename]) {
            return (int)[m_DicSimpleComs count];
        }
    }
    return 0;
}
NSString* CSimpleCom::CheckResource(NSString *Interfacename)
{
    for (NSString *key in [m_DicSimpleComs allKeys])
    {
        id obj=[m_DicSimpleComs objectForKey:key];
        if ([obj isKindOfClass:[SerialportFactory class]]) {
            if ([obj CheckResourceName:Interfacename]) {
                return key;
            }
        }
    }
    return nil;
}
int CSimpleCom::Create(SimpleComSetting* simpleComSetting)//"resource_name;setting"
{
    for (NSString *key in [m_DicSimpleComs allKeys])
    {
        if ([key isEqualToString:[simpleComSetting getInterfaceName]]) {
            return (int)[m_DicSimpleComs count];
        }
    }
    if ([simpleComSetting getType] == eIITSerial)
    {
        
        NSString* strInstrName = [simpleComSetting getSetting];
        NSArray* arr=[strInstrName componentsSeparatedByString:@";"];
        if ([arr count]<2) {
            return -1;
        }
        SerialportFactory* mDUTComm = [[SerialportFactory alloc]initwithName:simpleComSetting];
        if (mDUTComm)
        {
            [mDUTComm OpenSimpleCom];
            [m_DicSimpleComs setObject:mDUTComm forKey:[simpleComSetting getInterfaceName]];
            
        }
    }
    else if ([simpleComSetting getType] == eIITGpib)
    {
        
    }
    else if ([simpleComSetting getType] == eIITLan)
    {
        
        NSString* strInstrName = [simpleComSetting getSetting];
        NSArray* arr=[strInstrName componentsSeparatedByString:@";"];
        if ([arr count]<2) {
            return -1;
        }
        SocketFactory* mDUTComm = [[SocketFactory alloc]initwithName:simpleComSetting];
        if (mDUTComm)
        {
            [mDUTComm OpenSimpleCom];
            [m_DicSimpleComs setObject:mDUTComm forKey:[simpleComSetting getInterfaceName]];
            
        }
    }
    else if ([simpleComSetting getType] == eIITUsb)
    {
        
    }
    else if ([simpleComSetting getType] == eIITNa)
    {
//        NSString* strInstrName = [NSString stringWithUTF8String:name];
//        NSString* strSetting = [NSString stringWithUTF8String:szSetting];
//        if(strInstrName && strSetting)
//        {
//
//        }
//        else{
//            NSLog(@"Incorrect args for creating simplecom.");
//        }

    }
    return (int)[m_DicSimpleComs count];
}
bool CSimpleCom::WriteString(const char*szDeviceName,const char *szCMD)
{
    if (szDeviceName==NULL || szCMD==NULL) {
        return NO;
    }
    NSString* strInstrName = [NSString stringWithUTF8String:szDeviceName];
    id simple=[m_DicSimpleComs objectForKey:strInstrName];
    if (simple) {
        return [simple WriteString:szCMD];
    }
    return NO;
}
void CSimpleCom::AddLabelString(const char*szDeviceName,const char*LabelStr)
{
    if (szDeviceName==NULL || LabelStr==NULL) {
        return;
    }
    NSString* strInstrName = [NSString stringWithUTF8String:szDeviceName];
    id simple=[m_DicSimpleComs objectForKey:strInstrName];
    if (simple)
    {
        [simple AddLabelString:LabelStr];
    }
}
const char* CSimpleCom::ReadStringRegex(const char*szDeviceName,const char*Regex,int timeout)
{
    if (szDeviceName==NULL) {
        return nil;
    }
    NSString* strInstrName = [NSString stringWithUTF8String:szDeviceName];
    id simple=[m_DicSimpleComs objectForKey:strInstrName];
    if (simple) {
        try {
            return [simple ReadStringRegex:Regex Timeout:timeout];
        } catch (...)
        {
            NSLog(@"Regex error\n");
        }
    }
    return nil;
}
const char* CSimpleCom::ReadString(const char*szDeviceName)
{
    if (szDeviceName==NULL) {
        return nil;
    }
    NSString* strInstrName = [NSString stringWithUTF8String:szDeviceName];
    id simple=[m_DicSimpleComs objectForKey:strInstrName];
    if (simple) {
        return [simple ReadString];
    }
    return nil;
}
int CSimpleCom::SetDetectString(const char*szDeviceName,const char *regex)
{
    if (szDeviceName==NULL) {
        return -1;
    }
    NSString* strInstrName = [NSString stringWithUTF8String:szDeviceName];
    id simple=[m_DicSimpleComs objectForKey:strInstrName];
    if (simple) {
        return [simple SetDetectString:regex];
    }
    return -1;
}
int CSimpleCom::WaitForString(const char*szDeviceName,int iTimeout)
{
    if (szDeviceName==NULL) {
        return -1;
    }
    NSString* strInstrName = [NSString stringWithUTF8String:szDeviceName];
    id simple=[m_DicSimpleComs objectForKey:strInstrName];
    if (simple) {
        return [simple WaitForString:iTimeout];
    }
    return -1;
}

void CSimpleCom::ClearBuffer(char*szDeviceName)
{
    if (szDeviceName == NULL) {
        return;
    }
    NSString* strInstrName = [NSString stringWithUTF8String:szDeviceName];
    id simple=[m_DicSimpleComs objectForKey:strInstrName];
    if (simple) {
        [simple ClearBuffer];
    }

}
const char* CSimpleCom::GetDeviceName(int index)
{
    NSArray* arrAllkeys = [m_DicSimpleComs allKeys];
    if (arrAllkeys == nil) {
        return NULL;
    }
    if ([arrAllkeys count]<=index) {
        return NULL;
    }
    return [[arrAllkeys objectAtIndex:index] UTF8String];
}
NSUInteger CSimpleCom::GetDeviceCount()
{
    return [m_DicSimpleComs count];
}
char* CSimpleCom::GetDeviceNames()
{
    NSString* strMsg = [NSString stringWithFormat:@"Current simplecom count = %d", (int)[m_DicSimpleComs count]];
    SimpleComLog([strMsg UTF8String]);
    
    NSArray* arrAllkeys = [m_DicSimpleComs allKeys];
    if (arrAllkeys == nil) {
        return NULL;
    }
    NSString* strDevNames = [arrAllkeys componentsJoinedByString:@","];
    if ([strDevNames length] == 0) {
        return NULL;
    }
    
    return (char*)[strDevNames UTF8String];
}

void CSimpleCom::Remove(char* name)
{
    NSString* strInstrName = [NSString stringWithUTF8String:name];
    id simple=[m_DicSimpleComs objectForKey:strInstrName];
    if (simple)
    {
        [m_DicSimpleComs removeObjectForKey:strInstrName];
        [simple closeSimpleCom];
        [simple release];
    }
}

void CSimpleCom::RemoveAll()
{
    NSArray* arrAllKey = [m_DicSimpleComs allKeys];
    for (int i=(int)[arrAllKey count]-1; i>=0; i--)
    {
        id simple=[m_DicSimpleComs objectForKey:[arrAllKey objectAtIndex:i]];
        if (simple)
        {
            [m_DicSimpleComs removeObjectForKey:[arrAllKey objectAtIndex:i]];
            [simple closeSimpleCom];
            [simple release];
        }
    }
}

void SimpleComLog(const char* msg)
{
    NSString * strLog = [NSString stringWithFormat:@"SimpleComs: %s", msg];
    [SimpleComWinDelegateObj PrintLog:strLog];
    
}

