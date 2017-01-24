//
//  HalconLib.h
//  HalconLib
//
//  Created by FoLeakey on 7/8/16.
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
    
    /*!
     *	@brief Set image files root path
     *	@param pPath	Image files root path
     */
    void setRootPath(const char *pPath);
    /*!
     *	@brief Set a view to display image
     *	@param View     A NSView
     */
    void initWindow(NSView *View);
    /*!
     *	@brief Display a image in view
     *	@param pFilePath	Image path
     */
    void displayPic(const char *pFilePath);
    /*!
     *	@brief Display a image in view
     *	@param pImage	Image ptr
     */
    void displayPic(HImage pImage);
    /*!
     *	@brief Train shape model with image in view
     *	@return Trained?
     */
    BOOL trainModel();
    /*!
     *	@brief Train shape model with selected image
     *	@param pImage	Image ptr
     *	@return Trained?
     */
    BOOL trainModel(HImage pImage);
    /*!
     *	@brief Train shape model with selected image
     *	@param pImage	Image ptr
     *	@param pIndex	Region index
     *	@param isDraw	Draw region manually/automatic
     *	@param isSave	Save drew region or not
     *	@return Trained?
     */
    BOOL trainModel(HImage pImage,int pIndex,BOOL isDraw=TRUE,BOOL isSave=TRUE);
    /*!
     *	@brief Train shape model with selected image and set its name
     *	@param pImage	Image ptr
     *	@param isDraw	Draw region manually/automatic
     *	@param isSave	Save drew region or not
     *	@param pRegion	Image name
     *	@param pIndex	Region index
     *	@return Trained?
     */
    BOOL trainModel(HImage pImage,BOOL isDraw,BOOL isSave, const char *pRegion,int pIndex);
    /*!
     *	@brief Save trained model in local
     *	@param pRegion	Model name
     *	@param pIndex	Model index
     *	@return Saved?
     */
    BOOL saveModel(const char *pRegion,int pIndex);
    /*!
     *	@brief Test saved region data
     *	@param pFilePaht	Region data path
     *	@param index		Region data index
     */
    void testRegion(const char *pFilePaht,int index);
    /*!
     *	@brief Test all model with selected image
     *	@param pImage	Sample image
     *	@param pRegion	Sample's name
     *	@param pIndex	Sample's index
     *	@return Test done?
     */
    BOOL modelTest(HImage pImage, const char *pRegion,int pIndex);
    /*!
     *	@brief Add a class name in vec
     *	@param pRegion	Class name
     */
    void addRegion(const char *pRegion);
    /*!
     *	@brief Display an image in view via its class name and index
     *	@param pImgIndex	Image's class name
     *	@param pIndex		Image's index
     */
    void showResult(int pImgIndex,int pIndex);
    /*!
     *	@brief Input a sample and its class name and index, output its detected score
     *	@param pImage	Sample image ptr
     *	@param pRegion	Sample's class name
     *	@param pIndex	Sample's index
     *	@return Detected score
     */
    HTuple detectSample(HImage pImage,const char *pRegion,int pIndex);
    /*!
     *	@brief Input a sample and its class name and index, output its detected score
     *	@param pImage		Input a sample and its class name and index, output its detected score
     *	@param pRegion		Sample image ptr
     *	@param pIndex		Sample's class name
     *	@param hv_Row		Detected row
     *	@param hv_Column	Detected col
     *	@param hv_Angle     Detected angle
     *	@param hv_Scale     Detected scale
     *	@param hv_I			Detected index
     *	@return Detected score
     */
    HTuple detectSample(HImage pImage,const char *pRegion,int pIndex,HTuple *hv_Row,HTuple *hv_Column,HTuple *hv_Angle,HTuple *hv_Scale,HTuple *hv_I);
private:
    /*!
     *	@brief Model root model
     */
    char sRootPath[255];
    /*!
     *	@brief Window handle to display image
     */
    HTuple WinHandle;
    /*!
     *	@brief Longlive image ptr
     */
    HImage sImage;
    /*!
     *	@brief Model data
     */
    HTuple TemplateModel;
    /*!
     *	@brief Store region info
     */
    vector<string>sRegion;
    /*!
     *	@brief Stroe detected result image
     */
    vector<HObject>sResult;
};
/*!
 *	@brief Save a debug log
 *	@param msg	Log to be saved
 */
void SaveDebug(const char *msg);
/*!
 *	@brief Check whthin the path exist or not, and create if path is not exist
 *	@param pFilePath	Path to be checked
 */
void checkPath(char *pFilePath);