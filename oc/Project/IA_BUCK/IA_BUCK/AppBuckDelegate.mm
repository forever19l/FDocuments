//
//  AppDelegate.m
//  IA_BUCK
//
//  Created by Haitao on 22/4/16.
//  Copyright © 2016年 mess. All rights reserved.
//

#import "AppBuckDelegate.h"
#include "UICommon.h"
#include "RegexKitLite.h"

#define NOTIFICATIONINFO "Start_OK"
#define NOTIFICATIONNAME @"Start_Test"
int calflag = 0;    //校准框锁定按钮flag
int iAuthor = 0;
NSMutableDictionary * dicConfig;
NSMutableDictionary * dicResult;

NSArray * keyArr;
NSMutableString * strPath;


@implementation AppBuckDelegate

-(id)init
{
    self= [super init];
    if (self) {
        pRS232 = nil;
        strPath = [[NSMutableString alloc] initWithFormat:@"%@/%@",[[NSBundle mainBundle]resourcePath],@"Config.plist"];
        dicConfig = [[NSMutableDictionary alloc ]initWithContentsOfFile:strPath];
        dicResult = [[NSMutableDictionary alloc]init];
        if(dicConfig == nil)
        {
            dicConfig = [[NSMutableDictionary alloc]init];
            [dicConfig setObject:@"/vault/Intelli_log/" forKey:keyCSV];
            [dicConfig setObject:@"IA960" forKey:keySTATION];
            [dicConfig setObject:@"/dev/cu.usbserial-F-CTR" forKey:keyPORT];
            [dicConfig setObject:@"19200,n,8,1" forKey:keySETTING];
            
            NSMutableDictionary * limit = [[NSMutableDictionary alloc]init];
            [limit setObject:@"1.9" forKey:keyUPPER];
            [limit setObject:@"0.1" forKey:keyLOWER];
            [limit setObject:@"" forKey:keyUNIT];
            
            [limit setObject:@"Position1" forKey:keyNAME];
            [dicConfig setObject:limit forKey:keyITEM1];
            [limit setObject:@"Position2" forKey:keyNAME];
            [dicConfig setObject:limit forKey:keyITEM2];
            [limit setObject:@"Position3" forKey:keyNAME];
            [dicConfig setObject:limit forKey:keyITEM3];
            [dicConfig writeToFile:strPath atomically:YES];
        }
        keyArr = [[NSArray alloc ]initWithObjects:keyITEM1,keyITEM2,keyITEM3, nil];
        [self clearResult];
        dl = [[datalog alloc]init];
        timer = [NSTimer scheduledTimerWithTimeInterval:0.6 target:self selector:@selector(updateTime) userInfo:nil repeats:YES];
        [timer fire];
    }
    return  self;
}

#pragma mark TimeUpdate
- (void)updateTime
{
    if(startTime != 0)
    {
        NSTimeInterval ex = [[NSDate date]timeIntervalSince1970] -  startTime;
        NSDictionary * dic = [NSDictionary dictionaryWithObjectsAndKeys:[NSString stringWithFormat:@"%.03f sec", ex],@"time", nil];
        [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate date]];
        [self performSelectorOnMainThread:@selector(threadUpdate:) withObject:dic waitUntilDone:YES];
    }
}

- (void)threadUpdate:(NSDictionary *)dic
{
    NSString * time = [dic objectForKey:@"time"];
    [txtTime setStringValue:time];
}

#pragma mark clear result
- (void)clearResult
{
    [state0 setBackgroundColor:crIDLE];
    [state1 setBackgroundColor:crIDLE];
    [state2 setBackgroundColor:crIDLE];
    
    [state0 setStringValue:szIDLE];
    [state1 setStringValue:szIDLE];
    [state2 setStringValue:szIDLE];
    
    [check0 setHidden:NO];
    [check1 setHidden:NO];
    [check2 setHidden:NO];
    
    [dicResult setObject:[dicConfig objectForKey:keySTATION] forKey:keySTATION];
    [dicResult setObject:@"PASS" forKey:keySTATUS];
    [dicResult setObject:@"" forKey:keySTARTTIME];
    [dicResult setObject:@"" forKey:keyTOTALTIME];
    [dicResult setObject:@"" forKey:keySTATIONID];
    [dicResult setObject:@"" forKey:keySITEID];
    [dicResult setObject:@"" forKey:keySTARTTIME];
    [dicResult setObject:@"" forKey:keyFAILITEM];
    [dicResult setObject:@"" forKey:keySN];
    [dicResult setObject:@"" forKey:keyERRORMSG];
    [dicResult setObject:[dicConfig objectForKey:keyCSV] forKey:keyCSV];
    for(NSString * key in keyArr)
    {
        NSMutableDictionary * tmp = [dicConfig objectForKey:key];
        [tmp setObject:@"" forKey:keyRESULT];
        [dicResult setObject:tmp forKey:key];
    }
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender
{
    return YES;
}

- (void)comPortInit
{
    if(pRS232)
    {
        pRS232->Close();
        delete pRS232;
        pRS232 = nil;
    }
    pRS232 = new CRS232();
    int ret = pRS232->Open([[dicConfig objectForKey:keyPORT] UTF8String], [[dicConfig objectForKey:keySETTING] UTF8String]);
    if(ret < 0)
    {
        NSDictionary * userInfo = [NSDictionary dictionaryWithObjectsAndKeys:[NSString stringWithFormat:@"Open COM port Error : %@",[dicConfig objectForKey:keyPORT]],NSLocalizedDescriptionKey, nil];
        NSError * error = [NSError errorWithDomain:kERRORDOMAIN code:ret userInfo:userInfo];
        NSAlert * alert = [NSAlert alertWithError:error];
        [alert runModal];
        delete pRS232;
        pRS232 = nil;
        return;
    }
    pRS232->SetDetectString("\r\n");
    pRS232->AttachehView(txtView);
    pRS232->SetNotificationInfo("Start_OK", [NOTIFICATIONNAME UTF8String]);
}

-(void)awakeFromNib
{
    [state0 setBackgroundColor:crIDLE];
    [state1 setBackgroundColor:crIDLE];
    [state2 setBackgroundColor:crIDLE];
    
    [state0 setStringValue:szIDLE];
    [state1 setStringValue:szIDLE];
    [state2 setStringValue:szIDLE];
    
    [check0 setHidden:NO];
    [check1 setHidden:NO];
    [check2 setHidden:NO];

    [self comPortInit];
    [[NSNotificationCenter defaultCenter]addObserver:self selector:@selector(BtStart:) name:NOTIFICATIONNAME object:nil];
}

-(void)dealloc
{
    [dicConfig release];
    [dicResult release];
    [dl release];
    [super dealloc];
}

#pragma mark set cotroller state
-(void)setRunSates
{
    if([check0 state]==NSOnState)
    {
        [state0 setBackgroundColor:crRUN];
        [state0 setStringValue:szRUN];
        [check0 setHidden:YES];
    }
    
    if([check1 state]==NSOnState)
    {
        [state1 setBackgroundColor:crRUN];
        [state1 setStringValue:szRUN];
        [check1 setHidden:YES];
    }
    
    if([check2 state]==NSOnState)
    {
        [state2 setBackgroundColor:crRUN];
        [state2 setStringValue:szRUN];
        [check2 setHidden:YES];
    }
}

-(void)setStopstate
{
    [state0 setBackgroundColor:crERROR];
    [state1 setBackgroundColor:crERROR];
    [state2 setBackgroundColor:crERROR];
    
    [state0 setStringValue:szERROR];
    [state1 setStringValue:szERROR];
    [state2 setStringValue:szERROR];
    
    [check0 setHidden:NO];
    [check1 setHidden:NO];
    [check2 setHidden:NO];
}

-(void)setFinishState
{
    [stop setEnabled:false];
    [start setEnabled:true];
    [[NSNotificationCenter defaultCenter]addObserver:self selector:@selector(BtStart:) name:NOTIFICATIONNAME object:nil];
}


- (IBAction)btnCheck:(id)sender
{
    bool enable = ([sender state] == NSOnState);
    if([sender tag]==0)
    {
        [state0 setBackgroundColor: (enable ? crIDLE : crNA)];
        [state0 setStringValue:(enable?szIDLE:szNA)];
    }
    else if([sender tag]==1)
    {
        [state1 setBackgroundColor: (enable ? crIDLE : crNA)];
        [state1 setStringValue:(enable?szIDLE:szNA)];
    }
    else if([sender tag]==2)
    {
        [state2 setBackgroundColor: (enable ? crIDLE : crNA)];
        [state2 setStringValue:(enable?szIDLE:szNA)];
    }
}

#pragma mark test
-(void)compareResult:(int)index :(float)result
{
    NSColor * clor;
    NSString * str;
    NSString * key = [keyArr objectAtIndex:index];
    NSMutableDictionary * item = [dicResult objectForKey:key];
    
    float upper = [[item objectForKey:keyUPPER] floatValue];
    float lower = [[item objectForKey:keyLOWER]floatValue];
    
    if(result >= lower && result <= upper)
    {
        clor = crPASS;
        str = szPASS;
    }
    else
    {
        NSMutableString * failstr = [NSMutableString stringWithString:[dicResult objectForKey:keyFAILITEM]];
        [failstr appendString:[NSString stringWithFormat:@"%@;",[item objectForKey:keyNAME]]];
        [dicResult setObject:failstr forKey:keyFAILITEM];
        [dicResult setObject:@"FAIL" forKey:keySTATUS];
        clor = crFAIL;
        str = szFAIL;
    }
    
//    NSArray * state = [NSArray arrayWithObjects:state0,state1,state2, nil];
//    NSArray * value = [NSArray arrayWithObjects:value0,value1,value2, nil];
//    [[state objectAtIndex:index]setStringValue:str];
//    [[state objectAtIndex:index]setBackgroundColor:clor];
//    [[value objectAtIndex:index] setStringValue:[NSString stringWithFormat:@"%.6f",result]];
//
    NSDictionary * dic = [NSDictionary dictionaryWithObjectsAndKeys:[NSString stringWithFormat:@"%d",index],@"index",str,@"status",clor,@"color",[NSString stringWithFormat:@"%.6f",result],@"result", nil];
    
    [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate date]];
    [self performSelectorOnMainThread:@selector(updateStatus:) withObject:dic waitUntilDone:YES];
    
    [item setObject:[NSString stringWithFormat:@"%.6f",result] forKey:keyRESULT];
}

- (void)updateStatus:(NSDictionary *)dic
{
    int index = [[dic objectForKey:@"index"] intValue];
    NSString * status = [dic objectForKey:@"status"];
    NSColor * c = [dic objectForKey:@"color"];
    NSString * result = [dic objectForKey:@"result"];
    
    NSArray * state = [NSArray arrayWithObjects:state0,state1,state2, nil];
    NSArray * value = [NSArray arrayWithObjects:value0,value1,value2, nil];
    [[state objectAtIndex:index]setStringValue:status];
    [[state objectAtIndex:index]setBackgroundColor:c];
    [[value objectAtIndex:index] setStringValue:result];
}

-(IBAction)BtStart:(id)sender
{
    [[NSNotificationCenter defaultCenter]removeObserver:self name:NOTIFICATIONNAME object:nil];//移除通知
    [self clearResult];

    [dicResult setObject:TxtSnInput.stringValue forKey:keySN];
    NSDateFormatter * fmt = [[NSDateFormatter alloc]init];
    [fmt setDateFormat:@"HH:mm:ss.SSS"];
    NSString * date = [fmt stringFromDate:[NSDate date]];
    [fmt release];
    [dicResult setObject:date forKey:keySTARTTIME];
    startTime = [[NSDate date]timeIntervalSince1970];
    
//    [self movePosition:0 :5000];
    [self delay:1];
    [start setEnabled:false];
    [stop setEnabled:true];
    [self setRunSates];
//    [self runTestDemo];
    int c1 = [self cal:0];
    int c2 = [self cal:1];
    NSLog(@"!!!!k1=%d;k2=%d",c1,90+c2);

    if (c1 != 9998 && c2 != 9999 )
    {
        [self runTest:c1:c2];
        [self FPY];
        startTime = 0;
    }
    NSTimeInterval endTime = [[NSDate date]timeIntervalSince1970];
    NSString * time = [NSString stringWithFormat:@"%.03f",(endTime-startTime)];
    [dicResult setObject:time forKey:keyTOTALTIME];
    NSLog(@"dicresult=%@",dicResult);
    [dl saveLog:dicResult :keyArr];
    [txtTime setStringValue:time];
    [self movePosition:2 :5000];
    [self setFinishState];
}

- (void)FPY
{
    int pass = [[txtPass stringValue]intValue];
    int fail = [[txtFail stringValue]intValue];
    
    if([[dicResult objectForKey:keySTATUS]isEqualToString:@"PASS"])
        pass = pass + 1;
    else
        fail = fail + 1;
    int total = pass + fail;
    
    float fpy;
    if(fail == 0)
        fpy = 100;
    else
        fpy = (float)pass / (float)total;
    
    [txtPass setStringValue:[NSString stringWithFormat:@"%d",pass]];
    [txtFail setStringValue:[NSString stringWithFormat:@"%d", fail]];
    [txtTotal setStringValue:[NSString stringWithFormat:@"%d", total]];
    [txtFpy setStringValue:[NSString stringWithFormat:@"%.2f%%",fpy]];
}

-(IBAction)Btstop:(id)sender
{
    [stop setEnabled:false];
    [start setEnabled:true];
    [self setStopstate];
}

- (void)delay:(int)sec
{
    int i = 0;
    while(i<sec*2)
    {
        [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate date]];
        [NSThread sleepForTimeInterval:0.5];
        i++;
    }
}

-(int)cal:(int)position
{
    if (position == 0) {
        NSString *k1 = [calleft stringValue];
        if ([k1  isEqual: @"9998"] )
        {
            if([check0 state] == NSOnState)
            {
                float result = [self testPosition:0 :5000];
                [self compareResult:0 :result];
            }
        }
        return [k1 floatValue]*1.0;
    }
    else
    {
        NSString *k2 = [calright stringValue];
        if ([k2  isEqual:@"9999"])
        {
            [self delay:1];
            if([check1 state] == NSOnState)
            {
                float result = [self testPosition:1 :5000];
                [self compareResult:1 :result];
            }
        }
    }
    return -999;
}

- (float)testPosition:(int)index :(int)timeout
{
//    NSArray * action = [NSArray arrayWithObjects:@"@ACTION:LEFT_DOWN\r\n",@"@ACTION:RIGHT_DOWN\r\n", nil];
    NSArray * measure = [NSArray arrayWithObjects:@"@MEASURE:VOLTAGE1\r\n",@"@MEASURE:VOLTAGE2\r\n", nil];
//    const char * tmp = pRS232->WriteReadString([[action objectAtIndex:index]UTF8String], timeout);
//    NSString * ret = [NSString stringWithUTF8String:tmp];
//    if([ret containsString:@"OK"])
//    {
    NSLog(@"%s",[[measure objectAtIndex:index]UTF8String]);
        const char * tmp = pRS232->WriteReadString([[measure objectAtIndex:index]UTF8String], timeout);
        NSString * ret = [NSString stringWithUTF8String:tmp];
        return [ret floatValue]/1000;//（A*2*1000）/5000）*5//5.4更新底层直接返回计算后的值
//    }
//    else
//    {
//        return -999;
//    }
}

- (BOOL)movePosition:(int)position :(int)timeout
{
    if(position==0)//move position 1
    {
        pRS232->WriteReadString("@ACTION:LEFT_UP\r\n", timeout);
        [self delay:1];
        pRS232->WriteReadString("@ACTION:RIGHT_UP\r\n", timeout);
        [self delay:1];
        pRS232->WriteReadString("@ACTION:FIXTURE_OUT\r\n", timeout);
        [self delay:1];
        pRS232->WriteReadString("@ACTION:LEFT_DOWN\r\n", timeout);
        [self delay:1];
        pRS232->WriteReadString("@ACTION:RIGHT_DOWN\r\n", timeout);
        return YES;
    }
    else if(position==1)//move position 2
    {
        pRS232->WriteReadString("@ACTION:LEFT_UP\r\n", timeout);
        [self delay:1];
        pRS232->WriteReadString("@ACTION:RIGHT_UP\r\n", timeout);
        [self delay:1];
        pRS232->WriteReadString("@ACTION:FIXTURE_IN\r\n", timeout);
        [self delay:1];
        pRS232->WriteReadString("@ACTION:LEFT_DOWN\r\n", timeout);
        return YES;
    }
    else//open fixture
    {
        pRS232->WriteReadString("@ACTION:LEFT_UP\r\n", timeout);
        [self delay:1];
        pRS232->WriteReadString("@ACTION:RIGHT_UP\r\n", timeout);
        [self delay:1];
        pRS232->WriteReadString("@ACTION:FIXTURE_OUT\r\n", timeout);
        return YES;
    }
}

-(void)ShowMsg:(NSString*)msg
{
    [self performSelectorOnMainThread:@selector(fnErrMsg:) withObject:msg waitUntilDone:NO];
}

-(void)fnErrMsg:(id)par
{
    NSRunAlertPanel(@"Fixture Controlller Information", @"%@", @"OK", nil, nil, par);
}

- (void)runTest:(float)kleft:(float)kright
{
    if([check0 state] == NSOnState)
    {
        float result = ([self testPosition:0 :5000]+kleft);
        [self compareResult:0 :result];
    }
    [self delay:1];
    if([check1 state] == NSOnState)
    {
        float result = ([self testPosition:1 :5000]+kright);
        [self compareResult:1 :result];
    }
    NSLog(@"test 0,1 position ok!");
    [self movePosition:1 :5000];//move position 2
    
    [self delay:1];
    if([check2 state] == NSOnState)
    {
        float result = ([self testPosition:0 :5000]+kleft);
        [self compareResult:2 :result];
    }
    
    [self movePosition:2 :5000];//out
    
    NSTimeInterval endTime = [[NSDate date]timeIntervalSince1970];
    NSString * time = [NSString stringWithFormat:@"%.03f",(endTime-startTime)];
    [dicResult setObject:time forKey:keyTOTALTIME];
    [dl saveLog:dicResult :keyArr];
    [txtTime setStringValue:time];
    [self setFinishState];
//    [self ShowMsg:@"test done!"];
}
 

- (void)runTestDemo
{
    if([check0 state] == NSOnState)
    {
        float result = 1.0;
        NSLog(@"Test1 : %s",pRS232->WriteReadString("test1\r\n", 5000));
        [self compareResult:0 :result];
    }
    
    if([check1 state] == NSOnState)
    {
        float result = 1.1;
        NSLog(@"Test2 : %s",pRS232->WriteReadString("test2\r\n", 5000));
        [self compareResult:1 :result];
    }
    
    if([check2 state] == NSOnState)
    {
        float result = 1.2;
        NSLog(@"Test3 : %s",pRS232->WriteReadString("test3\r\n", 5000));
        [self compareResult:2 :result];
    }
    
    NSTimeInterval endTime = [[NSDate date]timeIntervalSince1970];
    NSString * time = [NSString stringWithFormat:@"%.03f",(endTime-startTime)];
    [dicResult setObject:time forKey:keyTOTALTIME];
    [dl saveLog:dicResult :keyArr];
    [txtTime setStringValue:time];
    [self setFinishState];
}

#pragma mark menu
- (BOOL)validateUserInterfaceItem:(id < NSValidatedUserInterfaceItem >)anItem
{
    if (anItem.tag == 0)
        return YES;
    if (anItem.tag != 0 && iAuthor != 0) {
        return YES;
    }
    return NO;
}


#pragma mark CONFIG ACTION
- (IBAction)btnConfig:(id)sender
{
    [mainWnd beginSheet:wndCom completionHandler:^(NSModalResponse returnCode){
        switch (returnCode) {
            case NSModalResponseOK:
                [self comPortInit];
                [dicConfig writeToFile:strPath atomically:YES];
                break;
            case NSModalResponseCancel:
                
                break;
            default:
                break;
        }
    }];
}

- (IBAction)btnLogIn:(id)sender
{
    if([menuLog.title isEqualToString:@"LogOut"])
    {
        iAuthor = 0;
        [menuLog setTitle:@"LogIn"];
        return;
    }
    [mainWnd beginSheet:wndUser completionHandler:^(NSModalResponse returnCode){
        switch (returnCode) {
            case NSModalResponseOK:
                [menuLog setTitle:@"LogOut"];
                break;
            case NSModalResponseCancel:
                
                break;
            default:
                break;
        }
    }];
}

- (IBAction)btnItemEdit:(id)sender
{
    [mainWnd beginSheet:wndItemEdit completionHandler:^(NSModalResponse returnCode){
        switch (returnCode) {
            case NSModalResponseOK:
                [dicConfig writeToFile:strPath atomically:YES];
                break;
            case NSModalResponseCancel:
                
                break;
            default:
                break;
        }
    }];
}

- (IBAction)menuDebug:(id)sender
{
    [wndDebug center];
    [wndDebug makeKeyAndOrderFront:sender];
}

- (IBAction)btnSend:(id)sender
{
    NSArray * arr = [comboCmd objectValues];
    if(comboCmd.stringValue && [comboCmd.stringValue length]>0)
    {
        if(![arr containsObject:comboCmd.stringValue])
            [comboCmd addItemWithObjectValue:comboCmd.stringValue];
        if(pRS232)
        {
            pRS232->WriteString([[NSString stringWithFormat:@"%@\r\n",comboCmd.stringValue] UTF8String]);
        }
    }
}

- (IBAction)calbutton:(id)sender
{
    if (calflag == 0) {
        [calleft setEditable:true];
        [calright setEditable:true];
//        [calbutton ]
        calflag = 1 ;
        [self ShowMsg:@"UNLOCK CAL !"];
    }
    else
    {
        [calleft setEditable:false];
        [calright setEditable:false];
        calflag = 0 ;
        [self ShowMsg:@"LOCK CAL OK!\rPlease click ‘SN INPUT’ fied！"];
    }
}
@end
