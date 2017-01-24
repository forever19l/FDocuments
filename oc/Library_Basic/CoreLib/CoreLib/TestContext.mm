//
//  TestContext.cpp
//  CoreLib
//
//  Created by Ryan on 12-12-26.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "TestContext.h"



NSMutableDictionary * CTestContext::m_dicGlobal =nil;
NSMutableDictionary * CTestContext::m_dicConfiguration =nil;

CTestContext::CTestContext()
{
    m_dicContext = [NSMutableDictionary new];
    if (m_dicGlobal == nil)
    {
        m_dicGlobal = [NSMutableDictionary new];
    }
    
    if (m_dicConfiguration == nil)
    {
        m_dicConfiguration = [NSMutableDictionary new];
    }
}

CTestContext::~CTestContext()
{
    [m_dicContext release];
    m_dicContext = nil;
    [m_dicGlobal release];
    m_dicGlobal = nil;
    [m_dicConfiguration release];
    m_dicConfiguration = nil;
}

const char * CTestContext::getContext(char *szkey,int index) const
{
    id value=nil;
    

    switch (index) {
        case 0:
            value = [m_dicContext valueForKey:[NSString stringWithUTF8String:szkey]];
            break;
        case 1:
            value = [m_dicGlobal valueForKey:[NSString stringWithUTF8String:szkey]];
            break;
        case 2:
            value = [m_dicConfiguration valueForKey:[NSString stringWithUTF8String:szkey]];
            break;
            
        default:
            break;
    }
    
//    NSLog(@"getContext('%s',%d,value=%@",szkey,index,value);
    if ([value isKindOfClass:[NSString class]])
    {
        return [value UTF8String];
    }
    else
    {
        return [[value stringValue] UTF8String];
    }
    return NULL;
}

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

-(void)IntialStation
{
    NSString * strRestore = [NSString stringWithContentsOfFile:@"/Users/Ryan/Restore Info.txt" encoding:NSASCIIStringEncoding error:nil];
    station.cbsize =sizeof(StationInfo);
    const char * pstring = [[strRestore stringByMatching:@"STATION=\\s*(.*)[^.]?" capture:1] UTF8String];
    strcpy(station.stationName,pstring);
    pstring = [[strRestore stringByMatching:@"LINE=\\s*(.*)[^.]?" capture:1] UTF8String];
    strcpy(station.lineName, pstring);
    pstring = [[strRestore stringByMatching:@"PDCA_SERVER=\\s*(.*)[^.]?" capture:1] UTF8String];
    strcpy(station.pdcaServer, pstring);
    pstring = [[strRestore stringByMatching:@"SFC_SERVER=\\s*(.*)[^.]?" capture:1] UTF8String];
    strcpy(station.SFC, pstring);
    
    UUT_INFOR * pContext[] = {&UUT_CONTEXT0,&UUT_CONTEXT1,&UUT_CONTEXT2,&UUT_CONTEXT3};
    
    for (int i=0; i<=3; i++) {
        pContext[i]->cbsize = sizeof(UUT_CONTEXT0);
        pContext[i]->sitID = i;
        pContext[i]->USB_Location = 0xFA121000+0X1000*i;
        strcpy(pContext[i]->cfg,"no cfg number");
        strcpy(pContext[i]->mlbsn, "no serial number");
    }
}*/


//@implementation MyMutableDictionary
////- (id)valueForKey:(NSString *)key
////{
////    id ret=[super valueForKey:key];
////    if ([key isEqualToString:kContextMLBSN]) {
////        NSLog(@"valueForKey kContextMLBSN:%@",ret);
////    }
////    return ret;
////}
//- (void)setValue:(id)value forKey:(NSString *)key
//{
//    if ([key isEqualToString:kContextMLBSN]) {
//        NSLog(@"setValue kContextMLBSN:%@",value);
//    }
//}
//@end
