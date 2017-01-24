//
//  CVImgLib.m
//  CVImgLib
//
//  Created by FoLeakey on 6/14/16.
//  Copyright (c)2016 IntelligentGroup. All rights reserved.
//

#include "CVImgLib.h"

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


void saveInXml(CvMat *Mat,char *FilePath_n){
    CvFileStorage *FS=cvOpenFileStorage(FilePath_n,0,CV_STORAGE_WRITE);
    cvWrite(FS,"content",Mat);
    cvReleaseFileStorage(&FS);
}
CvMat *readFromXml(char *FilePath_n){
    CvFileStorage *FS=cvOpenFileStorage(FilePath_n,0,CV_STORAGE_READ);
    CvMat *Mat=(CvMat *)cvReadByName(FS,0,"content");
    cvReleaseFileStorage(&FS);
    return Mat;
}

int cvx_dump_vector(CvMat *Mat,char *FilePath_n,int Mode){
    int i,j;
    FILE *FP=fopen(FilePath_n,"w+");
    if(FP!=NULL){
        fprintf(FP,"%d %d\n",Mat->rows,Mat->cols);
        for(i=0;i<Mat->rows;i++){
            for(j=0;j<Mat->cols;j++){
                switch(Mode){
                    case 0:
                        fprintf(FP,"%d ",(Mat->data.i+i*Mat->step/4)[j]);
                        break;
                    case 1:
                        fprintf(FP,"%f ",(Mat->data.fl+i*Mat->step/4)[j]);
                        break;
                    case 2:
                        fprintf(FP,"%lf ",(Mat->data.db+i*Mat->step/8)[j]);
                        break;
                }
            }
            fprintf(FP,"\n");
        }
        fclose(FP);
    }
    return 0;
}
CvMat *cvx_load_vector(char *FilePath_n,int Mode){
    int Rows,Cols,i,j;
    CvMat *Mat;
    FILE *FP=fopen(FilePath_n,"r+");
    if(FP!=NULL){
        fscanf(FP,"%d %d",&Rows,&Cols);
        switch(Mode){
            case 0:
                Mat=cvCreateMat(Rows,Cols,CV_32SC1);
                break;
            case 1:
                Mat=cvCreateMat(Rows,Cols,CV_32FC1);
                break;
            case 2:
                Mat=cvCreateMat(Rows,Cols,CV_64FC1);
                break;
        }
        for(i=0;i<Mat->rows;i++){
            for(j=0;j<Mat->cols;j++){
                switch(Mode){
                    case 0:
                        fscanf(FP,"%d",&(Mat->data.i+i*Mat->step/4)[j]);
                        break;
                    case 1:
                        fscanf(FP,"%f",&(Mat->data.fl+i*Mat->step/4)[j]);
                        break;
                    case 2:
                        fscanf(FP,"%lf",&(Mat->data.db+i*Mat->step/8)[j]);
                        break;
                }
            }
        }
        fclose(FP);
    }
    
    return Mat;
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

//IplImage *autoStcer(IplImage *SrcImgQueen[],int lenth){
//    vector<Mat> MatVec;
//    Mat DstMat;
//    for (int i=0;i<lenth;i++){
//        MatVec.push_back(SrcImgQueen[i]);
//    }
//
//    Stitcher stitcher=Stitcher::createDefault(false);
//    Stitcher::Status status=stitcher.stitch(MatVec,DstMat);
//    
//    if(status != Stitcher::OK)
//    {
//        printf("Stitching error!");
//        return NULL;
//    }
//    
//    IplImage TmpImg=DstMat;
//    IplImage *DstImg=cvCloneImage(&TmpImg);
//    
//    return DstImg;
//}

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

CvMat *GuassFilter(CvMat *pGrayMat,int nWidth,int nHeight,double dSigma){
    //pGrayMat:待处理图像数组
    //pFilterMat:保存高斯滤波结果
    //nWidth:图像宽度
    //nHeight:图像高度
    //dSigma:高斯滤波参数，方差
    CvMat *FilterMat;
    int nWidowSize=(int)(1+2*ceil(3*dSigma));
    int nCenter=(nWidowSize)/2;

    double *pdKernal=new double[nWidowSize*nWidowSize];
    double dSum=0.0;

    for(int i=0;i<nWidowSize;i++){
        for(int j=0;j<nWidowSize;j++){
            int nDis_x=i-nCenter;
            int nDis_y=j-nCenter;
            pdKernal[i+j*nWidowSize]=exp(-(1/2)*(nDis_x*nDis_x+nDis_y*nDis_y)/(dSigma*dSigma))/(2*3.1415926*dSigma*dSigma);
            dSum+=pdKernal[i+j*nWidowSize];
        }
    }

    for(int i=0;i<nWidowSize;i++){
        for(int j=0;j<nWidowSize;j++){
            pdKernal[i+j*nWidowSize]/=dSum;
        }
    }
    for(int i=0;i<nHeight;i++){
        for(int j=0;j<nWidth;j++){
            double dFilter=0.0;
            double dSum=0.0;
            for(int x=(-nCenter);x<=nCenter;x++){
                for(int y=(-nCenter);y<=nCenter;y++){
                    if((j+x)>=0&&(j+x)<nWidth&&(i+y)>=0&&(i+y)<nHeight){
                        double ImageData=cvmGet(pGrayMat,i+y,j+x);
                        dFilter+=ImageData*pdKernal[(y+nCenter)*nWidowSize+(x+nCenter)];
                        dSum+=pdKernal[(y+nCenter)*nWidowSize+(x+nCenter)];
                    }
                }
            }
            cvmSet(FilterMat,i,j,dFilter/dSum);
        }
    }
    delete []pdKernal;
    
    return FilterMat;
}
CvMat *MeanFilter(CvMat *pGrayMat,int nWidth,int nHeight,int nWindows)
{
    //pGrayMat:待处理图像数组
    //pFilterMat:保存高斯滤波结果
    //nWidth:图像宽度
    //nHeight:图像高度
    //nWindows:滤波窗口大小
    CvMat *FilterMat;
    if((nWindows%2)== 0){
        printf("Filter Windon Must Be Odd.\n");
        return NULL;
    }
    int nNumData=nWindows/2;
    if((nWindows>nHeight)&&(nWindows>nWidth)){
        printf("Filter Window Out of Range.\n");
        return NULL;
    }
    for(int i=nNumData;i<(nHeight-nNumData);i++){
        for(int j=nNumData;j<(nWidth-nNumData);j++){
            double dData=0.0;
            int nNum=0;
            for(int m=-nNumData;m<=nNumData;m++){
                for(int n=-nNumData;n<=nNumData;n++){
                    if((m!=0)&&(n!=0)){
                        dData+=cvmGet(pGrayMat,i+m,j+n);
                        nNum++;
                    }
                }
            }
            dData/=nNum;
            cvmSet(FilterMat,i,j,dData);
        }
    }
    
    return FilterMat;
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
CvMat *MedianFilter(CvMat *pGrayMat,int nWidth,int nHeight,int nWindows){
    //pGrayMat:待处理图像数组
    //pFilterMat:保存高斯滤波结果
    //nWidth:图像宽度
    //nHeight:图像高度
    //nWindows:滤波窗口大小
    CvMat *FilterMat;
    if((nWindows%2)== 0){
        printf("Filter Windon Must Be Odd.\n");
        return NULL;
    }
    int nNumData=nWindows/2;
    unsigned char *nData=new unsigned char[nWindows*nWindows-1];
    if((nWindows>nHeight)&&(nWindows>nWidth)){
        printf("Filter Window Out of Range.\n");
        return NULL;
    }
    for(int i=nNumData;i<(nHeight-nNumData);i++){
        for(int j=nNumData;j<(nWidth-nNumData);j++){
            int nIndex=0;
            for(int m=-nNumData;m<=nNumData;m++){
                for(int n=-nNumData;n<=nNumData;n++){
                    if((m!=0)&&(n!=0))
                        nData[nIndex++]=(unsigned char)cvmGet(pGrayMat,i+m,j+n);
                }
            }
            InsertSort(nData,nIndex);
            unsigned char nMedium=0;
            if(nIndex%2==0){
                nMedium=(unsigned char)((nData[(nIndex-1)/2]+nData[(nIndex+1)/2])/2);
            }else{
                nMedium=nData[nIndex/2];
            }
            cvmSet(FilterMat,i,j,nMedium);
        }
    }
    delete []nData;
    
    return FilterMat;
}
CvMat *BilateralFilter(CvMat *pGrayMat,int nWidth,int nHeight,double dSigma1,double dSigma2,int nWindows){
    //pGrayMat:待处理图像数组
    //pFilterMat:保存高斯滤波结果
    //nWidth:图像宽度
    //nHeight:图像高度
    //dSigma1、dSigma2:分别为几何与灰度相关高斯函数的方差
    CvMat *FilterMat;
    double *dDistance=new double[nWindows*nWindows];
    double *dGrayDiff=new double[nWindows*nWindows];
    
    for(int i=0;i<nWindows*nWindows;i++){
        int nNumX=i/nWindows;
        int nNumY=i%nWindows;
        dDistance[i]=((nWindows-1)/2-nNumX)*((nWindows-1)/2-nNumX)+((nWindows-1)/2-nNumY)*((nWindows-1)/2-nNumY);
        dDistance[i]=exp(-0.5*dDistance[i]/dSigma1/dSigma1);//C参数
    }
    
    for(int i=0;i<nHeight;i++){
        for(int j=0;j<nWidth;j++){
            double dGray=cvmGet(pGrayMat,i,j);
            double dData=0.0;
            double dTotal=0.0;
            for(int m=0;m<nWindows*nWindows;m++){
                int nNumX=m/nWindows;
                int nNumY=m%nWindows;
                int nX=i-(nWindows-1)/2+nNumX;
                int nY=j-(nWindows-1)/2+nNumY;
                if( (nY>=0)&&(nY<nWidth)&&(nX>=0)&&(nX<nHeight)){
                    double dGray1=cvmGet(pGrayMat,nX,nY);
                    dGrayDiff[m]=fabs(dGray-dGray1);
                    dGrayDiff[m]=exp(-0.5*dGrayDiff[m]*dGrayDiff[m]/dSigma2/dSigma2);
                    if(m!=4){
                        dData+=dGray1*dGrayDiff[m]*dDistance[m];
                        dTotal+=dGrayDiff[m]*dDistance[m];
                    }
                }
            }
            dData /=dTotal;
            cvmSet(FilterMat,i,j,dData);
        }
    }
    
    delete []dDistance;
    delete []dGrayDiff;
    
    return FilterMat;
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


//定义参数
vector<string> img_names;
bool try_gpu = false;
double work_megapix = 0.6;//图像匹配的分辨率大小，图像的面积尺寸变为work_megapix*100000

double seam_megapix = 0.1;//拼接缝像素的大小
double compose_megapix =0.6;//拼接分辨率
float conf_thresh = 1.f;//两幅图来自同一全景图的置信度
WaveCorrectKind wave_correct = detail::WAVE_CORRECT_HORIZ;//波形校验，水平
int expos_comp_type = ExposureCompensator::GAIN_BLOCKS;//光照补偿方法，默认是gain_blocks
float match_conf = 0.3f;//特征点检测置信等级，最近邻匹配距离与次近邻匹配距离的比值，surf默认为0.65
int blend_type = Blender::MULTI_BAND;//融合方法，默认是多频段融合
float blend_strength = 5;//融合强度，0 - 100.默认是5.
string result_name = "/Users/Leakey/Desktop/CVImgLibTest/Build/Products/Debug/result.jpg";//输出图像的文件名

void autoStitcher(){
	clock_t start,finish;
    double totaltime;
    start=clock();
	int argc = 3;
	char *argv[] = {
        "/Users/Leakey/Desktop/CVImgLibTest/Build/Products/Debug/01.jpg",
        "/Users/Leakey/Desktop/CVImgLibTest/Build/Products/Debug/02.jpg",
        "/Users/Leakey/Desktop/CVImgLibTest/Build/Products/Debug/03.jpg"
    };
    
	for (int i = 0; i < argc; i++)
		img_names.push_back(argv[i]);
	int num_images = static_cast<int>(img_names.size());
	double work_scale = 1, seam_scale = 1, compose_scale = 1;
	//特征点检测以及对图像进行预处理（尺寸缩放），然后计算每幅图形的特征点，以及特征点描述子
	cout<<"Finding features..."<<endl;
    
	Ptr<FeaturesFinder> finder;
	finder = new SurfFeaturesFinder();///采用Surf特征点检测
    
	Mat full_img1,full_img, img;
	vector<ImageFeatures> features(num_images);
	vector<Mat> images(num_images);
	vector<Size> full_img_sizes(num_images);
	double seam_work_aspect = 1;
    
	for (int i = 0; i < num_images; ++i)
	{
		full_img1 = imread(img_names[i]);
		resize(full_img1,full_img, Size(345,336));
		full_img_sizes[i] = full_img.size();
        
		//计算work_scale，将图像resize到面积在work_megapix*10^6以下
		work_scale = min(1.0, sqrt(work_megapix * 1e6 / full_img.size().area()));
        
		resize(full_img, img, Size(), work_scale, work_scale);
		//将图像resize到面积在work_megapix*10^6以下
		seam_scale = min(1.0, sqrt(seam_megapix * 1e6 / full_img.size().area()));
		seam_work_aspect = seam_scale / work_scale;
		// 计算图像特征点，以及计算特征点描述子，并将img_idx设置为i
		(*finder)(img, features[i]);
		features[i].img_idx = i;
		cout<<"Features in image #" << i+1 << ": " << features[i].keypoints.size()<<endl;
		//将源图像resize到seam_megapix*10^6，并存入image[]中
		resize(full_img, img, Size(), seam_scale, seam_scale);
		images[i] = img.clone();
	}
    
	finder->collectGarbage();
	full_img.release();
	img.release();
    
	//对图像进行两两匹配
	cout<<"Pairwise matching"<<endl;
    
	//使用最近邻和次近邻匹配，对任意两幅图进行特征点匹配
	vector<MatchesInfo> pairwise_matches;
	BestOf2NearestMatcher matcher(try_gpu, match_conf);//最近邻和次近邻法
	matcher(features, pairwise_matches); //对每两个图片进行匹配
	matcher.collectGarbage();
	//将置信度高于门限的所有匹配合并到一个集合中
	///只留下确定是来自同一全景图的图片
	
	vector<int> indices = leaveBiggestComponent(features, pairwise_matches, conf_thresh);
	vector<Mat> img_subset;
	vector<string> img_names_subset;
	vector<Size> full_img_sizes_subset;
	for (size_t i = 0; i < indices.size(); ++i)
	{
		img_names_subset.push_back(img_names[indices[i]]);
		img_subset.push_back(images[indices[i]]);
		full_img_sizes_subset.push_back(full_img_sizes[indices[i]]);
	}
    
	images = img_subset;
	img_names = img_names_subset;
	full_img_sizes = full_img_sizes_subset;
    
	// 检查图片数量是否依旧满足要求
	num_images = static_cast<int>(img_names.size());
	if (num_images < 2)
	{
        cout<<"Need more images"<<endl;
        return;
	}
    
	HomographyBasedEstimator estimator;//基于单应性的估计量
	vector<CameraParams> cameras;//相机参数
	estimator(features, pairwise_matches, cameras);
    
	for (size_t i = 0; i < cameras.size(); ++i)
	{
		Mat R;
		cameras[i].R.convertTo(R, CV_32F);
		cameras[i].R = R;
		cout<<"Initial intrinsics #" << indices[i]+1 << ":\n" << cameras[i].K()<<endl;
	}
    
	Ptr<detail::BundleAdjusterBase> adjuster;//光束调整器参数
	adjuster = new detail::BundleAdjusterRay();//使用Bundle Adjustment（光束法平差）方法对所有图片进行相机参数校正
    
	adjuster->setConfThresh(conf_thresh);//设置配置阈值
	Mat_<uchar> refine_mask = Mat::zeros(3, 3, CV_8U);
	refine_mask(0,0) = 1;
	refine_mask(0,1) = 1;
	refine_mask(0,2) = 1;
	refine_mask(1,1) = 1;
	refine_mask(1,2) = 1;
	adjuster->setRefinementMask(refine_mask);
	(*adjuster)(features, pairwise_matches, cameras);//进行矫正
    
	
	// 求出的焦距取中值和所有图片的焦距并构建camera参数，将矩阵写入camera
	vector<double> focals;
	for (size_t i = 0; i < cameras.size(); ++i)
	{
		cout<<"Camera #" << indices[i]+1 << ":\n" << cameras[i].K()<<endl;
		focals.push_back(cameras[i].focal);
	}
    
	sort(focals.begin(), focals.end());
	float warped_image_scale;
	if (focals.size() % 2 == 1)
		warped_image_scale = static_cast<float>(focals[focals.size() / 2]);
	else
		warped_image_scale = static_cast<float>(focals[focals.size() / 2 - 1] + focals[focals.size() / 2]) * 0.5f;
    
	///波形矫正
    vector<Mat> rmats;
    for (size_t i = 0; i < cameras.size(); ++i)
        rmats.push_back(cameras[i].R);
    waveCorrect(rmats, wave_correct);////波形矫正
    for (size_t i = 0; i < cameras.size(); ++i)
        cameras[i].R = rmats[i];
    
    
	cout<<"Warping images ... "<<endl;
    
    
	vector<Point> corners(num_images);//统一坐标后的顶点
	vector<Mat> masks_warped(num_images);
	vector<Mat> images_warped(num_images);
	vector<Size> sizes(num_images);
	vector<Mat> masks(num_images);//融合掩码
    
	// 准备图像融合掩码
	for (int i = 0; i < num_images; ++i)
	{
		masks[i].create(images[i].size(), CV_8U);
		masks[i].setTo(Scalar::all(255));
	}
    
	//弯曲图像和融合掩码
    
	Ptr<WarperCreator> warper_creator;
	warper_creator = new cv::SphericalWarper();
    
	Ptr<RotationWarper> warper = warper_creator->create(static_cast<float>(warped_image_scale * seam_work_aspect));
    
	for (int i = 0; i < num_images; ++i)
	{
		Mat_<float> K;
		cameras[i].K().convertTo(K, CV_32F);
		float swa = (float)seam_work_aspect;
		K(0,0) *= swa; K(0,2) *= swa;
		K(1,1) *= swa; K(1,2) *= swa;
        
		corners[i] = warper->warp(images[i], K, cameras[i].R, INTER_LINEAR, BORDER_REFLECT, images_warped[i]);//计算统一后坐标顶点
		sizes[i] = images_warped[i].size();
        
		warper->warp(masks[i], K, cameras[i].R, INTER_NEAREST, BORDER_CONSTANT, masks_warped[i]);//弯曲当前图像
	}
    
	vector<Mat> images_warped_f(num_images);
	for (int i = 0; i < num_images; ++i)
		images_warped[i].convertTo(images_warped_f[i], CV_32F);
    
    
	Ptr<ExposureCompensator> compensator = ExposureCompensator::createDefault(expos_comp_type);//建立补偿器以进行关照补偿，补偿方法是gain_blocks
	compensator->feed(corners, images_warped, masks_warped);
    
	//查找接缝
	Ptr<SeamFinder> seam_finder;
	seam_finder = new detail::GraphCutSeamFinder(GraphCutSeamFinderBase::COST_COLOR);
    seam_finder->find(images_warped_f, corners, masks_warped);
    
	// 释放未使用的内存
	images.clear();
	images_warped.clear();
	images_warped_f.clear();
	masks.clear();
    
	//////图像融合
	cout<<"Compositing..."<<endl;
    
    
	Mat img_warped, img_warped_s;
	Mat dilated_mask, seam_mask, mask, mask_warped;
	Ptr<Blender> blender;
	
	double compose_work_aspect = 1;
    
	for (int img_idx = 0; img_idx < num_images; ++img_idx)
	{
		cout<<"Compositing image #" << indices[img_idx]+1<<endl;
		//由于以前进行处理的图片都是以work_scale进行缩放的，所以图像的内参
		//corner（统一坐标后的顶点），mask（融合的掩码）都需要重新计算
        
		// 读取图像和做必要的调整
		
		full_img1 = imread(img_names[img_idx]);
		resize(full_img1,full_img, Size(400,300));
		compose_scale = min(1.0, sqrt(compose_megapix * 1e6 / full_img.size().area()));
		compose_work_aspect = compose_scale / work_scale;
		// 更新弯曲图像比例
		warped_image_scale *= static_cast<float>(compose_work_aspect);
		warper = warper_creator->create(warped_image_scale);
        
		// 更新corners和sizes
		for (int i = 0; i < num_images; ++i)
		{
			// 更新相机以下特性
			cameras[i].focal *= compose_work_aspect;
			cameras[i].ppx *= compose_work_aspect;
			cameras[i].ppy *= compose_work_aspect;
            
			// 更新corners和sizes
			Size sz = full_img_sizes[i];
			if (std::abs(compose_scale - 1) > 1e-1)
			{
				sz.width = cvRound(full_img_sizes[i].width * compose_scale);
				sz.height = cvRound(full_img_sizes[i].height * compose_scale);
			}
            
			Mat K;
			cameras[i].K().convertTo(K, CV_32F);
			Rect roi = warper->warpRoi(sz, K, cameras[i].R);
			corners[i] = roi.tl();
			sizes[i] = roi.size();
		}
        
		if (abs(compose_scale - 1) > 1e-1)
			resize(full_img, img, Size(), compose_scale, compose_scale);
		else
			img = full_img;
		full_img.release();
		Size img_size = img.size();
        
		Mat K;
		cameras[img_idx].K().convertTo(K, CV_32F);
		// 扭曲当前图像
		warper->warp(img, K, cameras[img_idx].R, INTER_LINEAR, BORDER_REFLECT, img_warped);
		// 扭曲当前图像掩模
		mask.create(img_size, CV_8U);
		mask.setTo(Scalar::all(255));
		warper->warp(mask, K, cameras[img_idx].R, INTER_NEAREST, BORDER_CONSTANT, mask_warped);
        
		// 曝光补偿
		compensator->apply(img_idx, corners[img_idx], img_warped, mask_warped);
        
		img_warped.convertTo(img_warped_s, CV_16S);
		img_warped.release();
		img.release();
		mask.release();
        
		dilate(masks_warped[img_idx], dilated_mask, Mat());
		resize(dilated_mask, seam_mask, mask_warped.size());
		mask_warped = seam_mask & mask_warped;
		//初始化blender
		if (blender.empty())
		{
			blender = Blender::createDefault(blend_type, try_gpu);
			Size dst_sz = resultRoi(corners, sizes).size();
			float blend_width = sqrt(static_cast<float>(dst_sz.area())) * blend_strength / 100.f;
			if (blend_width < 1.f)
				blender = Blender::createDefault(Blender::NO, try_gpu);
			else
			{
				MultiBandBlender* mb = dynamic_cast<MultiBandBlender*>(static_cast<Blender*>(blender));
				mb->setNumBands(static_cast<int>(ceil(log(blend_width)/log(2.)) - 1.));
				cout<<"Multi-band blender, number of bands: " << mb->numBands()<<endl;
			}
			//根据corners顶点和图像的大小确定最终全景图的尺寸
			blender->prepare(corners, sizes);
		}
        
		// // 融合当前图像
		blender->feed(img_warped_s, mask_warped, corners[img_idx]);
	}
    
	Mat result, result_mask;
	blender->blend(result, result_mask);
    
	imwrite(result_name, result);
    
    finish=clock();
    totaltime=(double)(finish-start)/CLOCKS_PER_SEC;
    cout<<"\n此程序的运行时间为"<<totaltime<<"秒！"<<endl;
    
	return;
}
