//
//  UICommon.h
//  MainUI
//
//  Created by Ryan on 12/3/15.
//  Copyright © 2015 ___Intelligent Automation___. All rights reserved.
//

#ifndef UICommon_h
#define UICommon_h

#define crPASS  [NSColor greenColor]
#define crFAIL  [NSColor redColor]
#define crRUN   [NSColor blueColor]
#define crNA    [NSColor grayColor]
#define crERROR [NSColor yellowColor]
//#define crIDLE  [NSColor selectedTextBackgroundColor]
#define crIDLE  [NSColor lightGrayColor]

#define szRUN   @"Testing"
#define szPASS  @"Pass"
#define szFAIL  @"Fail"
#define szERROR @"Error"
#define szIDLE  @"Idle"
#define szNA    @"N/A"

#define keyCSV  @"csvPath"
#define keySTATION  @"StationName"
#define keyPORT @"ComPort"
#define keySETTING  @"ComSetting"

#define keyITEM1 @"Item1"
#define keyITEM2 @"Item2"
#define keyITEM3 @"Item3"
#define keyNAME     @"Name"
#define keyUPPER    @"Upper"
#define keyLOWER    @"Lower"
#define keyUNIT     @"Unit"


#define keyRESULT    @"Result"
#define keySTARTTIME    @"TestStartTime"
#define keyTOTALTIME    @"TotalTestTime"
#define keySTATIONID    @"StationID"
#define keySITEID    @"SiteID"
#define keySTATUS    @"Status"
#define keyFAILITEM @"FailItem"
#define keySN   @"sn"
#define keyERRORMSG   @"errormsg"


#define kERRORDOMAIN @"IA.Error"


typedef enum _Author
{
    OPERATOR = 0,
    ADMIN,
}AUTHOR;

#endif /* UICommon_h */
