//
//  CIANetFixture.cpp
//  IANetFixture
//
//  Created by Louis on 13-11-13.
//  Copyright (c) 2013年 Louis. All rights reserved.
//
//#include "TestContext.h"
#import <CoreLib/Common.h>
#include "CIANetFixture.h"
#import "Commad.h"

extern RunHalcon *g_PrintDetect;

CIANetFixture::CIANetFixture()
{
    SF_Login();
    dic_JSON = [[NSMutableDictionary alloc] init];
    test_content = [[NSMutableDictionary alloc] init];
    Position_items_test = [[NSMutableArray alloc]init];
    Position_items_camera = [[NSMutableArray alloc]init];
    Position_items_cxy = [[NSMutableArray alloc]init];
    Position_items_cal = [[NSMutableArray alloc]init];
    CIANetFixture::ReadPosition();
}
CIANetFixture::~CIANetFixture()
{
    [dic_JSON release];
    [test_content release];
    [Position_items_test release];
    [Position_items_camera release];
    [Position_items_cxy release];
    [Position_items_cal release];
}

void SaveDebug(NSString *msg,const char *str,int line)
{
    NSDate *date=[NSDate date];
    NSCalendar *cal=[NSCalendar currentCalendar];
    unsigned int unitflags=NSYearCalendarUnit|NSMonthCalendarUnit|NSDayCalendarUnit|NSHourCalendarUnit|NSMinuteCalendarUnit|NSSecondCalendarUnit;
    NSDateComponents *d=[cal components:unitflags fromDate:date];
    long year=[d year];
    long month=[d month];
    long day=[d day];
    long hour=[d hour];
    long minute=[d minute];
    long second=[d second];
    
    NSString *tmp1 = [NSString stringWithFormat:@"%02ld.%02ld:%02ld %s:%d %@\n",hour,minute,second,str,line,msg];
    NSData *tmp2 = [tmp1 dataUsingEncoding:NSUTF8StringEncoding];
    NSString *path = [NSString stringWithFormat:@"/vault/Intelli_PM/Logs/debugfile-%ld-%02ld-%02ld.txt",year,month,day];
    NSFileManager *manager = [NSFileManager defaultManager];
    BOOL isDir, valid = [manager fileExistsAtPath:path isDirectory:&isDir];
    if(valid)
    {
        NSFileHandle *handle = [NSFileHandle fileHandleForWritingAtPath:path];
        [handle seekToEndOfFile];
        [handle writeData:tmp2];
        [handle closeFile];
    }
    else
    {
        [manager createFileAtPath:path contents:tmp2 attributes:nil];
    }
}

void CIANetFixture::StartTest()
{
    NSNotificationCenter * nf = [NSNotificationCenter defaultCenter];
    [nf postNotificationName:kNotificationDoTestStart object:nil];
    return;
}
void CIANetFixture::CancelTest()
{
    NSNotificationCenter * nf = [NSNotificationCenter defaultCenter];
    [nf postNotificationName:kNotificationDoTestStop object:nil];
    return;
}

void CIANetFixture::Read(){
}
void CIANetFixture::Write(){
}

int CIANetFixture::getTestPlatNum()
{
    return [[CTestContext::m_dicConfiguration valueForKey:@"m_testerpanel"] intValue];
}
int CIANetFixture::getTestDutNum(int plat)
{
    if (plat==1) {
        return [[CTestContext::m_dicConfiguration valueForKey:@"LEFT_DUT"]intValue];
    }else if (plat==2)
    {
        return [[CTestContext::m_dicConfiguration valueForKey:@"RIGHT_DUT"] intValue];
    }
    return plat;
}


int CIANetFixture::GetDUTTypeFromCamera(int PlatNum,int HandNum,int modelname)
{
    if (HandNum==1) {
        if(g_PrintDetect->IsLeftDUT(PlatNum,""))
        {
            return 1;
        }
        else //if(g_PrintDetect->IsRightDUT(PlatNum,""))
        {
            return 2;
        }
    }else if (HandNum==2)
    {
        if(g_PrintDetect->IsRightDUT(PlatNum,""))
        {
            return 2;
        }
        else //if(g_PrintDetect->IsLeftDUT(PlatNum,""))
        {
            return 1;
        }
    }
    return -1;
    
}

long CIANetFixture::GrabOnce()
{
    int ret = g_PrintDetect->GrabOnce();
    return ret;
    
}
int CIANetFixture::CalculateServoPoint(int platnum,int dutnum,int modelname)
{
    return  g_PrintDetect->GetPlcXY(platnum, dutnum, modelname, PLC_X, PLC_Y, "");
}
float CIANetFixture::CalTestPointX()
{
    NSLog(@"hello world----------------------%0.3f-",PLC_X);
    NSLog(@"hello world-----------------------%0.3f",PLC_Y);
    SaveDebug([NSString stringWithFormat:@"get form camera point x=0.3%f",PLC_X], __func__, __LINE__);
    return [[NSString stringWithFormat:@"%0.3f",PLC_X] floatValue];;
}
float CIANetFixture::CalTestPointY()
{
    SaveDebug([NSString stringWithFormat:@"get form camera point y=0.3%f",PLC_Y], __func__, __LINE__);
    return [[NSString stringWithFormat:@"%0.3f",PLC_Y] floatValue];;
}
//float CIANetFixture::CalTestPointX(int PlatNum,int HandNum,int modelname,const char *m_path)//获取相机校准后的x
//{
//    int a = g_PrintDetect->GetPlcXY(PlatNum, HandNum, modelname, PLC_X, PLC_Y, m_path);
//    if (a<0) {
//        NSLog(@"Camera caltestpointx fail:%d",a);
//        return -999;
//    }
//    else{
//        float x = [[NSString stringWithFormat:@"%0.3f",PLC_X] floatValue];
//        return x;
//    }
//    return -999;
//}
//float CIANetFixture::CalTestPointY(int PlatNum,int HandNum,int modelname,const char *m_path)//获取相机校准后的y
//{
//    int a = g_PrintDetect->GetPlcXY(PlatNum, HandNum, modelname, PLC_X, PLC_Y, m_path);
//    if (a<0) {
//        NSLog(@"Camera caltestpointy fail:%d",a);
//        return -999;
//    }
//    else{
//        float y = [[NSString stringWithFormat:@"%0.3f",PLC_Y] floatValue];
//        return y;
//    }
//    return -999;
//}
void CIANetFixture::ReadPosition()
{
    NSString *path1=TESTPOSITION;
    NSArray *array1=[[NSArray alloc]initWithContentsOfFile:path1];
    if ([array1 count]>0) {
        for(int i=0;i<[array1 count];i++){
            NSDictionary *dic1=[array1 objectAtIndex:i];
            [Position_items_test addObject:dic1];
        }
    }
    NSString *path2=CAMERAPOSITION;
    NSArray *array2=[[NSArray alloc]initWithContentsOfFile:path2];
    if ([array2 count]>0) {
        for(int i=0;i<[array2 count];i++){
            NSDictionary *dic2=[array2 objectAtIndex:i];
            [Position_items_camera addObject:dic2];
        }
    }
    NSString *path3=CXYCAMERAPOSITION;
    NSArray *array3=[[NSArray alloc]initWithContentsOfFile:path3];
    if ([array3 count]>0) {
        for(int i=0;i<[array3 count];i++){
            NSDictionary *dic3=[array3 objectAtIndex:i];
            [Position_items_cxy addObject:dic3];
        }
    }
    NSString *path4=CalCAMERAPOSITION;
    NSArray *array4=[[NSArray alloc]initWithContentsOfFile:path4];
    if ([array4 count]>0) {
        for(int i=0;i<[array4 count];i++){
            NSDictionary *dic4=[array4 objectAtIndex:i];
            [Position_items_cal addObject:dic4];
        }
    }
    
    [array1 release];
    [array2 release];
    [array3 release];
    [array4 release];
}

double CIANetFixture::getTestPoint(int platnum,int dutnum,string key,string xyz)//key为键名，xyz为坐标“XYZ”
{//0-5LL;6-11LR;12-17RL;18-23RR     //获取标准伺服坐标。
    double x = 0.0;
    int index = 0;
    if (platnum == 1 and dutnum == 1) {
        index = 0;
    }
    else if (platnum == 1 and dutnum == 2){
        index = 6;
    }
    else if (platnum == 2 and dutnum == 1){
        index = 12;
    }
    else if (platnum == 2 and dutnum == 2){
        index = 18;
    }
    
    if(key == "X"){
        index = index + 0;
    }
    else if (key == "Y"){
        index = index + 1;
    }
    else if (key == "O"){
        index = index + 2;
    }
    else if (key == "JoyStick"){
        index = index + 3;
    }
    else if (key == "Middle"){
        index = index + 4;
    }
    else if (key == "Index"){
        index = index + 5;
    }
    
    NSDictionary *str = [Position_items_test objectAtIndex:index];
    if (xyz == "X") {
        x = [[str objectForKey:@"XPOS"] doubleValue];
    }
    else if (xyz == "Y"){
        x = [[str objectForKey:@"YPOS"] doubleValue];
    }
    SaveDebug([NSString stringWithFormat:@"halcon coorder point %s=0.3%f",key.c_str(),x], __func__, __LINE__);
    return x;
}
double CIANetFixture::getRotatePoint(int platnum,int dutnum,int key)//key为0-2依次为xyz
{//0-5LL;6-11LR;12-17RL;18-23RR     //获取画圆joystick的段差x，y和画圆高度z
    double x = 0.0;
    int index = 0;
    if (platnum == 1 and dutnum == 1) {
        index = 0;
    }
    else if (platnum == 1 and dutnum == 2) {
        index = 3;
    }
    else if (platnum == 2 and dutnum == 1) {
        index = 6;
    }
    else if (platnum == 2 and dutnum == 2) {
        index = 9;
    }
    key = index + key;
    NSDictionary *str = [Position_items_cxy objectAtIndex:0];
    if (key == 0) {
        x = [[str objectForKey:@"LL_JY_X"] doubleValue];
    }
    else if (key == 1) {
        x = [[str objectForKey:@"LL_JY_Y"] doubleValue];
    }
    else if (key == 2){
        x = [[str objectForKey:@"LL_JY_Z"] doubleValue];
    }
    else if (key == 3){
        x = [[str objectForKey:@"LR_JY_X"] doubleValue];
    }
    else if (key == 4){
        x = [[str objectForKey:@"LR_JY_Y"] doubleValue];
    }
    else if (key == 5){
        x = [[str objectForKey:@"LR_JY_Z"] doubleValue];
    }
    else if (key == 6){
        x = [[str objectForKey:@"RL_JY_X"] doubleValue];
    }
    else if (key == 7){
        x = [[str objectForKey:@"RL_JY_Y"] doubleValue];
    }
    else if (key == 8){
        x = [[str objectForKey:@"RL_JY_Z"] doubleValue];
    }
    else if (key == 9){
        x = [[str objectForKey:@"RR_JY_X"] doubleValue];
    }
    else if (key == 10){
        x = [[str objectForKey:@"RR_JY_Y"] doubleValue];
    }
    else if (key == 11){
        x = [[str objectForKey:@"RR_JY_Z"] doubleValue];
    }
    SaveDebug([NSString stringWithFormat:@"rotate coorder point %d=0.3%f",key,x], __func__, __LINE__);
    return x;
}
double CIANetFixture::getTestPointcal(int platnum,int dutnum,string key,string xyz)
{
    double x = 0.0;
    int index = 0;
    if (platnum == 1 and dutnum == 1) {
        index = 0;
    }
    else if (platnum == 1 and dutnum == 2){
        index = 6;
    }
    else if (platnum == 2 and dutnum == 1){
        index = 12;
    }
    else if (platnum == 2 and dutnum == 2){
        index = 18;
    }
    
    if(key == "X"){
        index = index + 0;
    }
    else if (key == "Y"){
        index = index + 1;
    }
    else if (key == "O"){
        index = index + 2;
    }
    else if (key == "JoyStick"){
        index = index + 3;
    }
    else if (key == "Middle"){
        index = index + 4;
    }
    else if (key == "Index"){
        index = index + 5;
    }
    
    NSDictionary *str = [Position_items_cal objectAtIndex:index];
    if (xyz == "X") {
        x = [[str objectForKey:@"XPOS"] doubleValue];
    }
    else if (xyz == "Y"){
        x = [[str objectForKey:@"YPOS"] doubleValue];
    }
    else if (xyz == "Z"){
        x = [[str objectForKey:@"ZPOS"] doubleValue];
    }
    SaveDebug([NSString stringWithFormat:@"ab coorder point %s=0.3%f",key.c_str(),x], __func__, __LINE__);
    return x;
}

double CIANetFixture::getCameraPoint(int plantnum,int dutnum,string key,string xyz)//key为键名，xyz为坐标“XYZ”
{//0-5LL;6-11LR;12-17RL;18-23RR     //获取相机拍照的坐标
    double x = 0.0;
    int index = 0;
    if (plantnum == 1 and dutnum == 1) {
        index = 0;
    }
    else if (plantnum == 1 and dutnum == 2){
        index = 6;
    }
    else if (plantnum == 2 and dutnum == 1){
        index = 12;
    }
    else if (plantnum == 2 and dutnum == 2){
        index = 18;
    }
    
    if(key == "X"){
        index = index + 0;
    }
    else if (key == "Y"){
        index = index + 1;
    }
    else if (key == "O"){
        index = index + 2;
    }
    else if (key == "JoyStick"){
        index = index + 3;
    }
    else if (key == "Middle"){
        index = index + 4;
    }
    else if (key == "Index"){
        index = index + 5;
    }
    
    NSDictionary *str = [Position_items_camera objectAtIndex:index];
    if (xyz == "X") {
        x = [[str objectForKey:@"XPOS"] doubleValue];
    }
    else if (xyz == "Y"){
        x = [[str objectForKey:@"YPOS"] doubleValue];
    }
    else if (xyz == "Z"){
        x = [[str objectForKey:@"ZPOS"] doubleValue];
    }
    SaveDebug([NSString stringWithFormat:@"take picture coorder point %s=0.3%f",key.c_str(),x], __func__, __LINE__);
    return x;
}

void CIANetFixture::ClearLoadData()//清楚数据
{
    [mm_loadcell ClearTestData];
    [mm_plcdata clearData];
}
//this function only control plc.and waiting plc run done then return;
//添加plc控制命令
void CIANetFixture::AppendCmdPlc(ushort cmd_count, ushort cmd_type, ushort cmd_id, ushort cmd_v1, ushort cmd_v2)
{
    memset(&sendbuffer, 0, sizeof(g_SendBeckHoff));
    sendbuffer.bh_Cmd_cnt = cmd_count;
    sendbuffer.bh_Array_cmd[cmd_count].bh_function_code = cmd_type;
    sendbuffer.bh_Array_cmd[cmd_count].bh_device_id = cmd_id;
    sendbuffer.bh_Array_cmd[cmd_count].bh_op_value1 = cmd_v1;
    sendbuffer.bh_Array_cmd[cmd_count].bh_op_value2 = cmd_v2;
}
long CIANetFixture::SendCmdPLC()
{
    return 0;

}
long CIANetFixture::SendCmdLoadcell(int KeyIndex,char* localCmd,int PlcCmdTimeout)
{
    memset(&sendbuffer, 0, sizeof(g_SendBeckHoff));
    sendbuffer.bh_Array_cmd[0].bh_op_value1 = KeyIndex==0?1:KeyIndex;
    [mm_plcact lock];
    [mm_loadcell ClearTestData];
    long a1 = [mm_plcact SendCmd:sendbuffer];
    if(a1 == 0)
    {
        long a2 = [mm_plcact ReadActRecv:1000];
        if(a2 == 1)
        {
            long a3 = [mm_loadcell SendCmd:localCmd];
            if(!(a3<0))
            {
                long a4 = [mm_plcact ReadActDone:2000];
                if(a4==1)
                {
                    long ret = [mm_loadcell RecvMsg:PlcCmdTimeout];
                    if(ret<0)
                    {
                        [mm_plcact unlock];
                        return ret;
                    }
                    else
                    {
                        [mm_loadcell CellReadAndeDraw:KeyIndex];
                        [mm_plcact unlock];
                        return ret;
                    }
                }
                [mm_plcact unlock];
                return a4;
            }
            [mm_plcact unlock];
            return a3;
        }
        [mm_plcact unlock];
        return a2;
    }
    [mm_plcact unlock];
    return a1;
}

long CIANetFixture::SendCMDJoystick(int downORrotate ,ushort d1,ushort d2)
{
    //downORrotate1=1下压，后台线程存储数据。d1type,d2角度,d3下压半径
    //downORrotate1=2画圆，后台线程存储数据。d1type,d2旋转半径

    [mm_plcdata clearData];
    long ret = [mm_plcact mm_JY_Rotate:downORrotate Value1:d1 Value2:d2];
    //[mm_plcdata performSelectorInBackground:@selector(PLCDataRcv) withObject:nil];
    return ret;
}
int CIANetFixture::ShowJoystick()//画图
{
    int a = [mm_plcdata PLCDataDraw];
    return a;
}
//double CIANetFixture::getPLCMarketData(int Index)//获取校准点数据，未完成
//{
//    double ret = [mm_plcdata ReadData:&readcmd MaxLenght:1];
//    return ret;
//}

double CIANetFixture::getLoadCellMarketData(int Index)//获取校准点数据，未完成
{
    return [mm_loadcell getMarkPoint:Index];
}
int CIANetFixture::ClearTmpDatafile()
{  NSError * err;
    //Clear
    NSDateFormatter * fmt1 = [[NSDateFormatter alloc]init];
    [fmt1 setDateFormat:@"yyyy-MM-dd"];
    NSString * date1 = [fmt1 stringFromDate:[NSDate date]];
    [fmt1 release];
    NSString * file_path = [NSString stringWithFormat:@"%@/%@/tmp",zipfilepath,date1];
    
    if (![[NSFileManager defaultManager] removeItemAtPath:file_path error:&err])
    {
        NSLog(@"ZipBlobFile:%@",[err description]);
    }
    return 0;}
int CIANetFixture::CreatePath(NSString* path)
{
    
    NSFileManager *manager = [NSFileManager defaultManager];
//    NSString *home=[NSString stringWithUTF8String:path];
    
    BOOL isDir, valid = [manager fileExistsAtPath:path isDirectory:&isDir];
    if(valid)
    {
        return 1;
    }
    BOOL r=[[NSFileManager defaultManager] createDirectoryAtPath:path withIntermediateDirectories:YES attributes:nil error:nil];
    if (r) {
        return 0;
    }
    return -1;
}
int CIANetFixture::ZipRawdata(const char* zippathfile)
{
    NSFileManager *fm=[NSFileManager defaultManager];
    NSDateFormatter * fmt1 = [[NSDateFormatter alloc]init];
    [fmt1 setDateFormat:@"yyyy-MM-dd"];
    NSString * date1 = [fmt1 stringFromDate:[NSDate date]];
    [fmt1 release];
    NSString * file_path = [NSString stringWithFormat:@"%@/%@/tmp",zipfilepath,date1];
    NSString * csv_path = [NSString stringWithFormat:@"%@/%@",csvfilepath,date1];
    if (![fm fileExistsAtPath:file_path])
    {
        printf("Raw files is not Exists");
        return -1;
    }
    if(![fm fileExistsAtPath:csv_path])
    {
        printf("Rawcsv files is not Exists");
        return -1;
    }
    else{
        NSArray * arr = [fm contentsOfDirectoryAtPath:file_path error:nil];
        NSString *subpath = nil;
        for (subpath in arr)
        {
            BOOL f =  [fm copyItemAtPath:[NSString stringWithFormat:@"%@/%@",file_path,subpath] toPath:[NSString stringWithFormat:@"%@/%@",csv_path,subpath]  error:nil];
            if(f)
            {
                NSLog(@"copy successful");
            }
            else{
                NSLog(@"copy fail");
            }
        }
        
    }
    NSString * strCMD=@"ditto -ck ";
    
    NSString * blob_file = [NSString stringWithFormat:@"%s", zippathfile];//[blob_dir stringByAppendingPathComponent:;
    //    CreatePath(blob_file);
    NSError * err;
    NSLog(@"bbbbbbbbbbbbb%@",blob_file);
    strCMD = [strCMD stringByAppendingFormat:@"%@ %@",file_path, blob_file];
    system([strCMD UTF8String]);
    //Clear
    if (![[NSFileManager defaultManager] removeItemAtPath:file_path error:&err])
    {
        NSLog(@"ZipBlobFile:%@",[err description]);
    }
    return 0;
}

NSString* PostXml(NSString* url,NSString* action,NSString* body,int timeout)
{
    // 创建SOAP消息，内容格式就是网站上提示的请求报文的主体实体部分
    NSString *soapMsg = [NSString stringWithFormat:
                         @"<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                         "<soap12:Envelope xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
                         "xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" "
                         "xmlns:soap12=\"http://www.w3.org/2003/05/soap-envelope\">"
                         "<soap12:Header>"
                         "<Credentials xmlns=\"MESWebPortal\">"
                         "<key>%@</key>"
                         "</Credentials>"
                         "</soap12:Header>"
                         "<soap12:Body>"
                         "%@"
                         "</soap12:Body>"
                         "</soap12:Envelope>",[CTestContext::m_dicGlobal valueForKey:@"SFIP"] ,body];
    // 创建URL，内容是前面的请求报文报文中第二行主机地址加上第一行URL字段
    NSURL *strurl = [NSURL URLWithString:url];
    // 根据上面的URL创建一个请求
    NSMutableURLRequest *req = [NSMutableURLRequest requestWithURL:strurl];
    req.timeoutInterval = timeout;
    NSString *msgLength = [NSString stringWithFormat:@"%lu", (unsigned long)[soapMsg length]];
    
    // 添加请求的详细信息，与请求报文前半部分的各字段对应
//    [req addValue:@"ws.webxml.com.cn" forHTTPHeaderField:@"Host"];
    [req addValue:[CTestContext::m_dicGlobal valueForKey:@"SFIP"]  forHTTPHeaderField:@"Host"];
    [req addValue:@"application/soap+xml; charset=utf-8" forHTTPHeaderField:@"Content-Type"];
    [req addValue:msgLength forHTTPHeaderField:@"Content-Length"];
    
    // 设置请求行方法为POST，与请求报文第一行对应
    [req setHTTPMethod:@"POST"];
    // 将SOAP消息加到请求中
    [req setHTTPBody: [soapMsg dataUsingEncoding:NSUTF8StringEncoding]];
    NSError * err = [[[NSError alloc ] init] autorelease];
    NSData *conn1 = [[[NSData alloc] init] autorelease];
    conn1 = [NSURLConnection sendSynchronousRequest:req returningResponse:nil error:&err];
    NSString *ret = [[[NSString alloc] initWithData:conn1 encoding:NSUTF8StringEncoding] autorelease];
    if ([ret containsString:[NSString stringWithFormat:@"%@Result",action]]) {
        return ret;
    }
    else{
        return [NSString stringWithFormat:@"%@",err];
    }
}

//void CIANetFixture::xmltest()
//{
//    NSString *url = @"http://ws.webxml.com.cn/WebServices/MobileCodeWS.asmx";
//    NSString *action = @"getMobileCodeInfo";
//    NSString *body = [NSString stringWithFormat:
//                      @"<getMobileCodeInfo xmlns=\"http://WebXml.com.cn/\">"
//                      "<mobileCode>%@</mobileCode>"
//                      "<userID>%@</userID>"
//                      "</getMobileCodeInfo>" ,@"18942914647",@""];
//    NSString * ret = PostXml(url, action, body, 50);
//    NSLog(@"%@",ret);
//}

NSString * putSFC(const char * url,int timout)
{
    NSURL * strURL = [NSURL URLWithString:[NSString stringWithUTF8String:url]];
    if (!strURL)
    {
        NSLog(@"Invalid URL : %@",[NSString stringWithUTF8String:url]);
        return @"Invalid URL";
    }
    
    NSURLRequest* request = [NSURLRequest requestWithURL:strURL cachePolicy:(NSURLRequestCachePolicy)0 timeoutInterval:timout];
    NSURLResponse* response=nil;
    NSError* error=nil;
    NSData* data=[NSURLConnection sendSynchronousRequest:request returningResponse:&response error:&error];
    if (data==nil) {
        return @"empty data";
    }
    NSString * str = [[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding] autorelease];
    SaveDebug([NSString stringWithFormat:@"-----SFC RET=%@;",str],__func__,__LINE__);
    if (str==nil) {
        return @"empty str";
    }
    NSLog(@"SFC Response : %@",str);
    return str;
}

int CIANetFixture::SF_Login()
{
    NSString* ret = @"";
    NSString *username = @"";
    NSString *password = @"";
    NSString *url = [[NSString stringWithFormat:@"http://%@/WebService/Wip_TestSvr.asmx",[CTestContext::m_dicGlobal objectForKey:@"SFIP"]] stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    NSString *action = @"UserLogin";
    
    int platnum = [[CTestContext::m_dicConfiguration valueForKey:@"m_testerpanel"] intValue];
    int dutnum = getTestDutNum(platnum);
    if(dutnum==1)
    {
        username = [CTestContext::m_dicGlobal valueForKey:@"SF_L_Username"];
        password = [CTestContext::m_dicGlobal valueForKey:@"SF_L_Password"];
    }
    else
    {
        username = [CTestContext::m_dicGlobal valueForKey:@"SF_R_Username"];
        password = [CTestContext::m_dicGlobal valueForKey:@"SF_R_Password"];
    }

    NSString *webServiceBodyStr = [NSString stringWithFormat:
                                   @"<%@ xmlns=\"MESWebPortal\">"
                                   "<user>%@</user>"
                                   "<password>%@</password>"
                                   "</%@>" ,action,username,password,action];
    SaveDebug([NSString stringWithFormat:@"-----%@=%@",action,webServiceBodyStr],__func__,__LINE__);
    ret = PostXml(url, action, webServiceBodyStr, 10);
    if (![ret containsString:[NSString stringWithFormat: @"<%@Result>true",action]]) {
        NSAlert * alert = [NSAlert alertWithMessageText:@"check login failed,please use correct username and password" defaultButton:@"Exit" alternateButton:nil otherButton:nil informativeTextWithFormat:@""];
        [alert runModal];
        NSLog(@"===========ret not match========%@",ret);
        return -1;
    }
    else{
        NSLog(@"===========ret match========%@",ret);
        return 0;
    }
}

int CIANetFixture::SF_CheckRoute(string web ,string sn)
{
    NSString* ret = @"";
    NSString *lineD = @"";
    NSString *sectionD = @"";
    NSString *sectionC = @"";
    NSString *stationC = @"";
    NSString *stationD  = @"";
    NSString *url = [[NSString stringWithFormat:@"http://%@/%s",[CTestContext::m_dicGlobal objectForKey:@"SFIP"],web.c_str()] stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    NSString *action = @"CheckRoute";
 
    int platnum = [[CTestContext::m_dicConfiguration valueForKey:@"m_testerpanel"] intValue];
    int dutnum = getTestDutNum(platnum);
//    int dutnum = 1;
    if(dutnum==1)
    {
        lineD = [CTestContext::m_dicGlobal valueForKey:@"l_lineD"];
        sectionC = [CTestContext::m_dicGlobal valueForKey:@"l_sectionC"];
        sectionD = [CTestContext::m_dicGlobal valueForKey:@"l_sectionD"];
        stationC = [CTestContext::m_dicGlobal valueForKey:@"l_stationC"];
        stationD = [CTestContext::m_dicGlobal valueForKey:@"l_stationD"];
    }
    else
    {
        lineD = [CTestContext::m_dicGlobal valueForKey:@"r_lineD"];
        sectionC = [CTestContext::m_dicGlobal valueForKey:@"r_sectionC"];
        sectionD = [CTestContext::m_dicGlobal valueForKey:@"r_sectionD"];
        stationC = [CTestContext::m_dicGlobal valueForKey:@"r_stationC"];
        stationD = [CTestContext::m_dicGlobal valueForKey:@"r_stationD"];
    }
    
    NSString *webServiceBodyStr = [NSString stringWithFormat:
                                   @"<%@ xmlns=\"MESWebPortal\">"
                                   "<sn>%s</sn>"
                                   "<lineD>%@</lineD>"
                                   "<sectionC>%@</sectionC>"
                                   "<sectionD>%@</sectionD>"
                                   "<stationC>%@</stationC>"
                                   "<stationD>%@</stationD>"
                                   "</%@>",action ,sn.c_str(),lineD,sectionC,sectionD,stationC,stationD,action];
    SaveDebug([NSString stringWithFormat:@"-----%@=%@",action,webServiceBodyStr],__func__,__LINE__);
    ret = PostXml(url, action, webServiceBodyStr, 10);
    
    if (![ret containsString:[NSString stringWithFormat: @"<%@Result>OK",action]]) {
        NSAlert * alert = [NSAlert alertWithMessageText:@"check route failed,please check route again" defaultButton:@"Exit" alternateButton:nil otherButton:nil informativeTextWithFormat:@""];
        [alert runModal];
        NSLog(@"===========ret not match========%@",ret);
        return -1;
    }
    else{
          NSLog(@"===========ret match========%@",ret);
        return 0;
    }
}

int CIANetFixture::SF_SaveJson(string value,string key,string name)
{
    if(value == "test_item_content"){
        NSMutableDictionary * temp = [[NSMutableDictionary alloc] initWithDictionary:test_content];
        [dic_JSON setObject:temp forKey:[NSString stringWithFormat:@"%s",key.c_str()]];
        [temp release];
    }
    else if(value =="test_done"){
        NSData *data = [NSJSONSerialization dataWithJSONObject:dic_JSON options:NSJSONWritingPrettyPrinted error:nil];
        NSString *path = [NSString stringWithFormat:@"/vault/Intelli_PM/JsonLog/%s.json",name.c_str()];
        NSFileManager *manager = [NSFileManager defaultManager];
        BOOL isDir, valid = [manager fileExistsAtPath:path isDirectory:&isDir];
        if(valid)
        {
            NSFileHandle *handle = [NSFileHandle fileHandleForWritingAtPath:path];
            [handle seekToEndOfFile];
            [handle writeData:data];
            [handle closeFile];
        }
        else
        {
            BOOL sucess = [manager createFileAtPath:path contents:data attributes:nil];
            if(sucess)
            {
                NSLog(@"Json data save success!/r%@",path);
                return 0;
            }
            else
            {
                NSLog(@"Json data save fail!/r%@",path);
                return -1;
            }
        }
        [dic_JSON removeAllObjects];
    }
    else{
        [test_content setValue:[NSString stringWithFormat:@"%s",value.c_str()] forKey:[NSString stringWithFormat:@"%s",key.c_str()] ];
    }
    return 0;
}

int CIANetFixture::SF_SendJson(string web ,string sn,string date,int result)
{
    NSString* ret = @"";
    NSString *lineD = @"";
    NSString *sectionD = @"";
    NSString *sectionC = @"";
    NSString *stationC = @"";
    NSString *stationD  = @"";
    NSString *res = @"";
    NSString *url = [NSString stringWithFormat:@"http://%@/%s",[CTestContext::m_dicGlobal objectForKey:@"SFIP"],web.c_str()];
    //http://10.10.32.50:7077/WebService/Wip_TestSvr.asmx
    NSString *action = @"CommitTestDataOneFile";

    int platnum = [[CTestContext::m_dicConfiguration valueForKey:@"m_testerpanel"] intValue];
    int dutnum = getTestDutNum(platnum);
//    int dutnum = 1;
    if(dutnum==1)
    {
        lineD = [CTestContext::m_dicGlobal valueForKey:@"l_lineD"];
        sectionC = [CTestContext::m_dicGlobal valueForKey:@"l_sectionC"];
        sectionD = [CTestContext::m_dicGlobal valueForKey:@"l_sectionD"];
        stationC = [CTestContext::m_dicGlobal valueForKey:@"l_stationC"];
        stationD = [CTestContext::m_dicGlobal valueForKey:@"l_stationD"];
    }
    else
    {
        lineD = [CTestContext::m_dicGlobal valueForKey:@"r_lineD"];
        sectionC = [CTestContext::m_dicGlobal valueForKey:@"r_sectionC"];
        sectionD = [CTestContext::m_dicGlobal valueForKey:@"r_sectionD"];
        stationC = [CTestContext::m_dicGlobal valueForKey:@"r_stationC"];
        stationD = [CTestContext::m_dicGlobal valueForKey:@"r_stationD"];
    }
    if(result == 1)
    {
        res = @"PASS";
    }
    else
    {
        res = @"Fail";
    }
    NSString *path = [NSString stringWithFormat:@"/vault/Intelli_PM/JsonLog/%s%s.json",sn.c_str(),date.c_str()];
    NSString *filepath = [NSString stringWithFormat:@"[%s][%s][%@][%@][%@].json",sn.c_str(),date.c_str(),lineD,sectionC,res];
    NSString *json = [NSString stringWithContentsOfFile:path encoding:NSUTF8StringEncoding error:nil];
    //    NSString *testData = [json stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    //    NSString *filebytes = [@"null" stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
//    int totaltime = [[CTestContext::m_dicGlobal valueForKey:kContextTotalTime] intValue];
    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
    [dateFormatter setDateFormat:@"yyyy-MM-dd HH:mm:ss"];
    NSString * totaltime = [dateFormatter stringFromDate:[NSDate date]];
    [dateFormatter release];
    NSString *webServiceBodyStr = [NSString stringWithFormat:
                                   @"<%@ xmlns=\"MESWebPortal\">"
                                   "<sn>%s</sn>"
                                   "<stationCode>%@</stationCode>"
                                   "<stationDesc>%@</stationDesc>"
                                   "<sectionCode>%@</sectionCode>"
                                   "<sectionDesc>%@</sectionDesc>"
                                   "<lineCode>%@</lineCode>"
                                   "<lineName>%@</lineName>"
                                   "<tester>CL01</tester>"
                                   "<test_time>%@</test_time>"
                                   "<testResult>%d</testResult>"
                                   "<ResultType>%d</ResultType>"
                                   "<errorCode>null</errorCode>"
                                   "<errorDesc>null</errorDesc>"
                                   "<datastr><![CDATA[%@]]></datastr>"
                                   "<testFileName>"
                                   "<string>%@</string>"
                                   "</testFileName>"
                                   "</%@>",action,sn.c_str(),stationC,stationD,sectionC,sectionD,lineD,lineD,totaltime,result,result,json,filepath,action];
    SaveDebug([NSString stringWithFormat:@"-----%@=%@",action,webServiceBodyStr],__func__,__LINE__);
    ret = PostXml(url, action, webServiceBodyStr, 10);
    if (![ret containsString:[NSString stringWithFormat: @"<%@Result>OK",action]]) {
        NSAlert * alert = [NSAlert alertWithMessageText:@"CommitTestData failed,please commit again" defaultButton:@"Exit" alternateButton:nil otherButton:nil informativeTextWithFormat:@""];
        [alert runModal];
        NSLog(@"===========ret not match========%@",ret);

        return -1;
    }
    else{
        NSLog(@"===========ret match========%@",ret);

        return 0;
    }
}

