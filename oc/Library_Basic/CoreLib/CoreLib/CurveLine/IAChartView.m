

#import "IAChartView.h"
#import "IAFeelingData.h"
#define  MAX_FORCE 180.0
#define  MAX_TRAVE  1.0

float MaxTrave=MAX_TRAVE;
float MaxForce=MAX_FORCE;


@implementation IAChartView

@synthesize viewtitle;





const NSString *MarkdataNames[] =  {@"force at 0.2mm",@"Slope before Peak",@"Peak force",@"Peak travel",@"Bottom force",@"Bottom travel",@"Drop Force",@"Slope peak botton",@"Drop Stroke",@"Travel at 80g",@"Travel at 160g",@"Area under curve",@"Ratio",@"Close point travel",@"Close point force"};


-(void)setLineNumber:(const char*)buf
{
    strcpy(m_LineNumber, buf);
}
-(void)setStationNumber:(const char*)buf
{
    strcpy(m_SerialNumber, buf);
}
-(void)setSerialNumber:(const char*)buf
{
    strcpy(m_SerialNumber, buf);
}
-(void)setPlatfrom:(const char*)buf
{
    strcpy(m_Platfrom, buf);
}
-(void)setKeyName:(const char*)buf
{
    strcpy(m_KeyName, buf);
}

- (instancetype)init
{
    self = [super init];
    if (self) {
        memset(&loadcelldata,0,sizeof(loadcelldata));
    }
    return self;
}
-(id)initWithFrame:(NSRect)frameRect
{
    self = [super initWithFrame:frameRect];
    if (self) {
        memset(&loadcelldata,0,sizeof(loadcelldata));
    }
    return self;
}
- (void)mouseDown:(NSEvent *)theEvent
{
    
}
-(void)addMarketDataview:(NSTableView*)view
{
    [self addSubview:view];
}
-(void)setCurve:(g_LOADCELLDATA)data
{
    MaxTrave=MAX_TRAVE;
    memcpy(&loadcelldata,&data,sizeof(g_LOADCELLDATA));
    
    unsigned int fLen=(loadcelldata.dataleng/4-15)/2;
    for (int i=fLen-1; i>=0; i--) {
        if (loadcelldata.rawdata[i+fLen]>MaxTrave) {
            MaxTrave=loadcelldata.rawdata[i+fLen];
        }
    }
}
-(void)setCurveData:(NSData*)data
{
    MaxTrave=MAX_TRAVE;
    MaxForce=MAX_FORCE;
    memcpy(&loadcelldata,[data bytes],[data length]);
    unsigned int fLen=(loadcelldata.dataleng/sizeof(float)-15)/2;
    for (int i=fLen-1; i>=0; i--) {
        if (loadcelldata.rawdata[i+fLen]>MaxTrave) {
            MaxTrave=loadcelldata.rawdata[i+fLen];
        }
        if (loadcelldata.rawdata[i]>MaxForce) {
            MaxForce=loadcelldata.rawdata[i];
        }
    }
 
}
-(int)LoadDataFromCSV:(const char*)filename
{
    MaxTrave=MAX_TRAVE;
    MaxForce=MAX_FORCE;
    int ret=[IAFeelingData ReadLocalDatatoFile:&loadcelldata LineNumber:m_LineNumber StationNumber:m_StationNumber SerialNumber:m_SerialNumber Platfrom:m_Platfrom KeyName:m_KeyName Filename:filename];
    if (ret<0) {
        NSAlert *alert=[NSAlert alertWithMessageText:@"Load file fail" defaultButton:@"OK" alternateButton:nil otherButton:nil informativeTextWithFormat:@""];
        [alert runModal];
        return ret;
    }
    unsigned int fLen=(loadcelldata.dataleng/4-15)/2;
    for (int i=fLen-1; i>=0; i--) {
        if (loadcelldata.rawdata[i+fLen]>MaxTrave) {
            MaxTrave=loadcelldata.rawdata[i+fLen];
        }
        if (loadcelldata.rawdata[i]>MaxForce) {
            MaxForce=loadcelldata.rawdata[i];
        }
    }
    return 0;
}
- (BOOL)writePNGToURL:(NSString*)strpath
{
    NSRect bounds = self.bounds;
    NSSize outputSizePx=bounds.size;
    NSBitmapImageRep* rep = [[NSBitmapImageRep alloc]
                             initWithBitmapDataPlanes:nil
                             pixelsWide:outputSizePx.width
                             pixelsHigh:outputSizePx.height
                             bitsPerSample:8
                             samplesPerPixel:4
                             hasAlpha:YES isPlanar:NO
                             colorSpaceName:NSDeviceRGBColorSpace
                             bytesPerRow:outputSizePx.width*4
                             bitsPerPixel:32];
    
    [self cacheDisplayInRect:bounds toBitmapImageRep:rep];
    NSData* kData = [rep representationUsingType:NSPNGFileType properties:nil];
    [kData writeToFile:strpath atomically:NO];
    [rep release];
    return YES;
}

- (void)drawRect:(NSRect)dirtyRect {
//    NSLog(@"----------------9999------------------------------------------------");
    CGRect rect;
    NSRect bounds = self.bounds;
    [[NSColor colorWithCalibratedRed:0.7098 green:0.7294 blue:0.7294 alpha:1] setFill];
//    [[NSColor windowBackgroundColor] set];
    [NSBezierPath fillRect: bounds];
    
    CGContextRef context = (CGContextRef)[[NSGraphicsContext currentContext] graphicsPort];
    CGContextBeginPage(context, &rect);
    [self drawRuler:context];
//    CGContextEndPage(context);
    CGContextFlush(context);
    
    [self drawTitle];
    NSBezierPath *linepath=[NSBezierPath bezierPath];
    [self DarwCurveLine:linepath];
    [linepath stroke];
    [linepath closePath];
    [self DarwMarkPoint:context];
//    [self drawDataLine:context];
    CGContextEndPage(context);
    CGContextFlush(context);

}
-(void)DarwCurveLine:(NSBezierPath*)linepath
{
    if (loadcelldata.dataleng<4*10) {
//        NSLog(@"----------------2222-------------------------------------------------");
        return;
    }
    float maxd=MaxTrave;
    float maxf=MaxForce;
    unsigned int min_x=CHART_OFFset;
    unsigned int min_y=CHART_OFFset;
    
    unsigned int max_x=[self bounds].size.width-min_x;
    
    unsigned int max_y=[self bounds].size.height-min_y;
    float xstep=max_x*3/4/maxd;
    float ystep=max_y/maxf;
    unsigned int fLen=(loadcelldata.dataleng/4-15)/2;
    if (fLen<4) {
//        NSLog(@"----------------1111-------------------------------------------------");
        return;
    }
    NSPoint point;
    [[NSColor redColor] set];
    [linepath moveToPoint:NSMakePoint(min_x, min_y)];
    for (int i=0; i<fLen-2; i++) {
        
        point.x=loadcelldata.rawdata[i+fLen]*xstep+CHART_OFFset;
        point.y=loadcelldata.rawdata[i]*ystep+CHART_OFFset;
        [linepath lineToPoint:point];
    }
    [linepath stroke];
}
-(void)drawDataLine:(CGContextRef)gc
{
    //2 Slope before the peak
    NSPoint point;
    float maxd=MaxTrave;
    float maxf=MaxForce;
    unsigned int min_x=CHART_OFFset;
    unsigned int min_y=CHART_OFFset;
    
    unsigned int max_x=[self bounds].size.width-min_x;
    
    unsigned int max_y=[self bounds].size.height-min_y;
    
    
    float xstep=max_x*3/4/maxd;
    float ystep=max_y/maxf;
    
    CGPoint p=CGPointMake(0,0);
    CGContextSetLineWidth(gc,1.5);
    [[NSColor grayColor] set];
    CGContextMoveToPoint(gc, p.x + 5, p.y + 5);
    point.x=loadcelldata.markdata[3]*xstep+CHART_OFFset;
    point.y=loadcelldata.markdata[2]*ystep+CHART_OFFset;
    CGContextAddLineToPoint(gc, point.x,point.y);
    CGContextStrokePath(gc);
    
}
-(BOOL)GetPointNameMaxSize:(NSSize*)pSize Attributes:(NSMutableDictionary*)attributes
{
    pSize->height=0;
    pSize->width=0;
    NSRect dirtyRect = self.bounds;
    for (int i=0; i<15; i++)
    {
    
    NSRect retSize = [MarkdataNames[i] boundingRectWithSize:NSMakeSize(dirtyRect.size.width, dirtyRect.size.height)
                                          options:NSStringDrawingUsesFontLeading|NSStringDrawingUsesLineFragmentOrigin
                                       attributes:attributes];
        if (retSize.size.width>pSize->width)
        {
            pSize->width=retSize.size.width;
        }
        if (retSize.size.height>pSize->height)
        {
            pSize->height=retSize.size.height;
        }
    }
    return YES;
}
-(BOOL)GetPointValueMaxSize:(NSRect*)pSize Attributes:(NSMutableDictionary*)attributes
{
    pSize->size.height=0;
    pSize->size.width=0;
    NSRect dirtyRect = self.bounds;
    for (int i=0; i<15; i++)
    {
        NSString *Val=[NSString stringWithFormat:@"%.3f", loadcelldata.markdata[i]];
        
        NSRect retSize = [Val boundingRectWithSize:NSMakeSize(dirtyRect.size.width, dirtyRect.size.height)
                                                        options:NSStringDrawingUsesFontLeading|NSStringDrawingUsesLineFragmentOrigin
                                                     attributes:attributes];
        if (retSize.size.width>pSize->size.width)
        {
            pSize->size.width=retSize.size.width;
        }
        if (retSize.size.height>pSize->size.height)
        {
            pSize->size.height=retSize.size.height;
        }
    }
    pSize->origin.x=dirtyRect.size.width-pSize->size.width-Drawmarkdata_offset_x;
    pSize->origin.y=dirtyRect.size.height-pSize->size.height;
    return YES;
}
float Drawmarkdata_offset_Y=20.0;
float Drawmarkdata_offset_x=6.0;

-(void)DarwMarkDatatable:(CGContextRef)gc Height:(float)height
          NameWidth:(float)namewidth
         ValueWidth:(float)valwidth
         IndexWidth:(float)Indexwidth
{
    NSRect ValRect=NSMakeRect(0, 0, valwidth, height);
    for (int i=0; i<15; i++) {
        ValRect.origin.y=self.bounds.size.height-height*(1.0+i)-Drawmarkdata_offset_Y;
        ValRect.origin.x=self.bounds.size.width-valwidth-Drawmarkdata_offset_x;
        ValRect.size.height=height;
        ValRect.size.width=valwidth;
        CGContextAddRect(gc, ValRect);
        ValRect.origin.x=self.bounds.size.width-namewidth-valwidth-Drawmarkdata_offset_x;
        ValRect.size.width=namewidth;
        CGContextAddRect(gc, ValRect);
        
        ValRect.origin.x=self.bounds.size.width-Indexwidth-namewidth-valwidth-Drawmarkdata_offset_x;
        ValRect.size.width=Indexwidth;
        CGContextAddRect(gc, ValRect);
    }
    CGContextStrokePath(gc);
}
-(void)DarwMarkData:(CGContextRef)gc Index:(int)index Height:(float)height
          NameWidth:(float)namewidth
         ValueWidth:(float)valwidth
         IndexWidth:(float)Indexwidth
         Attributes:(NSMutableDictionary*)attributes
{
    NSRect ValRect;
    NSMutableParagraphStyle *style = [[NSParagraphStyle defaultParagraphStyle] mutableCopy];
    [style setAlignment:NSRightTextAlignment];
    [attributes setObject: style forKey:NSParagraphStyleAttributeName];
    
    ValRect.origin.y=self.bounds.size.height-height*(1.0+index)-Drawmarkdata_offset_Y;
    
    
    NSString *Val=[NSString stringWithFormat:@"%.3f", loadcelldata.markdata[index]];
    
    ValRect.origin.x=self.bounds.size.width-valwidth-Drawmarkdata_offset_x;
    ValRect.size.width=valwidth;
    ValRect.size.height=height;
    [Val drawInRect:ValRect withAttributes:attributes];
    
    [style setAlignment:NSLeftTextAlignment];
    [attributes setObject: style forKey:NSParagraphStyleAttributeName];
    
    
    ValRect.size.width=namewidth;
    ValRect.origin.x=self.bounds.size.width-namewidth-valwidth-Drawmarkdata_offset_x+5;
    [MarkdataNames[index] drawInRect:ValRect withAttributes:attributes];


    NSString* Module=[NSString stringWithFormat:@"%d",index+1];
    
    [style setAlignment:NSLeftTextAlignment];
    [attributes setObject: style forKey:NSParagraphStyleAttributeName];
    ValRect.origin.x=self.bounds.size.width-namewidth-valwidth-Indexwidth-Drawmarkdata_offset_x+5;
    ValRect.size.width=Indexwidth;
    [Module drawInRect:ValRect withAttributes:attributes];
    [style release];
    
}

-(void)DarwMarkLable:(CGContextRef)gc Lable:(NSString*)lable Point:(NSPoint)point LableRect:(NSRect)rect Posetion:(int)posetion Attributes:(NSMutableDictionary*)attributes
{
    CGContextSetLineWidth(gc,1.5);
    CGContextMoveToPoint(gc, point.x, point.y);
    
    switch (posetion) {
        case 0:
            point.x=rect.origin.x+rect.size.width;
            point.y=rect.origin.y+rect.size.height/2;
            break;
        case 1:
            point.x=rect.origin.x;
            point.y=rect.origin.y+rect.size.height/2;
            break;
        case 2:
            point.x=rect.origin.x+rect.size.width/2;
            point.y=rect.origin.y;
            break;
        case 3:
            point.x=rect.origin.x+rect.size.width/2;
            point.y=rect.origin.y+rect.size.height;
            break;
        default:
            break;
    }
    CGContextAddLineToPoint(gc, point.x,point.y);
    
    CGContextAddEllipseInRect(gc,rect);
    CGContextStrokePath(gc);
    NSMutableParagraphStyle *style = [[NSParagraphStyle defaultParagraphStyle] mutableCopy];
    [style setAlignment:NSCenterTextAlignment];
    [attributes setObject: style forKey:NSParagraphStyleAttributeName];
    [lable drawInRect:rect withAttributes:attributes];
    [style release];
}
-(void)DarwMarkPoint:(CGContextRef)gc
{
    if (loadcelldata.dataleng==0) {
//        NSLog(@"----------------00000-------------------------------------------------");
        return;
    }
    NSPoint centerPoint;
    float maxd=MaxTrave;
    float maxf=MaxForce;
    float chart_offset=CHART_OFFset;
    unsigned int min_x=chart_offset;
    unsigned int min_y=chart_offset;
    
    unsigned int max_x=[self bounds].size.width-min_x;
    
    unsigned int max_y=[self bounds].size.height-min_y;
    
    
    float xstep=max_x*3/4/maxd;
    float ystep=max_y/maxf;
    
    
    
    
    NSMutableDictionary *attributes = [NSMutableDictionary dictionary];
    NSFont *ModuleFont = [NSFont userFontOfSize: 14.0];
    
    NSFontManager *fm = [NSFontManager sharedFontManager];
    NSFont *boldFont = [fm convertFont: ModuleFont
                           toHaveTrait: NSBoldFontMask];
    [attributes setObject: boldFont
                   forKey: NSFontAttributeName];
    [attributes setObject:[NSColor brownColor] forKey:NSForegroundColorAttributeName];
    
    NSPoint point;
    [[NSColor darkGrayColor] set];
    [self DarwMarkDatatable:gc Height:20 NameWidth:172 ValueWidth:50 IndexWidth:20];
    //
    if (loadcelldata.markdata[0]>0) {
        point.x=(0.2*xstep)+chart_offset;
        point.y=loadcelldata.markdata[0]*ystep+chart_offset;
        [[NSColor brownColor] set];
        [self DrawPoint:gc CenterPoint:point];
        [self DarwMarkLable:gc Lable:@"1" Point:point LableRect:NSMakeRect(point.x-80, point.y-10, 20, 20) Posetion:0 Attributes:attributes];
        

        
    }
    [self DarwMarkData:gc Index:0 Height:20 NameWidth:172 ValueWidth:50 IndexWidth:22 Attributes:attributes];
    

    

    
    [attributes setObject:[NSColor orangeColor] forKey:NSForegroundColorAttributeName];
    [self DarwMarkData:gc Index:1 Height:20 NameWidth:172 ValueWidth:50 IndexWidth:22 Attributes:attributes];
    
    
    
    
    if (loadcelldata.markdata[3]>0 && loadcelldata.markdata[2]>0) {
        point.y=loadcelldata.markdata[2]*ystep+chart_offset;
        point.x=loadcelldata.markdata[3]*xstep+chart_offset;
        
        [[NSColor orangeColor] set];
        CGPoint p=CGPointMake(chart_offset,chart_offset);
        CGContextSetLineWidth(gc,1.5);
        CGContextMoveToPoint(gc, p.x,p.y);
        CGContextAddLineToPoint(gc, point.x,point.y);
        CGContextStrokePath(gc);
        centerPoint=NSMakePoint((point.x-chart_offset)/2+chart_offset, (point.y-chart_offset)/2+chart_offset);
        [attributes setObject:[NSColor orangeColor] forKey:NSForegroundColorAttributeName];
        [self DarwMarkLable:gc Lable:@"2" Point:centerPoint LableRect:NSMakeRect(centerPoint.x+10, centerPoint.y-10, 20, 20) Posetion:1 Attributes:attributes];
        
        [[NSColor purpleColor] set];
        [self DrawPoint:gc CenterPoint:point];
        [attributes setObject:[NSColor purpleColor] forKey:NSForegroundColorAttributeName];
        [self DarwMarkLable:gc Lable:@"3" Point:point LableRect:NSMakeRect(point.x-50, point.y-10, 20, 20) Posetion:0 Attributes:attributes];
        [self DarwMarkLable:gc Lable:@"4" Point:point LableRect:NSMakeRect(point.x-10, point.y+50, 20, 20) Posetion:2 Attributes:attributes];
        
    }
    [attributes setObject:[NSColor purpleColor] forKey:NSForegroundColorAttributeName];
    [self DarwMarkData:gc Index:2 Height:20 NameWidth:172 ValueWidth:50 IndexWidth:22 Attributes:attributes];
    [self DarwMarkData:gc Index:3 Height:20 NameWidth:172 ValueWidth:50 IndexWidth:22 Attributes:attributes];
    
    
    if (loadcelldata.markdata[4]>0 && loadcelldata.markdata[5]>0) {
        [[NSColor blueColor] set];
        point.x=loadcelldata.markdata[5]*xstep+CHART_OFFset;
        point.y=loadcelldata.markdata[4]*ystep+CHART_OFFset;
        [self DrawPoint:gc CenterPoint:point];
        
        [attributes setObject:[NSColor blueColor] forKey:NSForegroundColorAttributeName];
        [self DarwMarkLable:gc Lable:@"5" Point:point LableRect:NSMakeRect(point.x+50, point.y-10, 20, 20) Posetion:1 Attributes:attributes];
        [self DarwMarkLable:gc Lable:@"6" Point:point LableRect:NSMakeRect(point.x-10, point.y+40, 20, 20) Posetion:2 Attributes:attributes];
        
        [[NSColor colorWithCalibratedRed:1 green:0.5 blue:0.5 alpha:1]set];
        point.y=loadcelldata.markdata[2]*ystep+chart_offset;
        point.x=loadcelldata.markdata[3]*xstep+chart_offset;
        centerPoint.x=point.x;
        centerPoint.y=(point.y-loadcelldata.markdata[4]*ystep+chart_offset)/2+loadcelldata.markdata[4]*ystep+chart_offset;
        
        CGContextMoveToPoint(gc, point.x,point.y);
        point.y=loadcelldata.markdata[4]*ystep+CHART_OFFset;
        CGContextAddLineToPoint(gc, point.x,point.y);
        CGContextStrokePath(gc);
        point.x=loadcelldata.markdata[5]*xstep+CHART_OFFset;
        [[NSColor darkGrayColor]set];
        CGContextMoveToPoint(gc, point.x,point.y);
        point.x=loadcelldata.markdata[3]*xstep+chart_offset-10;
        CGContextAddLineToPoint(gc, point.x,point.y);
        CGContextStrokePath(gc);
        
        [[NSColor colorWithCalibratedRed:1 green:0.5 blue:0.5 alpha:1] set];
        [attributes setObject:[NSColor colorWithCalibratedRed:1 green:0.5 blue:0.5 alpha:1] forKey:NSForegroundColorAttributeName];
        [self DarwMarkLable:gc Lable:@"7" Point:centerPoint LableRect:NSMakeRect(centerPoint.x+10, centerPoint.y-10, 20, 20) Posetion:1 Attributes:attributes];
        
        [[NSColor colorWithCalibratedRed:0.5 green:1 blue:0.5 alpha:1] set];
        point.y=loadcelldata.markdata[2]*ystep+chart_offset;
        point.x=loadcelldata.markdata[3]*xstep+chart_offset;
        NSPoint point3=point;
        CGContextMoveToPoint(gc, point.x,point.y);
        point.x=loadcelldata.markdata[5]*xstep+CHART_OFFset;
        point.y=loadcelldata.markdata[4]*ystep+CHART_OFFset;
        NSPoint point5=point;
        CGContextAddLineToPoint(gc, point.x,point.y);
        CGContextStrokePath(gc);
        
        centerPoint.y=(point3.y-point5.y)/2+point5.y;
        centerPoint.x=(point5.x-point3.x)/2+point3.x;
        [attributes setObject:[NSColor colorWithCalibratedRed:0.5 green:1 blue:0.5 alpha:1] forKey:NSForegroundColorAttributeName];
        
        [self DarwMarkLable:gc Lable:@"8" Point:centerPoint LableRect:NSMakeRect(centerPoint.x-10, centerPoint.y+30, 20, 20) Posetion:2 Attributes:attributes];
        
    }
    
    [attributes setObject:[NSColor blueColor] forKey:NSForegroundColorAttributeName];
    [self DarwMarkData:gc Index:4 Height:20 NameWidth:172 ValueWidth:50 IndexWidth:22 Attributes:attributes];
    [self DarwMarkData:gc Index:5 Height:20 NameWidth:172 ValueWidth:50 IndexWidth:22 Attributes:attributes];
    


    
    [attributes setObject:[NSColor colorWithCalibratedRed:1 green:0.5 blue:0.5 alpha:1] forKey:NSForegroundColorAttributeName];
    [self DarwMarkData:gc Index:6 Height:20 NameWidth:172 ValueWidth:50 IndexWidth:22 Attributes:attributes];
    
    
    
    [attributes setObject:[NSColor colorWithCalibratedRed:0.5 green:1 blue:0.5 alpha:1] forKey:NSForegroundColorAttributeName];
    [self DarwMarkData:gc Index:7 Height:20 NameWidth:172 ValueWidth:50 IndexWidth:22 Attributes:attributes];
    
    if ((loadcelldata.markdata[9]-loadcelldata.markdata[3])>0) {
        [[NSColor colorWithCalibratedRed:0.5 green:0.5 blue:1 alpha:1] set];
        if (loadcelldata.markdata[2]>80) {
            point.y=loadcelldata.markdata[2]*ystep+chart_offset;
        }else
        {
            point.y=80*ystep+chart_offset;
        }
        
        point.x=loadcelldata.markdata[3]*xstep+chart_offset;
        CGContextMoveToPoint(gc, point.x,point.y);
        point.x=loadcelldata.markdata[9]*xstep+chart_offset;
        CGContextAddLineToPoint(gc, point.x,point.y);
        CGContextStrokePath(gc);
        centerPoint.x=((loadcelldata.markdata[9]*xstep+chart_offset)-(loadcelldata.markdata[3]*xstep+chart_offset))/2+loadcelldata.markdata[3]*xstep+chart_offset;
        centerPoint.y=point.y;
        
        [attributes setObject:[NSColor colorWithCalibratedRed:0.5 green:0.5 blue:1 alpha:1] forKey:NSForegroundColorAttributeName];
        [self DarwMarkLable:gc Lable:@"9" Point:centerPoint LableRect:NSMakeRect(centerPoint.x-10, centerPoint.y+30, 20, 20) Posetion:2 Attributes:attributes];
    }
    
    [attributes setObject:[NSColor colorWithCalibratedRed:0.5 green:0.5 blue:1 alpha:1] forKey:NSForegroundColorAttributeName];
    [self DarwMarkData:gc Index:8 Height:20 NameWidth:172 ValueWidth:50 IndexWidth:22 Attributes:attributes];

    if (loadcelldata.markdata[9]>0 ) {
        [[NSColor cyanColor] set];
        point.x=loadcelldata.markdata[9]*xstep+CHART_OFFset;
        point.y=80.0*ystep+CHART_OFFset;
        [self DrawPoint:gc CenterPoint:point];
        float y3=loadcelldata.markdata[2]*ystep+chart_offset;
        y3=y3>point.y?y3:point.y;
        [attributes setObject:[NSColor cyanColor] forKey:NSForegroundColorAttributeName];
        [self DarwMarkLable:gc Lable:@"10" Point:point LableRect:NSMakeRect(point.x-10, y3+30, 20, 20) Posetion:2 Attributes:attributes];

    }
    [attributes setObject:[NSColor cyanColor] forKey:NSForegroundColorAttributeName];
    [self DarwMarkData:gc Index:9 Height:20 NameWidth:172 ValueWidth:50 IndexWidth:22 Attributes:attributes];
    
    if (loadcelldata.markdata[10]>0) {
        [[NSColor magentaColor] set];
        point.x=loadcelldata.markdata[10]*xstep+CHART_OFFset;
        point.y=160.0*ystep+CHART_OFFset;
        [self DrawPoint:gc CenterPoint:point];
        [self DrawPoint:gc CenterPoint:point];
        [attributes setObject:[NSColor magentaColor] forKey:NSForegroundColorAttributeName];
        [self DarwMarkLable:gc Lable:@"11" Point:point LableRect:NSMakeRect(point.x-10, point.y+20, 20, 20) Posetion:2 Attributes:attributes];
    }
    [attributes setObject:[NSColor magentaColor] forKey:NSForegroundColorAttributeName];
    [self DarwMarkData:gc Index:10 Height:20 NameWidth:172 ValueWidth:50 IndexWidth:22 Attributes:attributes];
    
    
    
    [attributes setObject:[NSColor colorWithCalibratedRed:0.25 green:0.75 blue:1 alpha:1] forKey:NSForegroundColorAttributeName];
    [self DarwMarkData:gc Index:11 Height:20 NameWidth:172 ValueWidth:50 IndexWidth:22 Attributes:attributes];
    
    [attributes setObject:[NSColor colorWithCalibratedRed:0.7 green:0.25 blue:0.25 alpha:1] forKey:NSForegroundColorAttributeName];
    [self DarwMarkData:gc Index:12 Height:20 NameWidth:172 ValueWidth:50 IndexWidth:22 Attributes:attributes];
    
    if (loadcelldata.markdata[13]>0 && loadcelldata.markdata[14]>0) {
        [[NSColor yellowColor] set];
        point.x=loadcelldata.markdata[13]*xstep+CHART_OFFset;
        point.y=loadcelldata.markdata[14]*ystep+CHART_OFFset;
        [self DrawPoint:gc CenterPoint:point];
        
        [[NSColor yellowColor] set];
        [self DrawPoint:gc CenterPoint:point];
        [attributes setObject:[NSColor yellowColor] forKey:NSForegroundColorAttributeName];
        [self DarwMarkLable:gc Lable:@"15" Point:point LableRect:NSMakeRect(point.x-50, point.y-10, 20, 20) Posetion:0 Attributes:attributes];
        [self DarwMarkLable:gc Lable:@"14" Point:point LableRect:NSMakeRect(point.x-10, point.y-50, 20, 20) Posetion:3 Attributes:attributes];

        
    }
    [attributes setObject:[NSColor yellowColor] forKey:NSForegroundColorAttributeName];
    [self DarwMarkData:gc Index:13 Height:20 NameWidth:172 ValueWidth:50 IndexWidth:22 Attributes:attributes];
    [self DarwMarkData:gc Index:14 Height:20 NameWidth:172 ValueWidth:50 IndexWidth:22 Attributes:attributes];
  
}
-(void)DrawPoint:(CGContextRef)gc CenterPoint:(NSPoint) point
{
    CGContextAddEllipseInRect(gc,CGRectMake(point.x-6/2,point.y-6/2,6,6));
    CGContextFillPath(gc);
    CGContextStrokePath(gc);
}
-(void)DrawDRulerText:(NSString*)text Rect:(NSPoint)point
{
    NSMutableDictionary *attributes = [NSMutableDictionary dictionary];
    NSFont *currentFont = [NSFont userFontOfSize: 8.0];
    if (bold) {
        NSFontManager *fm = [NSFontManager sharedFontManager];
        NSFont *boldFont = [fm convertFont: currentFont
                               toHaveTrait: NSItalicFontMask];
        [attributes setObject: boldFont
                       forKey: NSFontAttributeName];
        [attributes setObject: [NSColor blackColor]
                       forKey: NSForegroundColorAttributeName];
        
        
    } else {
        
        [attributes setObject: currentFont
                       forKey: NSFontAttributeName];
        [attributes setObject: [NSColor blackColor]
                       forKey: NSForegroundColorAttributeName];
    }
    NSMutableParagraphStyle *style =[[NSMutableParagraphStyle alloc] init];
    [style setAlignment:NSLeftTextAlignment];
    [attributes setObject:style forKey:NSParagraphStyleAttributeName];
    NSSize size=[text sizeWithAttributes:attributes];
    NSRect rect=NSMakeRect(point.x-size.width/2, point.y, size.width, size.height);
    [text drawInRect:rect withAttributes:attributes];
    [style release];
}
-(void)DrawFRulerText:(NSString*)text Rect:(NSPoint)point
{
    NSMutableDictionary *attributes = [NSMutableDictionary dictionary];
    NSFont *currentFont = [NSFont userFontOfSize: 8.0];
    if (bold) {
        NSFontManager *fm = [NSFontManager sharedFontManager];
        NSFont *boldFont = [fm convertFont: currentFont
                               toHaveTrait: NSItalicFontMask];
        [attributes setObject: boldFont
                       forKey: NSFontAttributeName];
        [attributes setObject: [NSColor blackColor]
                       forKey: NSForegroundColorAttributeName];
        
        
    } else {
        
        [attributes setObject: currentFont
                       forKey: NSFontAttributeName];
        [attributes setObject: [NSColor blackColor]
                       forKey: NSForegroundColorAttributeName];
    }
    NSMutableParagraphStyle *style =[[NSMutableParagraphStyle alloc] init];
    [style setAlignment:NSLeftTextAlignment];
    [attributes setObject:style forKey:NSParagraphStyleAttributeName];
    NSSize size=[text sizeWithAttributes:attributes];
    NSRect rect=NSMakeRect(point.x, point.y-size.height/2, size.width, size.height);
    [text drawInRect:rect withAttributes:attributes];
    [style release];
}
- (void)drawTitle
{
    NSString *keynae=[NSString stringWithCString:m_KeyName encoding:NSASCIIStringEncoding];
    if (keynae==nil) {
        return;
    }
    NSRect bounds = self.bounds;
    NSPoint point=NSMakePoint(bounds.size.width/4, bounds.size.height);
    
    NSMutableDictionary *attributes = [NSMutableDictionary dictionary];
    NSFont *currentFont = [NSFont userFontOfSize: 16.0];
    if (bold) {
        NSFontManager *fm = [NSFontManager sharedFontManager];
        NSFont *boldFont = [fm convertFont: currentFont
                               toHaveTrait: NSItalicFontMask];
        [attributes setObject: boldFont
                       forKey: NSFontAttributeName];
        [attributes setObject: [NSColor blackColor]
                       forKey: NSForegroundColorAttributeName];
        
        
    } else {
        
        [attributes setObject: currentFont
                       forKey: NSFontAttributeName];
        [attributes setObject: [NSColor blackColor]
                       forKey: NSForegroundColorAttributeName];
    }
    NSMutableParagraphStyle *style =[[NSMutableParagraphStyle alloc] init];
    [style setAlignment:NSLeftTextAlignment];
    [attributes setObject:style forKey:NSParagraphStyleAttributeName];
    NSSize size=[keynae sizeWithAttributes:attributes];
    NSRect rect=NSMakeRect(point.x, point.y-size.height-5, size.width, size.height);
    [keynae drawInRect:rect withAttributes:attributes];
    [style release];
}

-(NSPoint)dataPointtoScreenPoint:(NSPoint) pointdata
{
    
    float maxd=MaxTrave;
    float maxf=MaxForce;
    unsigned int min_x=5;
    unsigned int min_y=5;
    unsigned int max_x=[self bounds].size.width-min_x;
    unsigned int max_y=[self bounds].size.height-min_y;
    NSPoint point;
    point.x=max_x*pointdata.x/maxd+5;
    point.y=max_y*pointdata.y/maxf+5;
    return point;
}
- (void)drawRuler:(CGContextRef)gc
{
    float maxd=MaxTrave;
    float maxf=MaxForce;
    unsigned int min_x=5;
    unsigned int min_y=5;
    
    unsigned int max_x=[self bounds].size.width-min_x;
    
    unsigned int max_y=[self bounds].size.height-min_y;
    
    
    float xstep=max_x*3/4/maxd;
    
    float ystep=max_y/maxf;
    
    [[NSColor blackColor] set];
    NSRect bounds = self.bounds;
    CGPoint p=CGPointMake(0,0);
    
    CGContextSetLineWidth(gc,1.5);
    CGContextMoveToPoint(gc, p.x + 5, p.y + 5);
    CGContextAddLineToPoint(gc, NSWidth(bounds), p.y + 5);
    CGContextStrokePath(gc);
    double x=5.0;
    float f=0.0;
    float i=0;
    int b = 0;
    while(true)
    {
        float coorx = maxd/100;
        i+= coorx ;
        b++;
        if (NSWidth(bounds)-10<=xstep*i+x) {
            break;
        }
        CGContextMoveToPoint(gc, xstep*i+x, p.y + 5);
        if (b%10==0) {
            CGContextSetLineWidth(gc,1);
            CGContextAddLineToPoint(gc, xstep*i+x, p.y + 15);
            CGContextStrokePath(gc);
            f+=maxd/10;
            [self DrawDRulerText:[NSString stringWithFormat:@"%0.3f",f] Rect:NSMakePoint(xstep*i+x, p.y + 15)];
        }else
        {
            CGContextSetLineWidth(gc,0.5);
            CGContextAddLineToPoint(gc, xstep*i+x, p.y + 10);
            CGContextStrokePath(gc);
        }
    }
    
    CGContextSetLineWidth(gc,1.5);
    CGContextMoveToPoint(gc, p.x + 5, p.y + 5-1.5);
    CGContextAddLineToPoint(gc, p.x + 5,NSHeight(bounds));
    CGContextStrokePath(gc);
    //    double steph=(NSHeight(bounds)-5)/50;
    double xh=5.0;
    f=0.0;
    i=0;
    b=0;
    while(true)
    {
        float coory = maxf/100;
        i+=coory;
        b++;
        if (NSHeight(bounds)-10<=ystep*i+xh) {
            break;
        }
        CGContextMoveToPoint(gc, p.x+5,ystep*i+xh);
        if (b%10==0) {
            CGContextSetLineWidth(gc,1);
            CGContextAddLineToPoint(gc, p.x+15,ystep*i+xh);
            CGContextStrokePath(gc);
            f+=maxf/10;
            [self DrawFRulerText:[NSString stringWithFormat:@"%0.02f",f] Rect:NSMakePoint(p.x+15,ystep*i+xh)];
        }else
        {
            CGContextSetLineWidth(gc,0.5);
            CGContextAddLineToPoint(gc, p.x+10,ystep*i+xh);
            CGContextStrokePath(gc);
        }
        
    }
    CGAffineTransform transform = CGAffineTransformMakeTranslation(0, 0);
    CGMutablePathRef path = CGPathCreateMutable();
    CGPathMoveToPoint(path, &transform, NSWidth(bounds)-10, 0);
    CGPathAddLineToPoint(path, &transform,NSWidth(bounds)-10, 10);
    CGPathAddLineToPoint(path, &transform,NSWidth(bounds), 5);
    CGPathCloseSubpath(path);
    CGContextFillPath(gc);
    CGContextStrokePath(gc);
    
    CGPathMoveToPoint(path, &transform, 0, NSHeight(bounds)-10);
    CGPathAddLineToPoint(path, &transform,10, NSHeight(bounds)-10);
    CGPathAddLineToPoint(path, &transform,5,  NSHeight(bounds));
    CGPathCloseSubpath(path);
    CGContextAddPath(gc, path);
    CGPathRelease(path);
    CGContextFillPath(gc);
    CGContextStrokePath(gc);
}
@end
