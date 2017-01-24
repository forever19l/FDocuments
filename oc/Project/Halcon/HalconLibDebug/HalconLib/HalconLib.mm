//
//  HalconLib.m
//  HalconLib
//
//  Created by FoLeakey on 6/28/16.
//  Copyright(c)2016 IntelligentGroup. All rights reserved.
//
#include "HalconLib.h"

using namespace std;

HalconLib::HalconLib(){
    sprintf(sRootPath,"/vault/AOI_HALCON");
}
HalconLib::~HalconLib(){
    
}
void HalconLib::setRootPath(const char *pPath){
    sprintf(sRootPath,"%s",pPath);
}
void HalconLib::initWindow(NSView *View){
    try{
        NSRect frame=[View frame];
        OpenWindow(0,0,frame.size.width,frame.size.height,(Hlong)(View),"visible","",&WinHandle);
        SetPart(WinHandle,0,0,1228 ,1624);
    }catch(...){
        cout<<"Failed to init Window."<<endl;
        exit(0);
    }
}
void HalconLib::displayPic(HImage pImage){
    sImage=pImage.CopyImage();
    DispObj(sImage,WinHandle);
}
void HalconLib::displayPic(const char *pFilePath){
    ReadImage(&sImage,pFilePath);
    displayPic(sImage);
}
void HalconLib::addRegion(const char *pRegion){
    sRegion.push_back(pRegion);
}
HTuple hv_HomMat2D;
HObject  ho_ModelContours,ho_TransContours;
BOOL HalconLib::trainModel(){
    return trainModel(sImage,TRUE,FALSE,NULL,NULL);
}
BOOL HalconLib::trainModel(HImage pImage){
    return trainModel(pImage,TRUE,FALSE,NULL,NULL);
}
BOOL HalconLib::trainModel(HImage pImage,int pIndex,BOOL isDraw,BOOL isSave){
    return trainModel(pImage,isDraw,isSave,NULL,pIndex);
}
BOOL HalconLib::trainModel(HImage pImage,BOOL isDraw,BOOL isSave,const char *pRegion,int pIndex){
    HObject ho_ModelRegion;
    HObject ho_TemplateImage;
    HTuple hv_ModelRegionArea;
    HTuple hv_RefRow,hv_RefColumn;
    
    try{
        SetSystem("border_shape_models","false");
        SetColor(WinHandle,"green");
        SetDraw(WinHandle,"margin");
        
        if(isDraw){
            HTuple Row_Start,Row_End,Col_Start,Col_End;
            DrawRectangle1(WinHandle,&Row_Start,&Col_Start,&Row_End,&Col_End);
            GenRectangle1(&ho_ModelRegion,Row_Start,Col_Start,Row_End,Col_End);
        }else{
            char pParaPath[255];
            sprintf(pParaPath,"%s/Templates/Para/Region_%02d.hobj",sRootPath,pIndex);
            ReadObject(&ho_ModelRegion,pParaPath);
        }
        ReduceDomain(pImage,ho_ModelRegion,&ho_TemplateImage);
        
        CreateScaledShapeModel(ho_TemplateImage,4,HTuple(0).TupleRad(),HTuple(360).TupleRad(),HTuple(0.1626).TupleRad(),0.85,1.15,0.0028,(HTuple("point_reduction_high").Append("no_pregeneration")),"use_polarity",((HTuple(50).Append(120)).Append(30)),5,&TemplateModel);
        
        GetShapeModelContours(&ho_ModelContours,TemplateModel,1);
        
        AreaCenter(ho_ModelRegion,&hv_ModelRegionArea,&hv_RefRow,&hv_RefColumn);
        VectorAngleToRigid(0,0,0,hv_RefRow,hv_RefColumn,0,&hv_HomMat2D);
        AffineTransContourXld(ho_ModelContours,&ho_TransContours,hv_HomMat2D);
        
        DispObj(pImage,WinHandle);
        DispObj(ho_ModelRegion,WinHandle);
        DispObj(ho_TransContours,WinHandle);
    }catch(...){
        cout<<"Failed to train."<<endl;
        return FALSE;
    }
    if(isSave){
        try{
            char pParaPath[255];
            sprintf(pParaPath,"%s/Templates/Para/Region_%02d",sRootPath,pIndex);
            checkPath(pParaPath);
            WriteObject(ho_ModelRegion,pParaPath);
        }catch(...){
            cout<<"Failed to save region."<<endl;
        }
    }
    return TRUE;
}
BOOL HalconLib::saveModel(const char *pRegion,int pIndex){
    char pModelPath[255];
    char pParaPath[255];
    sprintf(pModelPath,"%s/Templates/%s/%02d.shm",sRootPath,pRegion,pIndex);
    checkPath(pModelPath);
    sprintf(pParaPath,"%s/Templates/Para/%s/%02d.shm",sRootPath,pRegion,pIndex);
    checkPath(pParaPath);
    try{
        WriteShapeModel(TemplateModel,pModelPath);
        ClearShapeModel(TemplateModel);
    }catch(...){
        cout<<"Failed to save model."<<endl;
        return FALSE;
    }
    try{
        sprintf(pParaPath,"%s/Templates/Para/%s/HomMat2D_%02d.tup",sRootPath,pRegion,pIndex);
        checkPath(pParaPath);
        WriteTuple(hv_HomMat2D,pParaPath);
        hv_HomMat2D.Clear();
        sprintf(pParaPath,"%s/Templates/Para/%s/ModelContours_%02d",sRootPath,pRegion,pIndex);
        checkPath(pParaPath);
        WriteObject(ho_ModelContours,pParaPath);
        ho_ModelContours.Clear();
        sprintf(pParaPath,"%s/Templates/Para/%s/TransContours_%02d",sRootPath,pRegion,pIndex);
        checkPath(pParaPath);
        WriteObject(ho_TransContours,pParaPath);
        ho_TransContours.Clear();
    }catch(...){
        cout<<"Failed to save para."<<endl;
    }
    return TRUE;
}
BOOL HalconLib::modelTest(HImage pImage,const char *pRegion,int pIndex){
    char msg[255];
    HTuple hv_Row,hv_Column,hv_Angle,hv_Scale,hv_I;
    HTuple pScore;
    sResult.clear();
    for(int i=0;i<sRegion.size();i++){
        //        char pModelPath[255];
        //        sprintf(pModelPath,"/vault/AOI_HALCON/Templates/%s/%02d.shm",sRegion[i].c_str(),pIndex);
        try{
            //            ReadShapeModel(pModelPath,&TemplateModel);
            //            FindScaledShapeModel(pImage,TemplateModel,HTuple(0).TupleRad(),HTuple(0).TupleRad(),0.85,1.15,0.6,0,0.5,"least_squares",(HTuple(4).Append(1)),0.9,&hv_Row,
            //                                 &hv_Column,&hv_Angle,&hv_Scale,&pScore);
            //            ClearShapeModel(TemplateModel);
            pScore=detectSample(pImage,sRegion[i].c_str(),pIndex,&hv_Row,&hv_Column,&hv_Angle,&hv_Scale,&hv_I);
            
            DispObj(pImage,WinHandle);
            
            HTuple end_val32=(pScore.TupleLength())-1;
            HTuple step_val32=1;
            HTuple pHomMat2D;
            HObject pModelContour,pTransContour;
            
            char pParaPath[255];
            sprintf(pParaPath,"%s/Templates/Para/%s/HomMat2D_%02d.tup",sRootPath,pRegion,pIndex);
            ReadTuple(pParaPath,&pHomMat2D);
            sprintf(pParaPath,"%s/Templates/Para/%s/ModelContours_%02d",sRootPath,pRegion,pIndex);
            ReadObject(&pModelContour,pParaPath);
            sprintf(pParaPath,"%s/Templates/Para/%s/TransContours_%02d",sRootPath,pRegion,pIndex);
            ReadObject(&pTransContour,pParaPath);
            
            if(pScore.Length()>0){
                for(hv_I=0;hv_I.Continue(end_val32,step_val32);hv_I+=step_val32){
                    HomMat2dIdentity(&pHomMat2D);
                    HomMat2dScale(pHomMat2D,HTuple(hv_Scale[hv_I]),HTuple(hv_Scale[hv_I]),
                                  0,0,&pHomMat2D);
                    HomMat2dRotate(pHomMat2D,HTuple(hv_Angle[hv_I]),0,0,&pHomMat2D);
                    HomMat2dTranslate(pHomMat2D,HTuple(hv_Row[hv_I]),HTuple(hv_Column[hv_I]),
                                      &pHomMat2D);
                    AffineTransContourXld(pModelContour,&pTransContour,pHomMat2D);
                    sResult.push_back(pTransContour);
                }
                
                sprintf(msg,"%s-%s-%02d-%.2f\n",pRegion,sRegion[i].c_str(),pIndex,pScore[0].D());
                SaveDebug(msg);
            }else{
                sResult.push_back((HObject)NULL);
                sprintf(msg,"%s-%s-%02d-%.2f\n",pRegion,sRegion[i].c_str(),pIndex,0.0);
                SaveDebug(msg);
            }
        }catch(...){
            printf("Failed to test %s.\n",sRegion[i].c_str());
            continue;
        }
    }
    return TRUE;
}
void HalconLib::showResult(int pImgIndex,int pIndex){
    char pFilePath[255];
    try{
        sprintf(pFilePath,"%s/TrainImage/%s/Train_Image_%02d.bmp",sRootPath,sRegion[pIndex].c_str(),pImgIndex);
        ReadImage(&sImage,pFilePath);
        DispImage(sImage,WinHandle);
        sResult[pIndex].CountObj();
        DispObj(sResult[pIndex],WinHandle);
    }catch(...){
        printf("Failed to show result of %s.\n",sRegion[pIndex].c_str());
    }
}
HTuple HalconLib::detectSample(HImage pImage,const char *pRegion,int pIndex){
    HTuple hv_Row,hv_Column,hv_Angle,hv_Scale,hv_I;
    return detectSample(pImage,pRegion,pIndex,&hv_Row,&hv_Column,&hv_Angle,&hv_Scale,&hv_I);
}
HTuple HalconLib::detectSample(HImage pImage,const char *pRegion,int pIndex,HTuple *hv_Row,HTuple *hv_Column,HTuple *hv_Angle,HTuple *hv_Scale,HTuple *hv_I){
    char pModelPath[255];
    HTuple pScore;
    sprintf(pModelPath,"%s/Templates/%s/%02d.shm",sRootPath,pRegion,pIndex);
    ReadShapeModel(pModelPath,&TemplateModel);
    FindScaledShapeModel(pImage,TemplateModel,HTuple(0).TupleRad(),HTuple(0).TupleRad(),0.85,1.15,0.6,0,0.5,"least_squares",(HTuple(4).Append(1)),0.9,hv_Row,
                         hv_Column,hv_Angle,hv_Scale,&pScore);
    ClearShapeModel(TemplateModel);
    return pScore;
}
void SaveDebug(const char *msg){
    cout<<msg;
    char filepath[255];
    sprintf(filepath,"/vault/AOI_HALCON/TestResult.txt");
    ofstream dbf(filepath,ios_base::out|ios_base::app);
    dbf.write(msg,strlen(msg));
    dbf.close();
}
void checkPath(char *pFilePath){
    NSString *path=[[NSString stringWithFormat:@"%s",pFilePath] stringByDeletingLastPathComponent];
    NSFileManager *pFileManager=[NSFileManager defaultManager];
    [pFileManager createDirectoryAtPath:path withIntermediateDirectories:YES attributes:nil error:nil];
}
