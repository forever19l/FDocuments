//
//  AppDelegate.h
//  HalconLib
//
//  Created by FoLeakey on 6/28/16.
//  Copyright (c) 2016 IntelligentGroup. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "HalconLib.h"

@interface AppDelegate : NSObject <NSApplicationDelegate>{
@private
    IBOutlet NSWindow *p_Win;
    IBOutlet NSView *p_View;
    IBOutlet NSTextField *v_Region;
    IBOutlet NSTextField *v_Index;
    IBOutlet NSComboBox *p_Combox;
    int vRegion,vIndex;
    int ImgIndex;
    HImage mImage;
    NSString *mRegion;
    int mIndex;
    NSArray *TemplateArray;
    BOOL isTrain;
    BOOL isLOCK;
@public
    HalconLib halobj;
}
-(IBAction)btnOnClick:(id)sender;
-(IBAction)btnShowResult:(id)sender;
-(IBAction)btnListPic:(id)sender;
-(IBAction)btnDrawRect:(id)sender;
@end
