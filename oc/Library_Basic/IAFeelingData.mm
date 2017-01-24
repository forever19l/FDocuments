//
//  IAFeelingData.m
//  IANetFixture
//
//  Created by ZL-Pro on 16/6/26.
//  Copyright © 2016年 Louis. All rights reserved.
//
#import "CommandStruct.h"
#import "IAFeelingData.h"
#include <cstdio>
using namespace std;

//const char FeelingVersion[]="Version:,Intelligent_Feeling_Test_Data_20160625V1";

const char FeelingVersion[]="Version:,Intelligent Feeling Test Data 20160625V1";

@implementation IAFeelingData
+(BOOL)SavePLCDatatoFile:(void *)pVoid
              LineNumber:(const char*)LineNumber
           StationNumber:(const char*)StationNumber
            SerialNumber:(const char*)mlbSN
                Platfrom:(const char*)Platfrom
                 KeyName:(const char*)KeyName
                Filename:(const char*)file
{
    g_PLCStruct*PLCdata=(g_PLCStruct*)pVoid;
    FILE* f1 = fopen(file, "wt");
    //    char buffter[255];
    if(!f1)
    {
        return NO;
    }
    fprintf(f1, "%s\n",FeelingVersion);
    fprintf(f1, "LineNumber:,%s\n",LineNumber);
    fprintf(f1, "StationNumber:,%s\n",StationNumber);
    fprintf(f1, "SerialNumber:,%s\n",mlbSN);
    fprintf(f1, "Platform:,%s\n",Platfrom);
    fprintf(f1, "KeyName:,%s\n",KeyName);
    fprintf(f1, "Test MarketData:,");
    for (int i=0; i<14; i++) {
        fprintf(f1, "%.3f,",PLCdata->markdata[i]);
    }
    fprintf(f1, "%.3f\n",PLCdata->markdata[14]);
    unsigned int fLen=PLCdata->flen;
    fprintf(f1, "FeelingCurve Data longht:%d\nTrave,Force1,Force2,Force3\n",fLen);
    for (int i=0; i<fLen; i++)
    {
        fprintf(f1, "%.3f,%.3f,%.3f,%.3f\n",1.0*PLCdata->time[i],1.0*PLCdata->line1[i],1.0*PLCdata->line2[i],1.0*PLCdata->line3[i]);
    }
    fclose(f1);
    return YES;
}
+(BOOL)SaveLocalDatatoFile:(void*)pVoid
                LineNumber:(const char*)LineNumber
                StationNumber:(const char*)StationNumber
                SerialNumber:(const char*)mlbSN
                  Platfrom:(const char*)Platfrom
                  KeyName:(const char*)KeyName
                  Filename:(const char*)file
{
    g_LOADCELLDATA*loadcelldata=(g_LOADCELLDATA*)pVoid;
    FILE* f1 = fopen(file, "wt");
//    char buffter[255];
     if(!f1)
     {
         return NO;
     }
    fprintf(f1, "%s\n",FeelingVersion);
    fprintf(f1, "LineNumber:,%s\n",LineNumber);
    fprintf(f1, "StationNumber:,%s\n",StationNumber);
    fprintf(f1, "SerialNumber:,%s\n",mlbSN);
    fprintf(f1, "Platfrom:,%s\n",Platfrom);
    fprintf(f1, "KeyName:,%s\n",KeyName);
    fprintf(f1, "Test MarketData:,");
    for (int i=0; i<14; i++) {
        fprintf(f1, "%.3f,",loadcelldata->markdata[i]);
    }
    fprintf(f1, "%.3f\n",loadcelldata->markdata[14]);
    unsigned int fLen=(loadcelldata->dataleng/4-15)/2;
    fprintf(f1, "FeelingCurve Data longht:%d\nTrave,Force\n",fLen);
    for (int i=0; i<fLen; i++)
    {
        fprintf(f1, "%.3f,%.3f\n",loadcelldata->rawdata[i+fLen],loadcelldata->rawdata[i]);
    }
    fclose(f1);
    return YES;
}
+(int)ReadLocalDatatoFile:(void*)pVoid
                LineNumber:(char*)LineNumber
             StationNumber:(char*)StationNumber
              SerialNumber:(char*)mlbSN
                  Platfrom:(char*)Platfrom
                   KeyName:(char*)KeyName
                  Filename:(const char*)file
{
    g_LOADCELLDATA*loadcelldata=(g_LOADCELLDATA*)pVoid;
    float tmp1,tmp2;
    FILE* f1 = fopen(file, "rt");
    char buffter[255];
    memset(buffter, 0, 255);
    if(!f1)
    {
        return -1;
    }
    size_t len=fread(buffter,sizeof(char),strlen(FeelingVersion)+1,f1);
    if (len<strlen(FeelingVersion))
    {
        fclose(f1);
        return -2;
    }
    if (strncmp(buffter, FeelingVersion,strlen(FeelingVersion))!=0)
    {
        fclose(f1);
        return -3;
    }
    fscanf(f1, "LineNumber:,%s\n",LineNumber);
    fscanf(f1, "StationNumber:,%s\n",StationNumber);
    fscanf(f1, "SerialNumber:,%s\n",mlbSN);
    fscanf(f1, "Platfrom:,%s\n",Platfrom);
    fscanf(f1, "KeyName:,%s\n",KeyName);
    fscanf(f1, "Test MarketData:,");
    
    for (int i=0; i<14; i++) {
        fscanf(f1, "%f,",&tmp1);
        loadcelldata->markdata[i]=tmp1;
    }
    fscanf(f1, "%f\n",&tmp1);
    loadcelldata->markdata[14]=tmp1;
    
    unsigned int fLen;//=(loadcelldata->dataleng/4-15)/2;
    fscanf(f1, "FeelingCurve Data longht:%d\nTrave,Force\n",&fLen);
    loadcelldata->dataleng=((fLen*2)+15)*4;
    for (int i=0; i<fLen; i++)
    {
        fscanf(f1, "%f,%f\n",&tmp1,&tmp2);
        loadcelldata->rawdata[i+fLen]=tmp1;
        loadcelldata->rawdata[i]=tmp2;
    }
    fclose(f1);
    return 0;
}
@end
