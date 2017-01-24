

#import <Cocoa/Cocoa.h>
#import "IABezierPath.h"
#import "CommandStruct.h"

#define CHART_OFFset 5.0f;
#define MARKPINTSIZE 6.0f;
#define SENDBUFFER_SIZE 255
#define REVBUFFER_SIZE 255
#define DATABUFFER_SIZE 1024*10*sizeof(float)

//typedef struct LOADCELLDATA
//{
//    unsigned int dataleng;
//    float markdata[15];
//    float rawdata[DATABUFFER_SIZE];
//}g_LOADCELLDATA;

@interface IAChartView : NSImageView
{
    char m_LineNumber[255];
    char m_StationNumber[255];
    char m_SerialNumber[255];
    char m_Platfrom[255];
    char m_KeyName[255];
    
    NSArray *  arrMarkPointname;
    NSArray *  arrMarkPoint;
    NSArray *  arrCurvePoints;
    NSString * viewtitle;
    g_LOADCELLDATA loadcelldata;
}

//@property (retain) NSArray *arrMarkPoint;
//@property (retain) NSArray *arrCurvePoints;
@property (retain) NSString *viewtitle;

-(int)LoadDataFromCSV:(const char*)filename;
-(void)setCurve:(g_LOADCELLDATA)data;
-(void)setCurveData:(NSData*)data;
//@property (retain) NSColor *backgroundColor;
-(BOOL)writePNGToURL:(NSString*)strpath;
-(void)setLineNumber:(const char*)buf;
-(void)setStationNumber:(const char*)buf;
-(void)setSerialNumber:(const char*)buf;
-(void)setPlatfrom:(const char*)buf;
-(void)setKeyName:(const char*)buf;

@end

