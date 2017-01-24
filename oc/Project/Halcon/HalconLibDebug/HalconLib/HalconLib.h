//
//  HalconLib.h
//  HalconLib
//
//  Created by FoLeakey on 6/28/16.
//  Copyright (c) 2016 IntelligentGroup. All rights reserved.
//
#include "HalconCpp.h"
#import <Cocoa/Cocoa.h>
#include <vector>
#include <iostream>
#include <fstream>

using namespace HalconCpp;
using namespace std;

class HalconLib{
public:
    HalconLib();
    ~HalconLib();
    void setRootPath(const char *pPath);
    void initWindow(NSView *View);
    void displayPic(const char *pFilePath);
    void displayPic(HImage pImage);
    BOOL trainModel();
    BOOL trainModel(HImage pImage);
    BOOL trainModel(HImage pImage,int pIndex,BOOL isDraw=TRUE,BOOL isSave=TRUE);
    BOOL trainModel(HImage pImage,BOOL isDraw,BOOL isSave, const char *pRegion,int pIndex);
    BOOL saveModel(const char *pRegion,int pIndex);
    void testRegion(const char *pFilePaht,int index);
    BOOL modelTest(HImage pImage, const char *pRegion,int pIndex);
    void addRegion(const char *pRegion);
    void showResult(int pImgIndex,int pIndex);
    HTuple detectSample(HImage pImage,const char *pRegion,int pIndex);
    HTuple detectSample(HImage pImage,const char *pRegion,int pIndex,HTuple *hv_Row,HTuple *hv_Column,HTuple *hv_Angle,HTuple *hv_Scale,HTuple *hv_I);
private:
    char sRootPath[255];
    HTuple WinHandle;
    HImage sImage;
    HTuple TemplateModel;
    vector<string>sRegion;
    vector<HObject>sResult;
};
void SaveDebug(const char *msg);
void checkPath(char *pFilePath);