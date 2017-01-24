//
//  CVLib.m
//  CVLib
//
//  Created by FoLeakey on 7/11/16.
//  Copyright (c) 2016 IntelligentGroup. All rights reserved.
//


#include "CVLib.h"

static int ThresholdA=0;
static int ThresholdB=0;
IplImage *GrayImg;
IplImage *CannyImg;


void findX(IplImage *SrcImg,int *Min,int *Max){
	int i;
	int MinFound=0;
	CvMat Data;
	CvScalar MaxVal=cvRealScalar(SrcImg->width*255);
	CvScalar Val=cvRealScalar(255);

	for (i=0;i<SrcImg->width;i++){
		cvGetCol(SrcImg,&Data,i);
		Val=cvSum(&Data);
		if(Val.val[0]<MaxVal.val[0]){
			*Max=i;
			if(!MinFound){
				*Min=i;
				MinFound=1;
			}
		}
	}
}
void findY(IplImage *SrcImg,int *Min,int *Max){
	int i;
	int MinFound=0;
	CvMat Data;
	CvScalar MaxVal=cvRealScalar(SrcImg->width*255);
	CvScalar Val=cvRealScalar(255);
    
	for (i=0;i<SrcImg->height;i++){
		cvGetRow(SrcImg,&Data,i);
		Val=cvSum(&Data);
		if(Val.val[0]<MaxVal.val[0]){
			*Max=i;
			if(!MinFound){
				*Min=i;
				MinFound=1;
			}
		}
	}
}
CvRect findMinRect(IplImage *SrcImg){
	CvRect MinRect;
	int X_Min,X_Max,Y_Min,Y_Max;
	X_Min=X_Max=Y_Min=Y_Max=0;
    
	findX(SrcImg,&X_Min,&X_Max);
	findY(SrcImg,&Y_Min,&Y_Max);
    
	MinRect=cvRect(X_Min,Y_Min,X_Max-X_Min+1,Y_Max-Y_Min+1);
    
	return MinRect;
}

IplImage *cvGetSubImage(IplImage *SrcImg,CvRect Roi){
    cvSetImageROI(SrcImg,Roi);
    IplImage *PrsImg=cvCreateImage(cvSize(Roi.width,Roi.height),SrcImg->depth,SrcImg->nChannels );
    cvCopy(SrcImg,PrsImg);
    cvResetImageROI(SrcImg);
    
    return PrsImg;
}
IplImage *prePrs(IplImage *SrcImg,int W_n,int H_n,int Mode){
    cvCanny(SrcImg,SrcImg,ThresholdA,ThresholdB);
    if(1==Mode){cvNot(SrcImg,SrcImg);}
    
    IplImage *PrsImg=cvCreateImage(cvSize(W_n,H_n),SrcImg->depth,SrcImg->nChannels);
    
    CvRect Rect=findMinRect(SrcImg);
    IplImage *RoiImg=cvGetSubImage(SrcImg,Rect);
    cvResize(RoiImg,PrsImg);
    
    return PrsImg;
}

IplImage *resizeImg(char *FilePath_n,CvSize Size_n){
    IplImage *RawImg=cvLoadImage(FilePath_n);
    IplImage *PrsImg=cvCreateImage(Size_n,RawImg->depth,RawImg->nChannels);
    cvResize(RawImg,PrsImg);
    
    return PrsImg;
}

void showImg(int TA,int TB){
    CvFont Font;
    int LineWidth=1;
    char ShowMsg[30];
    double H_Scale=1;
    double V_Scale=1;
    
    cvCanny(GrayImg,CannyImg,ThresholdA,ThresholdB);
    sprintf(ShowMsg,"Threshold A=%d,Threshold B=%d",TA,TB);
    cvInitFont(&Font,CV_FONT_HERSHEY_PLAIN|CV_FONT_ITALIC,H_Scale,V_Scale,0,LineWidth);
    cvPutText(CannyImg,ShowMsg,cvPoint(0,20),&Font,CV_RGB(255,255,255));
    cvShowImage("GetThreshold",CannyImg);
}
void min_Trackbar(int Threshold){
    ThresholdA=Threshold;
    showImg(ThresholdA,ThresholdB);
}
void max_Trackbar(int Threshold){
    ThresholdB=Threshold;
    showImg(ThresholdA,ThresholdB);
}

void getThreshold(IplImage *pSrcImg,int *TA,int *TB){
    GrayImg=cvCreateImage(cvGetSize(pSrcImg),pSrcImg->depth,pSrcImg->nChannels);
    GrayImg=cvCloneImage(pSrcImg);
    CannyImg=cvCreateImage(cvGetSize(pSrcImg),pSrcImg->depth,1);
    
    cvNamedWindow("GetThreshold",CV_WINDOW_AUTOSIZE);
    int Pos_Min=1;
    int Pos_Max=10;
    
    cvCreateTrackbar("Threshold A","GetThreshold",&Pos_Min,600,min_Trackbar);
    cvCreateTrackbar("Threshold B","GetThreshold",&Pos_Max,600,max_Trackbar);
    
    min_Trackbar(Pos_Min);
    max_Trackbar(Pos_Max);
    
    cvWaitKey();
    *TA=ThresholdA;
    *TB=ThresholdB;
    
    cvDestroyWindow("GetThreshold");
    cvReleaseImage(&GrayImg);
    cvReleaseImage(&CannyImg);
}

void getThreshold(char *FilePath_n,int *TA,int *TB){
    getThreshold(cvLoadImage(FilePath_n,CV_LOAD_IMAGE_GRAYSCALE),TA,TB);
}

void findRegion(IplImage *SrcImg,int *x,int *y,int Flag){
	int i,j;
    double Pixel;
    *x=SrcImg->width;
    *y=SrcImg->height;
    
	for(i=0;i<SrcImg->height;i+=2){
		for(j=0;j<SrcImg->width;j+=2){
            Pixel=cvGet2D(SrcImg,i,j).val[0];
            if((Flag*255)!=Pixel){
                if(i<*y){*y=i;}
                if(j<*x){*x=j;}
            }
        }
	}
}

void findXRange(IplImage *SrcImg,int *Min,int *Max,int Flag){
    double Pixel;
    *Min=SrcImg->width;
    *Max=0;
    
	for(int i=0;i<SrcImg->height;i+=2){
		for(int j=0;j<SrcImg->width;j+=2){
            Pixel=cvGet2D(SrcImg,i,j).val[0];
            if((Flag*255)!=Pixel){
                if(j<(*Min)){*Min=j;}
                if(j>(*Max)){*Max=j;}
            }
        }
	}
}

void findYRange(IplImage *SrcImg,int *Min,int *Max,int Flag){
    double Pixel;
    *Min=SrcImg->width;
    *Max=0;
    
	for(int i=0;i<SrcImg->width;i+=2){
		for(int j=0;j<SrcImg->height;j+=2){
            Pixel=cvGet2D(SrcImg,i,j).val[0];
            if((Flag*255)!=Pixel){
                if(j<(*Min)){*Min=j;}
                if(j>(*Max)){*Min=j;}
            }
        }
	}
}

IplImage *picStcer_sub(IplImage *ImgA,IplImage *ImgB,int mode){
    switch (mode){
        case VERTICAL:{
            IplImage *DstImg=cvCreateImage(cvSize(ImgA->width,(ImgA->height+ImgB->height)),ImgA->depth,ImgA->nChannels);
            cvSetImageROI(DstImg,cvRect(0,0,ImgA->width,ImgA->height));
            cvCopyImage(ImgA,DstImg);
            cvResetImageROI(DstImg);
            cvSetImageROI(DstImg,cvRect(0,ImgA->height,ImgB->width,ImgB->height));
            cvCopyImage(ImgB,DstImg);
            cvResetImageROI(DstImg);
            
            return DstImg;
            break;
        }
        case HORIZONTAL:{
            IplImage *DstImg=cvCreateImage(cvSize((ImgA->width+ImgA->width),ImgA->height),ImgA->depth,ImgA->nChannels);
            cvSetImageROI(DstImg,cvRect(0,0,ImgA->width,ImgA->height));
            cvCopyImage(ImgA,DstImg);
            cvResetImageROI(DstImg);
            cvSetImageROI(DstImg,cvRect(ImgA->width,0,ImgB->width,ImgB->height));
            cvCopyImage(ImgB,DstImg);
            cvResetImageROI(DstImg);
            
            return DstImg;
            break;
        }
        default:
            return NULL;
            break;
    }
    return NULL;
}

IplImage *picStcer(IplImage *SrcImgQue[],int lenth,int mode){
    if(lenth>1){
        switch (mode){
            case VERTICAL:{
                int nHeight=0;
                int sHeight[lenth];
                sHeight[0]=0;
                for (int i=0;i<lenth;i++){
                    nHeight+=SrcImgQue[i]->height;
                    sHeight[i+1]=nHeight;
                }
                IplImage *DstImg=cvCreateImage(cvSize(SrcImgQue[0]->width,nHeight),SrcImgQue[0]->depth,SrcImgQue[0]->nChannels);
                for (int i=0;i<lenth;i++){
                    printf("%d\n",sHeight[i]);
                    cvSetImageROI(DstImg,cvRect(0,sHeight[i],SrcImgQue[i]->width,SrcImgQue[i]->height/*+SrcImgQue[i+1]->height*/));
                    cvCopyImage(/*picStcer_sub(SrcImgQue[i],SrcImgQue[i+1],VERTICAL)*/SrcImgQue[i],DstImg);
                    cvResetImageROI(DstImg);
                }
                
                return DstImg;
                break;
            }
            case HORIZONTAL:{
                int nWidth=0;
                int sWidth[lenth];
                sWidth[0]=0;
                for (int i=0;i<lenth;i++){
                    nWidth+=SrcImgQue[i]->width;
                    sWidth[i+1]=nWidth;
                }
                IplImage *DstImg=cvCreateImage(cvSize(nWidth,SrcImgQue[0]->height),SrcImgQue[0]->depth,SrcImgQue[0]->nChannels);
                for (int i=0;i<lenth;i++){
                    cvSetImageROI(DstImg,cvRect(sWidth[i],0,SrcImgQue[i]->width,SrcImgQue[i]->height/*+SrcImgQue[i+1]->height*/));
                    cvCopyImage(/*picStcer_sub(SrcImgQue[i],SrcImgQue[i+1],VERTICAL)*/SrcImgQue[i],DstImg);
                    cvResetImageROI(DstImg);
                }
                
                return DstImg;
                break;
            }
            default:
                break;
        }
    }else{
        if(lenth<1){
            printf("ERROR\n");
            return NULL;
        }else if(1==lenth){
            return SrcImgQue[0];
        }
    }
    
    return NULL;
}

IplImage *autoStcer(IplImage *SrcImgQueen[],int lenth){
    vector<Mat> MatVec;
    Mat DstMat;
    for (int i=0;i<lenth;i++){
        MatVec.push_back(SrcImgQueen[i]);
    }
    
    Stitcher stitcher=Stitcher::createDefault(false);
    Stitcher::Status status=stitcher.stitch(MatVec,DstMat);
    
    if(status != Stitcher::OK)
    {
        printf("Stitching error!");
        return NULL;
    }
    
    IplImage TmpImg=DstMat;
    IplImage *DstImg=cvCloneImage(&TmpImg);
    
    return DstImg;
}

void KNNClass::setK(int n){
    K=n;
}
void KNNClass::setSmpClass(int n){
    SmpClass=n;
}
void KNNClass::setClassQuty(int n){
    ClassQuty=n;
}
void KNNClass::setSmpSize(int n){
    SmpSize=n;
}
KNNClass::KNNClass(int k_n,int SmpClass_n,int ClassQuty_n,int SmpSize_n){
    K=k_n;
    SmpClass=SmpClass_n;
    ClassQuty=ClassQuty_n;
    SmpSize=SmpSize_n;
    DataMatrix=cvCreateMat(ClassQuty*SmpClass,SmpSize*SmpSize,CV_32FC1);
	ClassMatrix=cvCreateMat(ClassQuty*SmpClass,1,CV_32FC1);
}
KNNClass::~KNNClass(){
    cvReleaseMat(&DataMatrix);
    cvReleaseMat(&ClassMatrix);
}
void KNNClass::setFilePath(char *FilePath_n){
    sprintf(FilePath,"%s",FilePath_n);
}
void KNNClass::setThresA(int n){
    ThresholdA=n;
}
void KNNClass::setThresB(int n){
    ThresholdB=n;
}
void KNNClass::loadData(char *FilePath_n,int SmpClass_n,int ClassQuty_n){
    printf("Loading data...\n");
    setFilePath(FilePath_n);
    int TA=250,TB=550;
    char File[50];
    sprintf(File,"%s%d/%03d.jpg",FilePath,0,0);
    getThreshold(File,&TA,&TB);
    setThresA(TA);
    setThresB(TB);
    
	for (int i=0;i<SmpClass_n;i++){
		for (int j=0;j< ClassQuty_n;j++){
			sprintf(File,"%s%d/%03d.jpg",FilePath,i,j );
            
            IplImage *Src_image=cvCreateImage(cvSize(SmpSize,SmpSize),IPL_DEPTH_8U,1);
            Src_image=cvLoadImage(File,CV_LOAD_IMAGE_GRAYSCALE);
			if(!Src_image){
                printf("Error: Cant load image: %s\n",File);
            }else{
                CvMat Row,Data;
                IplImage *Prs_image=prePrs(Src_image,Src_image->width,Src_image->height);
                cvGetRow(ClassMatrix,&Row,i*ClassQuty_n + j);
                cvSet(&Row,cvRealScalar(i));
                
                cvGetRow(DataMatrix,&Row,i*ClassQuty_n + j);
                
                IplImage *BioImg=cvCreateImage(cvGetSize(Src_image),IPL_DEPTH_32F,1);
                cvConvertScale(Prs_image,BioImg,0.0039215,0);
                cvGetSubRect(BioImg,&Data,cvRect(0,0,Src_image->width,Src_image->height));
                CvMat Row_Header,*Row_Shape;
                Row_Shape=cvReshape(&Data,&Row_Header,0,1);
                cvCopy(Row_Shape,&Row,NULL);
                
                cvReleaseImage(&Src_image);
                cvReleaseImage(&Prs_image);
                cvReleaseImage(&BioImg);
            }
		}
	}
    TrainData();
}
void KNNClass::TrainData(){
    printf("Training data...\n");
	Knn=new CvKNearest(DataMatrix,ClassMatrix,0,false,K);
}
float KNNClass::classify(char *FilePath_n){
    float Result;
    CvMat Data;
    CvMat RowHeader,*Row;
	CvMat *Nearest=cvCreateMat(1,K,CV_32FC1);
    IplImage *RawImg=cvLoadImage(FilePath_n,CV_LOAD_IMAGE_GRAYSCALE);
    IplImage *PrsImg=prePrs(RawImg,SmpSize,SmpSize);
    IplImage *BioImg =cvCreateImage(cvSize(SmpSize,SmpSize),IPL_DEPTH_32F,1);
    
    cvConvertScale(PrsImg,BioImg);
	cvGetSubRect(BioImg,&Data,cvRect(0,0,SmpSize,SmpSize));
    
	Row=cvReshape(&Data,&RowHeader,0,1);
    
	Result=Knn->find_nearest(Row,K,0,0,Nearest,0);
    
    cvReleaseImage(&RawImg);
    cvReleaseImage(&PrsImg);
    cvReleaseImage(&BioImg);
	return Result;
}
void KNNClass::selfTest(char *FilePath_n,int TestClass,int Max){
    printf("System Self Testing...\n");
	int ErrCnt=0;
    int TestCnt=0;
    char File[50];
	for (int i=0;i<TestClass;i++){
		for (int j=0;j < Max;j++){
			sprintf(File,"%s%d/%03d.jpg",FilePath_n,i,j );
            float r=classify(File);
            if((int)r!=i){
                ErrCnt++;
                printf("%s---%.2f\n",File,r);
            }
            TestCnt++;
		}
	}
    
	float Accuracy=100 - (100*(float)ErrCnt / (float)TestCnt);
    printf("System Accuracy: %.2f\n",Accuracy);
    
}
void KNNClass::enhceData(char *SrcFilePath_n,char *DstFilePath_n,int SmpClass_n,int ClassQuty_n){
    for (int i=0;i<SmpClass_n;i++){
        int Cnt=0;
        for (int j=0;j< ClassQuty_n;j++){
            char File[50];
            sprintf(File,"%sTrain_Image_%02d.BMP",SrcFilePath_n,j );
            IplImage *RawImg=resizeImg(File,cvSize(400,400));
            IplImage *tmpImg=cvCreateImage(cvGetSize(RawImg),RawImg->depth,RawImg->nChannels);
            cvZero(tmpImg);
            if(!RawImg){
                printf("Error: Cant load image %s\n",File);
            }
            else{
                sprintf(File,"%sTrain_Image_%02d.BMP",DstFilePath_n,j );
                cvSaveImage(File,tmpImg);
                //                sprintf(File,"%s%d/%03d.jpg",DstFilePath_n,i,Cnt+100 );
                //                cvSaveImage(File,RawImg);
                //                sprintf(File,"%s%d/%03d.jpg",DstFilePath_n,i,Cnt+20 );
                //                cvSaveImage(File,RawImg);
                //                sprintf(File,"%s%d/%03d.jpg",DstFilePath_n,i,Cnt+120 );
                //                cvSaveImage(File,RawImg);
                //                sprintf(File,"%s%d/%03d.jpg",DstFilePath_n,i,Cnt+40 );
                //                cvSaveImage(File,RawImg);
                //                sprintf(File,"%s%d/%03d.jpg",DstFilePath_n,i,Cnt+140 );
                //                cvSaveImage(File,RawImg);
                //                sprintf(File,"%s%d/%03d.jpg",DstFilePath_n,i,Cnt+60 );
                //                cvSaveImage(File,RawImg);
                //                sprintf(File,"%s%d/%03d.jpg",DstFilePath_n,i,Cnt+160 );
                //                cvSaveImage(File,RawImg);
                //                sprintf(File,"%s%d/%03d.jpg",DstFilePath_n,i,Cnt+80 );
                //                cvSaveImage(File,RawImg);
                //                sprintf(File,"%s%d/%03d.jpg",DstFilePath_n,i,Cnt+180 );
                //                cvSaveImage(File,RawImg);
                Cnt++;
            }
        }
    }
}

CamCtrl::CamCtrl(){
    pFrame=NULL;
}
void CamCtrl::opnCam(int CamIndex){
    pCapture=cvCreateCameraCapture(CamIndex);
    while (1){
        pFrame=cvQueryFrame(pCapture);
        cvShowImage("Camera View",pFrame);
        if(27==cvWaitKey(1)){break;}
    }
    
    cvReleaseCapture(&pCapture);
    cvDestroyAllWindows();
}
void CamCtrl::saveVideo(CvCapture *pCapture_n,char *FilePath_n){
    IplImage *SrcImg=cvQueryFrame(pCapture_n);
    CvVideoWriter *vWriter=cvCreateVideoWriter(FilePath_n,CV_FOURCC_DEFAULT,25,cvGetSize(SrcImg));
    while (1){
        SrcImg=cvQueryFrame(pCapture_n);
        cvWriteFrame(vWriter,SrcImg);
        cvShowImage("Recording",SrcImg);
        char Key=cvWaitKey(33);
        if(32==Key){
            break;
        }else if(83==Key){
            cvSaveImage("1.jpg",SrcImg);
        }
    }
    cvReleaseVideoWriter(&vWriter);
    cvDestroyWindow("Recording");
}

void InsertSort(unsigned char a[],int count){
    int i,j;
    char temp;
    for(i=1;i<count;i++){
        temp=a[i];
        j=i-1;
        while(a[j]>temp&&j>=0){
            a[j+1]=a[j];
            j--;
        }
        if(j!=(i-1)){
            a[j+1]=temp;
        }
    }
}

static int tmpKnlSize=1;
static int tmpErode=1;
static int tmpDilate=1;
IplImage *ExpImg;
void showImg(int TA,int TB,int pKnlSize,int pErode,int pDilate){
    IplConvKernel *element;
    CvFont Font;
    int LineWidth=1;
    char ShowThresMsg[30];
    char ShowParaMsg[50];
    double H_Scale=1;
    double V_Scale=1;
    if(pKnlSize==0){
        pKnlSize=1;
    }
    cvReleaseImage(&GrayImg);
    cvReleaseImage(&CannyImg);
    GrayImg=cvCreateImage(cvGetSize(ExpImg),ExpImg->depth,ExpImg->nChannels);
    GrayImg=cvCloneImage(ExpImg);
    CannyImg=cvCreateImage(cvGetSize(ExpImg),ExpImg->depth,ExpImg->nChannels);
    
    element=cvCreateStructuringElementEx( pKnlSize,pKnlSize,0,0,CV_SHAPE_RECT,0);
    cvErode( GrayImg,GrayImg,element,pErode);
    cvDilate( GrayImg,GrayImg,element,pDilate);
    if(ThresholdA==0&&ThresholdB==0){
        CannyImg=cvCloneImage(GrayImg);
    }else{
        cvCanny(GrayImg,CannyImg,ThresholdA,ThresholdB);
    }
    
    sprintf(ShowThresMsg,"Threshold A=%d,Threshold B=%d",TA,TB);
    sprintf(ShowParaMsg,"Kernel size=%d,Erode iter=%d,Dilate iter=%d",pKnlSize,pErode,pDilate);
    cvInitFont(&Font,CV_FONT_HERSHEY_PLAIN|CV_FONT_ITALIC,H_Scale,V_Scale,0,LineWidth);
    cvPutText(CannyImg,ShowThresMsg,cvPoint(0,20),&Font,CV_RGB(255,255,255));
    cvPutText(CannyImg,ShowParaMsg,cvPoint(0,60),&Font,CV_RGB(255,255,255));
    cvShowImage("PrePrsCnt",CannyImg);
}
void setTa(int Threshold){
    ThresholdA=Threshold;
    showImg(ThresholdA,ThresholdB,tmpKnlSize,tmpErode,tmpDilate);
}
void setTb(int Threshold){
    ThresholdB=Threshold;
    showImg(ThresholdA,ThresholdB,tmpKnlSize,tmpErode,tmpDilate);
}
void setKnlSize(int pKnlSize){
    tmpKnlSize=pKnlSize;
    showImg(ThresholdA,ThresholdB,tmpKnlSize,tmpErode,tmpDilate);
}
void setErode(int pErode){
    tmpErode=pErode;
    showImg(ThresholdA,ThresholdB,tmpKnlSize,tmpErode,tmpDilate);
}
void setDilate(int pDilate){
    tmpDilate=pDilate;
    showImg(ThresholdA,ThresholdB,tmpKnlSize,tmpErode,tmpDilate);
}
IplImage *ObjectCnt::cntPrePrs(IplImage *pSrcImg){
    cvReleaseImage(&ExpImg);
    ExpImg=cvCreateImage(cvGetSize(pSrcImg),pSrcImg->depth,pSrcImg->nChannels);
    ExpImg=cvCloneImage(pSrcImg);
    
    cvNamedWindow("PrePrsCnt",CV_WINDOW_AUTOSIZE);
    
    cvCreateTrackbar("Threshold A","PrePrsCnt",&ThresA,600,setTa);
    cvCreateTrackbar("Threshold B","PrePrsCnt",&ThresB,600,setTb);
    cvCreateTrackbar("Kernel size","PrePrsCnt",&KnlSize,10,setKnlSize);
    cvCreateTrackbar("Erode inter","PrePrsCnt",&ErodeIter,10,setErode);
    cvCreateTrackbar("Dilate inter","PrePrsCnt",&DilateIter,10,setDilate);
    
    setTa(ThresA);
    setTb(ThresB);
    setKnlSize(KnlSize);
    setErode(ErodeIter);
    setDilate(DilateIter);
    
    
    cvWaitKey();
    
    IplImage *DstImg=cvCloneImage(CannyImg);
    cvDestroyWindow("PrePrsCnt");
    cvReleaseImage(&GrayImg);
    cvReleaseImage(&CannyImg);
    cvReleaseImage(&ExpImg);
    
    return DstImg;
}

ObjectCnt::ObjectCnt(){
    ThresA=0;
    ThresB=0;
    KnlSize=4;
    ErodeIter=3;
    DilateIter=3;
    cvReleaseImage(&ExpImg);
}
ObjectCnt::~ObjectCnt(){
    cvReleaseImage(&ExpImg);
}
int ObjectCnt::cntObjects(char *pFilePath){
    IplImage *SrcImg=cvLoadImage(pFilePath,CV_LOAD_IMAGE_GRAYSCALE);
    return cntObjects(SrcImg);
}
int ObjectCnt::cntObjects(IplImage *pSrcImg){
    IplImage *pImg=cntPrePrs(pSrcImg);
    CvMemStorage *storage=cvCreateMemStorage();
    CvSeq *first_contour=NULL;
    int ObjCnt=0;
    
    ObjCnt=cvFindContours(pImg,storage,&first_contour,sizeof(CvContour),CV_RETR_LIST);
    
    return ObjCnt;
}
void ObjectCnt::objPrtPara(){
    printf("ThresA=%d,ThresB=%d,Kernel size=%d,Erode iter=%d,Dilate iter=%d\n",ThresA,ThresB,KnlSize,ErodeIter,DilateIter);
}