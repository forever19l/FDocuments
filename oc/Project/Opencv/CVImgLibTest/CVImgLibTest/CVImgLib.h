//
//  CVImgLib.h
//  CVImgLib
//
//  Created by FoLeakey on 6/14/16.
//  Copyright(c)2016 IntelligentGroup. All rights reserved.
//

#ifndef __CVImgLibTest__CVImgLib__
#define __CVImgLibTest__CVImgLib__


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

//Resize pic into setted size.
IplImage *resizeImg(char *FilePath_n,CvSize Size_n);
//Get a pic's min non-empty pixel region.
CvRect findMinRect(IplImage *SrcImg);
//Prepress a pic, get a min binary region. 1 for turn pixel into ~.
IplImage *prePrs(IplImage *SrcImg,int W_n, int H_n,int Mode=1);

//Call a window and show a pic to ensure the threshold for binary a pic
void getThreshold(char *FilePath_n,int *TA,int *TB);
void getThreshold(IplImage *pSrcImg,int *TA,int *TB);

//Find the pixel region for a binary pic. 1 for white, 0 for black.
void findRegion(IplImage *SrcImg,int *x,int *y,int Flag=1);
void findXRange(IplImage *SrcImg,int *Min,int *Max,int Flag=1);
void findYRange(IplImage *SrcImg,int *Min,int *Max,int Flag=1);

//Four filter func, useless.
CvMat *GuassFilter(CvMat *pGrayMat, int nWidth, int nHeight, double dSigma);
CvMat *MeanFilter(CvMat *pGrayMat, int nWidth, int nHeight, int nWindows);
CvMat *MedianFilter(CvMat *pGrayMat, int nWidth, int nHeight, int nWindows);
CvMat *BilateralFilter(CvMat *pGrayMat, int nWidth, int nHeight, double dSigma1, double dSigma2, int nWindows);

//Stitch a series pic, which must be related pics.
//Stitching pics simply, 1 for vertical stitch, 0 for horizontal stitch.
IplImage *picStcer(IplImage *SrcImgQueen[],int lenth,int mode);
//Stitching pics with feature matching.
IplImage *autoStcer(IplImage *SrcImgQueen[],int lenth);

//Simple camera contrl.
class CamCtrl{
private:
    //int rFlag=0;
    IplImage *pFrame;
    CvCapture *pCapture;
public:
    CamCtrl();
    void opnCam(int CamIndex);
    void saveVideo(CvCapture *pCapture_n,char *FilePath_n);
};

//k-Nearest-Neighbor, for simple object detect.
class KNNClass{
public:
    void setK(int n);
    void setThresA(int n);
    void setThresB(int n);
    void setSmpSize(int n);
    void setSmpClass(int n);
    void setClassQuty(int n);
    void setFilePath(char *FilePath_n);
    KNNClass(int k_n=5,int SmpClass_n=5,
             int ClassQuty_n=200,
             int SmpSize_n=400);

    float classify(char *FilePath_n);
    void selfTest(char *FilePath_n,
                  int TestClass,
                  int Max);
	void loadData(char *FilePath_n,
                  int SmpClass_n,
                  int ClassQuty_n);
    void enhceData(char *SrcFilePath_n,
                   char *DstFilePath_n,
                   int SmpClass_n=5,
                   int ClassQuty_n=20);
private:
	int K;
    int SmpSize;
    int SmpClass;
    int ClassQuty;
	CvKNearest *Knn;
	char FilePath[255];
    CvMat *DataMatrix;
	CvMat *ClassMatrix;

    void TrainData();
};

//Objects counting.
class ObjectCnt{
private:
    int ThresA;
    int ThresB;
    int KnlSize;
    int ErodeIter;
    int DilateIter;
public:
    ObjectCnt();
    int cntObjects(char *pFilePath);
    int cntObjects(IplImage *pSrcImg);
    IplImage *cntPrePrs(IplImage *pSrcImg);
    void objPrtPara();
};

#endif /* defined(__CVImgLibTest__CVImgLib__) */