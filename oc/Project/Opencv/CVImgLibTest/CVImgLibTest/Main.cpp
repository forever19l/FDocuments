//
//  Main.cpp
//  CVImgLibTest
//
//  Created by FoLeakey on 6/15/16.
//  Copyright (c) 2016 IntelligentGroup. All rights reserved.
//

#include "CVImgLib.h"

#define SAMPLES "samples/train/"



int main(){
    char FilePath[255];
//#pragma mark--normal pic process
    
#pragma mark---auto pic stitcher func
//    IplImage *img[2];
//    for (int i=0; i<2; i++) {
//        sprintf(FilePath, "%03d.jpg",i);
//        img[i]=cvLoadImage(FilePath);
//    }
//    IplImage *FinImg=autoStcer(img, 2);
//    cvShowImage("Test", FinImg);
#pragma mark---low pic stitcher func
//    char *filepath="samples/train/0/";
//    IplImage *arr[10];
//    for (int i=0; i<10; i++) {
//        char file[255];
//        sprintf(file, "%s%03d.jpg",filepath,i);
//        arr[i]=cvLoadImage(file);
//    }
//    int len=0;
//    GET_ARRAY_LEN(arr, len);
//    printf("%d",len);
//    IplImage *steImg=picStcer(arr,len, VERTICAL);
//    cvShowImage("2333", steImg);
//    IplImage *SteImg=picStcer(arr,len, HORIZONTAL);
//    cvShowImage("2333", SteImg);
#pragma mark---low pic stitcher
//    IplImage *rawimg=cvLoadImage("233333.jpg");
//    IplImage *gryimg=cvLoadImage("233333.jpg",CV_LOAD_IMAGE_GRAYSCALE);
//    IplImage *cayimg=cvCreateImage(cvGetSize(gryimg), rawimg->depth, 1);
//    cvCanny(gryimg, cayimg, 100, 400);
//    
//    CvSize ddize;
//    ddize.width=rawimg->width;
//    ddize.height=rawimg->height*3;
//    IplImage *catimg=cvCreateImage(ddize, rawimg->depth, rawimg->nChannels);
//    cvZero(catimg);
//    
//    cvSetImageROI(catimg, cvRect(0, 0, rawimg->width, rawimg->height));
//    cvCopy(rawimg, catimg);
//    cvResetImageROI(catimg);
//    cvShowImage("CatImg", catimg);
//    cvWaitKey();
//    
//    cvSetImageROI(catimg, cvRect(0, rawimg->height,cayimg->width,cayimg->height));
////    catimg->nChannels=1;
//    cvCopy(rawimg, catimg);
//    cvResetImageROI(catimg);
//    cvShowImage("CatImg", catimg);
//    cvWaitKey();
//    
//    cvSetImageROI(catimg, cvRect(0, (rawimg->height+cayimg->height),gryimg->width,gryimg->height));
//    cvCopy(rawimg, catimg);
//    cvResetImageROI(catimg);
//    cvShowImage("CatImg", catimg);
#pragma mark---filter func test
//    rawimg=cvLoadImage("2333.jpg",CV_LOAD_IMAGE_GRAYSCALE);
//    IplImage *iu=cvCreateImage(cvSize((rawimg->width)/5, (rawimg->height)/5), rawimg->depth, rawimg->nChannels);
//    cvResize(rawimg, iu);
//    cvSaveImage("233333.jpg", iu);
//    CvMat *gmat=cvCreateMatHeader(rawimg->height, rawimg->width, CV_8UC3);
//    CvMat *mmat=cvCreateMatHeader(rawimg->height, rawimg->width, CV_8UC3);
//    CvMat *bmat=cvCreateMatHeader(rawimg->height, rawimg->width, CV_8UC3);
//    CvMat *dmat=cvCreateMatHeader(rawimg->height, rawimg->width, CV_8UC3);
//    cvGetMat(rawimg, gmat);
//    cvGetMat(rawimg, mmat);
//    cvGetMat(rawimg, bmat);
//    cvGetMat(rawimg, dmat);
////    gmat=GuassFilter(gmat, rawimg->width, rawimg->height, 5);
//    mmat=MeanFilter(gmat, rawimg->width, rawimg->height, 5);
//    bmat=BilateralFilter(bmat, rawimg->width, rawimg->height, 5, 5, 5);
//    dmat=MedianFilter(dmat, rawimg->width, rawimg->height, 5);
//    
////    IplImage *gimg=cvCreateImageHeader(cvGetSize(gmat), IPL_DEPTH_8U, 3);
//    IplImage *mimg=cvCreateImageHeader(cvGetSize(mmat), IPL_DEPTH_8U, 3);
//    IplImage *dimg=cvCreateImageHeader(cvGetSize(dmat), IPL_DEPTH_8U, 3);
//    IplImage *bimg=cvCreateImageHeader(cvGetSize(bmat), IPL_DEPTH_8U, 3);
////    cvGetImage(gmat, gimg);
//    cvGetImage(mmat, mimg);
//    cvGetImage(dmat, dimg);
//    cvGetImage(bmat, bimg);
////    cvShowImage("g",gimg);
//    cvShowImage("m",mimg);
//    cvShowImage("d",dimg);
//    cvShowImage("b",bimg);
//    cvWaitKey();
//    cvDestroyAllWindows();
#pragma mark---get threshold func test
//    int min,max;
//    getThreshold("233333.jpg",&min,&max);
//    cvWaitKey();
//    printf("min=%d,max=%d\n",min,max);
#pragma mark---object detect func test
    KNNClass obj;
//    obj.enhceData("/vault/AOI_HALCON/test/D/", "/vault/AOI_HALCON/test/E/",1,23);
    obj.loadData("samples/train/",5,200);
    obj.selfTest("samples/val/", 5, 20);

    float rate=obj.classify("samples/val/0/003.jpg");
    printf("The detect result is %d",(int)rate);
#pragma make---open camera func test
//    CamCtrl cam;
//    cam.opnCam(-1);
#pragma mark---save video
//    CamCtrl cam;
//    IplImage *SrcImg=NULL;
//    CvCapture *pCapture_n=cvCreateCameraCapture(-1);
//    SrcImg=cvQueryFrame(pCapture_n);

//    while (1) {
//        SrcImg=cvQueryFrame(pCapture_n);
//        cvSmooth(SrcImg, SrcImg,CV_GAUSSIAN,13,13);
//        cvShowImage("Video", SrcImg);
//        char Key=cvWaitKey(33);
//        if (32==Key) {
//            cvDestroyWindow("video");
////            cvReleaseImage(&SrcImg);
//            cam.saveVideo(pCapture_n, "1.mp4");
//        }else if(27==Key){
//            break;
//        }
//    }
//    cvReleaseCapture(&pCapture_n);
//
//    cvDestroyAllWindows();
//    int n=ObjectCnt::cntObjects("233333.jpg");
#pragma mark---object count func test
//    ObjectCnt obj;
//    printf("Count=%d\n",obj.cntObjects("233333.jpg"));
    
    cvWaitKey();
    cvDestroyAllWindows();
    
    
    return 0;
}