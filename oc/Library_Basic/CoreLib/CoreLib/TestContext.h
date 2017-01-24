//
//  TestContext.h
//  CoreLib
//
//  Created by Ryan on 12-12-26.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//


/*
 RESTORE_DATE=Mon Dec 10 03:31:18 UTC 2012
 UPDATE_RESTORE=0
 PRODUCT=J71
 LINE=G02-3FT-08A
 STATION=J71 FCT
 UNIT=J71 FCT
 IP_ADDRESS=172.30.82.195
 MAC_ADDRESS=a8:20:66:15:b7:3b
 GROUNDHOG_SERVER=172.30.240.104
 NTP_SERVER=17.239.208.55
 DCS_SERVER=17.239.212.199
 PDCA_SERVER=17.239.212.199
 KOMODO_SERVER=
 ACTIVATION_SERVER=17.239.208.197
 FUSING_SERVER=17.239.208.195
 DROPBOX_SERVER=
 SFC_SERVER=172.16.241.131
 SFC_URL=http://172.16.241.131/aa
 PROV_SERVER=17.239.208.210
 AV_SERVER=172.30.119.55
 IMAGE_SOURCE=afp://gdadmin:gdadmin@172.30.240.104/images
 OSX_IMAGE=Lion [default in ]
 WINXP_IMAGE=
 OVERLAY_SOURCE=afp://gdadmin:gdadmin@172.30.240.104/overlays
 OSX_OVERLAY_VERSION=j71/default-osx [default in j71/default-osx]
 WINXP_OVERLAY_VERSION=
 STATION_OVERLAY_VERSION=20121204_012614_j71 fct_SochiYabuli11A32070h_9D127i_RamDisk-v13 [default in j71/j71 fct]
 CREATE_PUDDING_WEBSERVICES_FILE=0
 MODEL_IDENTIFIER=Macmini5,2
 USE_HEDGEHOG=1
 STORE_VAULTS_IN_COMPRESSED_FORMAT=1
 SCRIPTBASE_VERSION=30
 
 */



#define kContextStationName     @"StationName"
#define kContextStationID       @"StationID"
#define kContextStationType     @"StationType"
#define kContextLineName        @"LineName"
#define kContextLineNumber      @"LineNumber"
#define kContextStationNumber   @"StationNumber"
#define kContextVaultPath       @"VaultPath"
#define kContextCsvPath         @"CsvLogPath"
#define kContextUartPath        @"UartLogPath"
#define kContextRawDataPath     @"RawDataLogPath"
#define kContextTestFlow        @"TestFlowPath"
#define kContextPdcaServer      @"PDCA_Server"
#define kContextSfcServer       @"SFC_server"
#define kContextSfcURL          @"SFC_URL"
#define kContextAppDir          @"Application_Dir"
#define kContextTMVersion       @"TM_Version"

#define kContextID              @"uid"
#define kContextUsbLocation     @"USBlocation"
#define kContextMLBSN           @"MLB_SN"
#define kContextMLBSN_A           @"MLB_SN_A"
#define kContextMLBSN_B           @"MLB_SN_B"
#define kContextCFG             @"CFG"
#define kContextStartTime       @"StartTime"
#define kContextStopTime        @"StopTime"
#define kContextTotalTime       @"TotalTime"
#define kContextEnableTest      @"IsEnableTest"


#define kContextPanelSN         @"kContextPanelSN"
#define kContextBuildEvent      @"kContextBuildEvent"
#define kContextSBuild          @"kContextSBuil"


#define kContextFixtureID       @"FixtureID"

#define kContextCBAuthStationNameToCheck    @"CBAuthStationNameToCheck"
#define kContextCBAuthNumberToCheck         @"CBAuthNumberToCheck"
#define kContextCBAuthMaxFailForStation     @"CBAuthMaxFailForStation"
#define kContextCBAuthToClearOnPass         @"CBAuthToClearOnPass"
#define kContextCBAuthToClearOnFail         @"CBAuthToClearOnFail"
#define kContextCBAuthStationSetControlBit  @"CBAuthStationSetControlBit"

#ifndef CoreLib_TestContext_h
#define CoreLib_TestContext_h

#import <Foundation/Foundation.h>

//@interface MyMutableDictionary : NSMutableDictionary
//- (id)valueForKey:(NSString *)key;
//- (void)setValue:(id)value forKey:(NSString *)key;
//@end

class CTestContext {
public:
    CTestContext();
    ~CTestContext();
    
public:
    const char * getContext(char * szkey,int index=0) const;     //0:testcontext,1:gloabl information,2:configuuration

public:
    static NSMutableDictionary * m_dicGlobal;
    static NSMutableDictionary * m_dicConfiguration;
    NSMutableDictionary * m_dicContext;
};


#endif
