//
//  AppDelegate.m
//  HalconLib
//
//  Created by FoLeakey on 6/28/16.
//  Copyright (c) 2016 IntelligentGroup. All rights reserved.
//

#import "AppDelegate.h"
#define LOADPIC 10
#define TRAIN 20
#define SAVE 30
#define MTEST 40
#define UPBTN 101
#define DOWNBTN 102
#define LBTN 103
#define RBTN 104
#define OBTN 100

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    // Insert code here to initialize your application
}
-(void)awakeFromNib{
    halobj.initWindow(p_View);
    isLOCK=FALSE;
    isTrain=FALSE;
    TemplateArray=[self lstPath:@"/vault/AOI_HALCON/TrainImage/"];
    vRegion=0;
    vIndex=0;
}
-(IBAction)btnOnClick:(id)sender{
    NSInteger n_tag=[sender tag];
    switch (n_tag) {
        case LOADPIC:{
                NSOpenPanel *openPanel = [NSOpenPanel openPanel];
                [openPanel setCanChooseFiles:YES];
                [openPanel setCanChooseDirectories:NO];
                [openPanel setAllowedFileTypes:[NSImage imageFileTypes]];
                
                [openPanel setMessage:@"Select a image to display."];
                [openPanel beginSheetModalForWindow:[NSApp mainWindow] completionHandler:^(NSInteger result) {
                    if (result == NSFileHandlingPanelOKButton){
                        NSArray *pPath=[[openPanel filename] componentsSeparatedByString:@"/"];
                        mRegion=[NSString stringWithFormat:@"%@",[pPath objectAtIndex:4]];
                        mIndex=[[[pPath objectAtIndex:5] substringWithRange:NSMakeRange(12, 2)]intValue];
                        ReadImage(&mImage, [[openPanel filename] UTF8String]);
                        halobj.displayPic(mImage);
                        NSLog(@"%@/%02d\n",mRegion,mIndex);
                    }
                }];
            break;
        }
        case TRAIN:{
            halobj.trainModel();
            break;
        }
        case SAVE:{
            halobj.saveModel([mRegion UTF8String], mIndex);
            break;
        }
        case MTEST:{
            halobj.modelTest(mImage, [mRegion UTF8String], mIndex);
            break;
        }
        default:{
            break;
        }
    }
}
-(IBAction)btnShowResult:(id)sender{
    halobj.showResult(mIndex,[TemplateArray indexOfObject:[p_Combox stringValue]]);
}
-(NSArray*)lstPath:(NSString*)Path{
    try {
        NSFileManager *pFileManager = [NSFileManager defaultManager];
        NSMutableArray *pArray = [pFileManager contentsOfDirectoryAtPath:Path error:nil];
        for (int i=0;i<[pArray count]; i++) {
            if ([[pArray objectAtIndex:i]isEqualToString:@".DS_Store"]) {
                [pArray removeObjectAtIndex:i];
            }
            halobj.addRegion([[pArray objectAtIndex:i]UTF8String]);
        }
        [p_Combox addItemsWithObjectValues:pArray];
        NSLog(@"array=%@",pArray);
        return pArray;
    }
    catch (...) {
        NSLog(@"Path error.\n");
        return nil;
    }
}
-(IBAction)btnSegCtrl:(id)sender{
    if (1==[sender selectedSegment]) {
        isTrain=TRUE;
    }else{
        isTrain=FALSE;
    }
}
-(IBAction)btnListPic:(id)sender{
    switch ([sender tag]) {
        case OBTN:
            vRegion=0;
            vIndex=0;
            break;
        case UPBTN:
            if (vRegion<([TemplateArray count]-1)) {
                vRegion++;
            }
            break;
        case DOWNBTN:
            if (vRegion>0){
                vRegion--;
            }
            break;
        case LBTN:
            if (vIndex>0){
                vIndex--;
            }else{
                if (vRegion>0) {
                    vRegion--;
                    vIndex=22;
                }
            }
            break;
        case RBTN:
            if (vIndex<22){
                vIndex++;
            }else{
                if (vRegion<([TemplateArray count]-1)) {
                    vRegion++;
                    vIndex=0;
                }
            }
            break;
        default:
            break;
    }
    char pFilePath[255];
    sprintf(pFilePath, "/vault/AOI_HALCON/TrainImage/%s/Train_Image_%02d.bmp",[[TemplateArray objectAtIndex:vRegion]UTF8String],vIndex);
    [v_Region setStringValue:[TemplateArray objectAtIndex:vRegion]];
    [v_Index setStringValue:[NSString stringWithFormat:@"%02d",vIndex]];
    halobj.displayPic(pFilePath);
    if (isTrain) {
        HalconCpp::HImage pImage;
        ReadImage(&pImage, pFilePath);
        halobj.trainModel(pImage,vIndex,FALSE,FALSE);
    }
}
-(IBAction)btnDrawRect:(id)sender{
    switch ([sender tag]) {
        case OBTN:
            vRegion=0;
            vIndex=0;
            break;
        case LBTN:
            vRegion=0;
            if (vIndex>0){
                vIndex--;
            }else{
                return;
            }
            break;
        case RBTN:
            vRegion=0;
            if (vIndex<22){
                vIndex++;
            }else{
                return;
            }
            break;
        default:
            break;
    }
    char pFilePath[255];
    sprintf(pFilePath, "/vault/AOI_HALCON/TrainImage/%s/Train_Image_%02d.bmp",[[TemplateArray objectAtIndex:vRegion]UTF8String],vIndex);
    HImage pImage;
    ReadImage(&pImage, pFilePath);
    halobj.displayPic(pImage);
    halobj.trainModel(pImage, vIndex);
}
-(IBAction)btnAutoOTS:(id)sender{
    [NSThread detachNewThreadSelector:@selector(AutoOTS) toTarget:self withObject:nil];
}
-(void)AutoOTS{
    for (int pRegion=0; pRegion<[TemplateArray count]; pRegion++) {
        for (int pIndex=0; pIndex<23; pIndex++) {
            [v_Region setStringValue:[TemplateArray objectAtIndex:pRegion]];
            [v_Index setStringValue:[NSString stringWithFormat:@"%02d",pIndex]];
            char pFilePath[255];
            sprintf(pFilePath, "/vault/AOI_HALCON/TrainImage/%s/Train_Image_%02d.bmp",[[TemplateArray objectAtIndex:pRegion]UTF8String],pIndex);
            HImage pImage;
            ReadImage(&pImage, pFilePath);
            halobj.trainModel(pImage,pIndex,FALSE,FALSE);
            halobj.saveModel([[TemplateArray objectAtIndex:pRegion]UTF8String],pIndex);
            usleep(200000);
        }
    }
}
int DetectQueen[5]={0,1,2,9,10};
-(IBAction)btnDetectAll:(id)sender{
    [NSThread detachNewThreadSelector:@selector(DetectAll) toTarget:self withObject:nil];
}
-(void)DetectAll{
    for (int pRegion=0; pRegion<[TemplateArray count]; pRegion++) {
        for (int pIndex=0; pIndex<5; pIndex++) {
            [v_Region setStringValue:[TemplateArray objectAtIndex:pRegion]];
            [v_Index setStringValue:[NSString stringWithFormat:@"%02d",DetectQueen[pIndex]]];
            char pFilePath[255];
            sprintf(pFilePath, "/vault/AOI_HALCON/TrainImage/%s/Train_Image_%02d.bmp",[[TemplateArray objectAtIndex:pRegion]UTF8String],DetectQueen[pIndex]);
            HImage pImage;
            ReadImage(&pImage, pFilePath);
            for (int pModel=0; pModel<[TemplateArray count]; pModel++) {
                HTuple pScore=halobj.detectSample(pImage, [[TemplateArray objectAtIndex:pModel]UTF8String], DetectQueen[pIndex]);
                char msg[255];
                if (pScore.Length()>0) {
                    sprintf(msg, "%s-%s-%02d-%.2f\n",[[TemplateArray objectAtIndex:pRegion]UTF8String],[[TemplateArray objectAtIndex:pModel]UTF8String], DetectQueen[pIndex],pScore[0].D());
                    SaveDebug(msg);
                }else{
                    sprintf(msg, "%s-%s-%02d-%.2f\n",[[TemplateArray objectAtIndex:pRegion]UTF8String],[[TemplateArray objectAtIndex:pModel]UTF8String], DetectQueen[pIndex],0.0);
                    SaveDebug(msg);
                }
            }
        }
    }
}
@end