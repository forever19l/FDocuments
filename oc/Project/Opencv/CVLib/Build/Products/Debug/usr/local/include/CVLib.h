//
//  CVLib.h
//  CVLib
//
//  Created by FoLeakey on 7/11/16.
//  Copyright (c) 2016 IntelligentGroup. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>

#include <cv.h>
#include <ml.h>
#include <math.h>
#include <highgui.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/stitching/stitcher.hpp"
#include <opencv2/opencv.hpp>
#include <imgproc/imgproc.hpp>

#define VERTICAL 1
#define HORIZONTAL 0

using namespace std;
using namespace cv;

#pragma mark---Common funcs
/*!
 *	@brief Resize pic into setted size
 *	@param FilePath_n	Pic path
 *	@param Size_n		Expectant height*width
 *	@return Resized pic
 */
IplImage *resizeImg(char *FilePath_n,CvSize Size_n);
/*!
 *	@brief Crop image from a ROI
 *	@param SrcImg	Image ptr
 *	@param Roi		ROI
 *	@return Croped image
 */
IplImage *cvGetSubImage(IplImage *SrcImg,CvRect Roi);
/*!
 *	@brief Sort algorithm
 *	@param a		Char array
 *	@param count	Lenth of char array
 */
void InsertSort(unsigned char a[],int count);

#pragma mark---Region funcs
/*!
 *	@brief Get a pic's min non-empty pixel region
 *	@param SrcImg	Image ptr
 *	@return Min non-empty pixel region
 */
CvRect findMinRect(IplImage *SrcImg);
/*!
 *	@brief Find the pixel region for a binary pic
 *	@param SrcImg	SourceImage
 *	@param x		X pos
 *	@param y		Y pos
 *	@param Flag		White/Black background,1 for white, 0 for black
 */
void findRegion(IplImage *SrcImg,int *x,int *y,int Flag=1);
/*!
 *	@brief Find the first and last effective pixel in image
 *	@param SrcImg	Image ptr
 *	@param Min		First X pos
 *	@param Max		Last X pos
 *	@param Flag		White/Black background,1 for white, 0 for black
 */
void findXRange(IplImage *SrcImg,int *Min,int *Max,int Flag=1);
/*!
 *	@brief Find the first and last effective pixel in image
 *	@param SrcImg	Image ptr
 *	@param Min		First Y pos
 *	@param Max		Last Y pos
 *	@param Flag		White/Black background,1 for white, 0 for black
 */
void findYRange(IplImage *SrcImg,int *Min,int *Max,int Flag=1);
/*!
 *	@brief Find the first and last effective pixel in image
 *	@param SrcImg	Image ptr
 *	@param Min		First X pos
 *	@param Max		Last X pos
 */
void findX(IplImage *SrcImg,int *Min,int *Max);
/*!
 *	@brief Find the first and last effective pixel in image
 *	@param SrcImg	Image ptr
 *	@param Min		First Y pos
 *	@param Max		Last Y pos
 */
void findY(IplImage *SrcImg,int *Min,int *Max);

/*!
 *	@brief Prepress a pic, get a min binary region. 1 for turn pixel into ~
 *	@param SrcImg	Image ptr
 *	@param W_n		Expectant width
 *	@param H_n		Expectant heigh
 *	@param Mode		White/Black background
 *	@return Processed image
 */
IplImage *prePrs(IplImage *SrcImg,int W_n, int H_n,int Mode=1);

/*!
 *	@brief Call a window and show a pic to ensure the threshold for binary a pic
 *	@param FilePath_n	Image path
 *	@param TA			Threshold A
 *	@param TB			Threshold B
 */
void getThreshold(char *FilePath_n,int *TA,int *TB);
/*!
 *	@brief Set threshold, also as a callback func
 *	@param Threshold	Threshold needed
 */
void min_Trackbar(int Threshold);
/*!
 *	@brief Set threshold, also as a callback func
 *	@param Threshold	Threshold needed
 */
void max_Trackbar(int Threshold);
/*!
 *	@brief Create a window to display image and modify threshold
 *	@param pSrcImg	Image ptr
 *	@param TA		Threshold A
 *	@param TB		Threshold B
 */
void getThreshold(IplImage *pSrcImg,int *TA,int *TB);
/*!
 *	@brief Create a window to display image and modify threshold
 *	@param pSrcImg  Image path
 *	@param TA		Threshold A
 *	@param TB		Threshold B
 */
void getThreshold(char *FilePath_n,int *TA,int *TB);
/*!
 *	@brief Display image in window with para threshold setted
 *	@param TA	Threshold A
 *	@param TB	Threshold B
 */
void showImg(int TA,int TB);

//Stitch a series pic, which must be related pics.
/*!
 *	@brief Stitching pics simply
 *	@param SrcImgQueen	Image ptrs array
 *	@param lenth		Sum width/heigh
 *	@param mode			Vertical/Horizontal, 1 for vertical, 0 for horizontal
 *	@return Stitched image
 */
IplImage *picStcer(IplImage *SrcImgQueen[],int lenth,int mode);
/*!
 *	@brief Stitching pics with feature matching
 *	@param SrcImgQueen	Image ptrs array
 *	@param lenth		Sum width/heigh
 *	@return Stitched image
 */
IplImage *autoStcer(IplImage *SrcImgQueen[],int lenth);
/*!
 *	@brief Stitching pics simply
 *	@param ImgA     Image ptr A
 *	@param ImgB     Image ptr B
 *	@param mode     Vertical/Horizontal, 1 for vertical, 0 for horizontal
 *	@return Stitched image
 */
IplImage *picStcer_sub(IplImage *ImgA,IplImage *ImgB,int mode);

//Simple Camera ctrl
class CamCtrl{
private:
    /*!
     *	@brief A ptr to save a frame
     */
    IplImage *pFrame;
    /*!
     *	@brief An video ptr to get video frame
     */
    CvCapture *pCapture;
public:
    CamCtrl();
    /*!
     *	@brief Open camera and display frame
     *	@param CamIndex	Camera index
     */
    void opnCam(int CamIndex=-1);
    
    /*!
     *	@brief Save frames in camera captureed frame
     *	@param pCapture_n	Video ptr
     *	@param FilePath_n	Save path
     */
    void saveVideo(CvCapture *pCapture_n,char *FilePath_n);
};

// k-Nearest-Neighbor, for simple object detect
class KNNClass{
public:
    KNNClass(int k_n=5,int SmpClass_n=5,int ClassQuty_n=200,int SmpSize_n=400);
    ~KNNClass();
    /*!
     *	@brief Set K number
     *	@param n	Number
     */
    void setK(int n);
    /*!
     *	@brief Set threshold
     *	@param n	Threshold
     */
    void setThresA(int n);
    /*!
     *	@brief Set threshold
     *	@param n	Threshold
     */
    void setThresB(int n);
    /*!
     *	@brief Set sample data's size
     *	@param n	Data size(width=heigh)
     */
    void setSmpSize(int n);
    /*!
     *	@brief Set sample data's classes
     *	@param n	Number of sample classes
     */
    void setSmpClass(int n);
    /*!
     *	@brief Set number of data in each class
     *	@param n	Number of data in each calss
     */
    void setClassQuty(int n);
    /*!
     *	@brief Set root folder path
     *	@param FilePath_n	Root folder path
     */
    void setFilePath(char *FilePath_n);
    
    /*!
     *	@brief Classify a pic
     *	@param FilePath_n	Pic path
     *	@return Class of pic
     */
    float classify(char *FilePath_n);
    /*!
     *	@brief System test
     *	@param FilePath_n	Root folder path
     *	@param TestClass	Number of sample classes
     *	@param Max			Number of data in each class
     */
    void selfTest(char *FilePath_n,int TestClass,int Max);
    /*!
     *	@brief Load local data
     *	@param FilePath_n	Data path
     *	@param SmpClass_n	Number of sample classes
     *	@param ClassQuty_n	Number of data in each calss
     */
	void loadData(char *FilePath_n,int SmpClass_n,int ClassQuty_n);
    /*!
     *	@brief Copy data to increase sum number of data
     *	@param SrcFilePath_n	Data path
     *	@param DstFilePath_n	Copyed data path
     *	@param SmpClass_n		Number of copyed sample classes
     *	@param ClassQuty_n		Number of copyed data in each class
     */
    void enhceData(char *SrcFilePath_n,char *DstFilePath_n,int SmpClass_n=5,int ClassQuty_n=20);
private:
    
    /*!
     *	@brief Neighber radius
     */
	int K;
    
    /*!
     *	@brief Szie of data(width=heigh)
     */
    int SmpSize;
    
    /*!
     *	@brief Number of sample classes
     */
    int SmpClass;
    
    /*!
     *	@brief Number of data in each calss
     */
    int ClassQuty;
    
    /*!
     *	@brief Algorithm ptr
     */
	CvKNearest *Knn;
    
    /*!
     *	@brief Root folder path
     */
	char FilePath[255];
    
    /*!
     *	@brief Matrix to load data info
     */
    CvMat *DataMatrix;
    
    /*!
     *	@brief Matrix to load classed info
     */
	CvMat *ClassMatrix;
    
    /*!
     *	@brief Train loaded data
     */
    void TrainData();
};

/*!
 *	@brief Create a window to display image and modify paras
 *	@param TA				Threshold A
 *	@param TB				Threshold B
 *	@param pKnlSize         Kernel size
 *	@param pErode           Erode times
 *	@param pDilate          Dilate times
 */
void showImg(int TA,int TB,int pKnlSize,int pErode,int pDilate);
/*!
 *	@brief Set threshold
 *	@param Threshold	Threshold
 */
void setTa(int Threshold);
/*!
 *	@brief Set threshold
 *	@param Threshold	Threshold
 */
void setTb(int Threshold);
/*!
 *	@brief Set kernel size
 *	@param pKnlSize	Kernel size(width=heigh)
 */
void setKnlSize(int pKnlSize);
/*!
 *	@brief Set erode times
 *	@param pErode	Erode times
 */
void setErode(int pErode);
/*!
 *	@brief Set dilate times
 *	@param pDilate	Dilate times
 */
void setDilate(int pDilate);

//Objects counting
class ObjectCnt{
private:
    
    /*!
     *	@brief Threshold
     */
    int ThresA;
    
    /*!
     *	@brief Threshold
     */
    int ThresB;
    
    /*!
     *	@brief Kernel size(width-heigh)
     */
    int KnlSize;
    
    /*!
     *	@brief Erode Times
     */
    int ErodeIter;
    
    /*!
     *	@brief Dilate Times
     */
    int DilateIter;
public:
    ObjectCnt();
    ~ObjectCnt();
    
    /*!
     *	@brief Load a pic and count objects
     *	@param pFilePath	Pic path
     *	@return Number of object
     */
    int cntObjects(char *pFilePath);
    /*!
     *	@brief Count objects in image
     *	@param pSrcImg	Image ptr
     *	@return Number of object
     */
    int cntObjects(IplImage *pSrcImg);
    /*!
     *	@brief Pre-Process image
     *	@param pSrcImg	Image ptr
     *	@return Processed image
     */
    IplImage *cntPrePrs(IplImage *pSrcImg);
    /*!
     *	@brief Print para
     */
    void objPrtPara();
};