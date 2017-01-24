//
//  CIANetFixture.h
//  IANetFixture
//
//  Created by Louis on 13-11-13.
//  Copyright (c) 2013年 Louis. All rights reserved.
//
/*给lua的接口类*/
#ifndef __IANetFixture__CIANetFixture__
#define __IANetFixture__CIANetFixture__

#include <iostream>
#include "tolua++.h"
#import <Foundation/Foundation.h>
#import <Foundation/Foundation.h>
#import "PLCCmd.h"
#import "LoadCell.h"
#import "PLCData.h"
#import "struct.h"
#import "AppDelegate.h"
#import "RunHalcon.h"
#import "SoapDelegate.h"
#import "PLCAct.h"

#define TESTPOSITION @"/vault/Intelli_PM/Config/TestKeyIndex.plist"             //标准伺服坐标
#define CAMERAPOSITION @"/vault/Intelli_PM/Config/CameraKeyIndex.plist"         //拍照位置
#define CXYCAMERAPOSITION @"/vault/Intelli_PM/Config/RotateCoorder.plist"       //画圆的绝对坐标
#define CalCAMERAPOSITION @"/vault/Intelli_PM/Config/ABcoorder.plist"           //绝对打键坐标


#define kNotificationDoTestStart        @"DO_TestStart"
#define kNotificationDoTestStop         @"DO_TestStop"
#define kNotificationDoUiCtrl           @"Do_UiCtrl"
#define kNotificationOnTestError        @"On_TestError"



class CIANetFixture {
public:
    CIANetFixture();
    ~CIANetFixture();
    double PLC_X = 0;
    double PLC_Y = 0;

    PLCData * mm_plcdata;
    LoadCell * mm_loadcell;
    PLCAct *mm_plcact;
    g_SendBeckHoff sendbuffer;
    //    g_ReadBeckHoff readcmd;   暂只有plcdata read markpoint使用，暂取消

public:
    int getTestPlatNum();
    int getTestDutNum(int plat);
    
    // halcon camera
    long GrabOnce();
    int CalculateServoPoint(int platnum,int dutnum,int modelname);
    float CalTestPointX();
    float CalTestPointY();
    int GetDUTTypeFromCamera(int PlatNum,int HandNum,int modelname);
//    float CalTestPointX(int PlatNum,int HandNum,int modelname,const char *m_path);
//    float CalTestPointY(int PlatNum,int HandNum,int modelname,const char *m_path);

    //get config coorder        *****y取消，y1y2坐标系需用同一套****
    void ReadPosition();
    double getTestPoint(int platnum,int dutnum,string key,string xyz);      //伺服坐标
    double getTestPointcal(int platnum,int dutnum,string key,string xyz);   //绝对打键坐标
    double getRotatePoint(int platnum,int dutnum,int key);                  //画圆坐标
    double getCameraPoint(int plantnum,int dutnum,string key,string xyz);   //绝对拍照坐标0-5LL;6-11LR;12-17RL;18-23RR

    //数据处理
    void ClearLoadData();
    double getLoadCellMarketData(int Index);
    int ShowJoystick();

    //动作命令，需修改
    void AppendCmdPlc(ushort cmd_count, ushort cmd_type, ushort cmd_id, ushort cmd_v1, ushort cmd_v2);
    long SendCmdPLC();
    long SendCMDJoystick(int downORrotate ,ushort d1,ushort d2);
    long SendCmdLoadcell(int KeyIndex,char* localCmd,int PlcCmdTimeout);

    //无用int CalPLCData(int PlatNum,int HandNum);
    //double getPLCMarketData(int Index);   //画圆mark点，尚未定义 
    
    //compress zip
    int ZipRawdata(const char* zippathfile);    //暂不使用，保留
    int ClearTmpDatafile();                     //暂不使用，保留

    //for shop flow
    int SF_Login();
    int SF_CheckRoute(string web, string sn);
    int SF_SaveJson(string value,string key,string name);
    int SF_SendJson(string web ,string sn,string date,int result);
//    void xmltest(); //for test
public:

    void Read();
    void Write();
    void CancelTest();
    void StartTest();

    NSMutableDictionary *dicConfiguration ;
    pthread_mutex_t mMutexLock ;
    

#pragma mark Camera COMMADN Fonution
public:
    void Init();
    

private:
    int CreatePath(NSString* path);
    int m_PlatNum;
    int m_DutNum;
    NSMutableArray *Position_items_test;
    NSMutableArray *Position_items_camera;
    NSMutableArray *Position_items_cxy;
    NSMutableArray *Position_items_cal;
    NSMutableDictionary *dic_JSON;
    NSMutableDictionary *test_content;
public:
    
    
};

#endif /* defined(__IANetFixture__CIANetFixture__) */
