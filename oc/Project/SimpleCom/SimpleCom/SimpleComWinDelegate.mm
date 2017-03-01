//
//  SimpleComWinDelegate.cpp
//  SimpleCom
//
//  Created by Liu Liang on 13-12-23.
//  Copyright (c) 2013年 Liu Liang. All rights reserved.
//

#include "SimpleComWinDelegate.h"
#import <CoreLib/Common.h>
#include "VISA/visa.h"
#import "RegexKitLite.h"
#include <CoreLib/PathManager.h>
#include "CoreLib/TestEngine.h"
#import "SendButton.h"
#import "SocketFactory.h"

#define INTERFACE_NA @"NA"
#define INTERFACE_SERIAL @"Serial"
#define INTERFACE_GPIB @"GPIB"
#define INTERFACE_LAN @"LAN"
#define INTERFACE_USB @"USB"

SimpleComWinDelegate* SimpleComWinDelegateObj = nil;

@implementation SimpleComWinDelegate

@synthesize dicConfiguration,strCfgFile ;
- (id)init
{
    self = [super init];
    dicConfiguration = nil;
    
    SimpleCom = new CSimpleCom();
    
    if (self) {
        //key=simplecom_name   value=simplecom_config(format: "type/////device_name;setting")
        dicConfiguration = [[NSMutableDictionary alloc] init];
        arrCommands = [[NSMutableArray alloc] initWithObjects:@"*idn?", nil];
#if 0
        NSString * contentPath = [[NSBundle bundleForClass:[self class]]executablePath];
        contentPath = [[contentPath stringByDeletingLastPathComponent]stringByDeletingLastPathComponent];
        self.strCfgFile = [contentPath stringByAppendingPathComponent:@"/Config/GT_SimpleCom_Config.plist"];
        [[NSFileManager defaultManager] createDirectoryAtPath:[self.strCfgFile stringByDeletingLastPathComponent] withIntermediateDirectories:YES attributes:nil error:nil];
#else
        NSString * config_dir  = [[PathManager sharedManager] configPath];
        NSString * config_file = [config_dir stringByAppendingPathComponent:@"GT_SimpleCom_Config.plist"];
        [[NSFileManager defaultManager] createDirectoryAtPath:config_dir withIntermediateDirectories:YES attributes:nil error:nil];
        self.strCfgFile = config_file;
#endif
        [[NSNotificationCenter defaultCenter] addObserver:self selector
                                                         :@selector(handleNotification:) name
                                                         :kNotificationAttachMenu object
                                                         :nil];
    }
    SimpleComWinDelegateObj = self;
    return self;
}

- (void)dealloc
{
    [dicConfiguration writeToFile:strCfgFile atomically:YES];
    delete SimpleCom ;
    
    [dicConfiguration release] ;
    [arrCommands release];
    [[NSNotificationCenter defaultCenter] removeObserver:self] ;
    [super dealloc];
}

-(void)awakeFromNib
{
    [combSimpleComType removeAllItems];
    [combSimpleComType addItemWithObjectValue:INTERFACE_SERIAL];
    [combSimpleComType addItemWithObjectValue:INTERFACE_LAN];
    [combSimpleComType selectItemWithObjectValue:INTERFACE_SERIAL];
    
    [self OnChangeInterfaceType:combSimpleComType];
    [combSimpleCom removeAllItems];
    [combSerialPortDevice removeAllItems];

    [self btn_SearchDUTDevice:nil];
    [self RemoveAlltabView];
    [self LoadConfig:strCfgFile];
}
-(void)RemoveTabViewWithName:(NSString*)name
{
    NSArray *arr=[m_tabview tabViewItems];
    int count = (int)[arr count];
    for (int i=0;i<count;i++)
    {
        NSTabViewItem* tempView = [m_tabview tabViewItemAtIndex:i];
        if ([[tempView label] isEqualToString:name])
        {
            [m_tabview removeTabViewItem:tempView];
            break;
        }
    }
}
-(void)RemoveAlltabView
{
    NSArray *arr=[m_tabview tabViewItems];
    int count = (int)[arr count];
    for (int i=0;i<count;i++)
    {
        NSTabViewItem* tempView = [m_tabview tabViewItemAtIndex:i];
        [m_tabview removeTabViewItem:tempView];
    }
}
- (void)InterfaceNameViewKeyUp:(NSControl *)control Event:(NSEvent *)theEvent
{
    NSString *strname=[control stringValue];
     NSLog(@"IAMotorButtonmouseUp:%@\r",strname);
    id inter=SimpleCom->getInterface(strname);
    if (!inter)
    {
        [btCreateInterface setTitle:@"Create New"];
    }
    else if([inter isOpen]) {
        [btCreateInterface setTitle:@"Refresh"];
    }else
    {
        [btCreateInterface setTitle:@"Open"];
    }
}
- (void)tabView:(NSTabView *)tabView didSelectTabViewItem:(NSTabViewItem *)tabViewItem
{
    [combSimpleCom selectItemWithObjectValue:[tabViewItem identifier]];
}
-(IBAction)OnChangeInterfaceName:(id)sender
{
    NSDictionary* dic = [dicConfiguration valueForKey:[sender objectValueOfSelectedItem]];
    if (dic == nil) {
        return;
    }
    [txtSimpleComName setStringValue:[dic objectForKey:@"Name"]];
    id inter=SimpleCom->getInterface([dic objectForKey:@"Name"]);
    if (inter && [inter isOpen]) {
        [btCreateInterface setTitle:@"Refresh"];
    }else
    {
        [btCreateInterface setTitle:@"Open"];
    }
    [m_tabview selectTabViewItemWithIdentifier:[sender objectValueOfSelectedItem]];
    
    eInstrInterfaceType type=(eInstrInterfaceType)[[dic objectForKey:@"type"] intValue];
    if (type==eIITSerial)
    {
        [combSimpleComType selectItemWithObjectValue:INTERFACE_SERIAL];
        [self OnChangeInterfaceType:combSimpleComType];
        [btIsSendLogtoPDCA setState:[[dic objectForKey:@"SENDLOGTOPDCA"] integerValue]];
        [btIsSaveLog setState:[[dic objectForKey:@"SAVELOG"] integerValue]];
        [btIsEcho setState:[[dic objectForKey:@"ECHO"] integerValue]];
        NSArray * arrSetting=[[dic objectForKey:@"RscName"] componentsSeparatedByString:@";"];
        NSString* strDUTAddr=[arrSetting objectAtIndex:0];
        NSString* strDUTSetting=[arrSetting objectAtIndex:1];
        
        [combSerialPortDevice setStringValue:strDUTAddr];
        [txtSetting setStringValue:strDUTSetting];
    }else if (type==eIITLan)
    {
        [combSimpleComType selectItemWithObjectValue:INTERFACE_LAN];
        [self OnChangeInterfaceType:combSimpleComType];
        [btIsSendLogtoPDCA setState:[[dic objectForKey:@"SENDLOGTOPDCA"] integerValue]];
        [btIsSaveLog setState:[[dic objectForKey:@"SAVELOG"] integerValue]];
        [btIsEcho setState:[[dic objectForKey:@"ECHO"] integerValue]];
        NSArray * arrSetting=[[dic objectForKey:@"RscName"] componentsSeparatedByString:@";"];
        NSString* strDUTAddr=[arrSetting objectAtIndex:0];
        NSString* strDUTSetting=[arrSetting objectAtIndex:1];
        
        [txtServerIP setStringValue:strDUTAddr];
        [txtPort setStringValue:strDUTSetting];
    }
    
}
-(IBAction)btSend:(id)sender
{
    NSTabViewItem* item=[m_tabview selectedTabViewItem];
    NSString *name=[item identifier];
    NSComboBox *Combox=[self getSubview:[item view] Identity:@"AppendString"];
    NSComboBox *txtCmd=[self getSubview:[item view] Identity:@"Txt_CMD"];
    
    SimpleComFactory* inface= SimpleCom->getInterface(name);
    NSString *strSend=[txtCmd stringValue];
    if ([strSend isEqualToString:@""])
    {
        NSAlert* alert = [[[NSAlert alloc] init] autorelease];
        [alert setMessageText:@"Tip"];
        [alert setInformativeText:@"please write down you command!"];
        [alert setAlertStyle:NSWarningAlertStyle];
        [alert runModal];
        return;
    }
    if (![inface isOpen])
    {
        NSAlert* alert = [[[NSAlert alloc] init] autorelease];
        [alert setMessageText:@"Tip"];
        [alert setInformativeText:@"please open you port!"];
        [alert setAlertStyle:NSWarningAlertStyle];
        [alert runModal];
        return;
    }
    if ([txtCmd indexOfItemWithObjectValue:strSend]==NSNotFound)
    {
        [txtCmd addItemWithObjectValue:strSend];
    }
    if ([[Combox stringValue] isEqualToString:@"NA"]) {
        strSend=[txtCmd stringValue];
    }else if ([[Combox stringValue] isEqualToString:@"\\r"])
    {
        strSend=[strSend stringByAppendingString:@"\r"];
    }else if ([[Combox stringValue] isEqualToString:@"\\n"]) {
        strSend=[strSend stringByAppendingString:@"\n"];
    }else if ([[Combox stringValue] isEqualToString:@"\\r\\n"]) {
       strSend=[strSend stringByAppendingString:@"\r\n"];
    }else if ([[Combox stringValue] isEqualToString:@"\\n\\r"]) {
        strSend=[strSend stringByAppendingString:@"\n\r"];
    }
    [inface WriteString:[strSend cStringUsingEncoding:NSASCIIStringEncoding]];
}
- (void)IAMotorButtonmouseDown:(NSControl *)control Event:(NSEvent *)theEvent
{
    [self btSend:control];
}
-(void)CreatetabView:(NSString*)name
{

    NSView* item =[[[NSView alloc] initWithFrame:[m_tabitemview frame]] autorelease];
    
//    NSView* mlba_Cmd=[self getSubview:m_tabitemview Identity:@"LAB_CMD"];
//    NSTextField *lba_Cmd=[[[NSTextField alloc]initWithFrame:[mlba_Cmd frame]] autorelease];
//    [lba_Cmd setBezelStyle:[mlba_Cmd bezelStyle]];
//    [item addSubview:lba_Cmd];
    
    NSView* mTxt_CMD=[self getSubview:m_tabitemview Identity:@"Txt_CMD"];
    NSComboBox *Txt_CMD=[[[NSComboBox alloc]initWithFrame:[mTxt_CMD frame]] autorelease];
    [Txt_CMD setIdentifier:@"Txt_CMD"];
    //[Txt_CMD setBezelStyle:[mTxt_CMD bezelStyle]];
    [Txt_CMD setButtonBordered:NO];
    [item addSubview:Txt_CMD];
    
    
    NSComboBox* mAppendString=[self getSubview:m_tabitemview Identity:@"AppendString"];
    NSComboBox *AppendString=[[[NSComboBox alloc]initWithFrame:[mAppendString frame]] autorelease];
    [AppendString setIdentifier:@"AppendString"];
    [AppendString setBezelStyle:[mAppendString bezelStyle]];
    [AppendString addItemsWithObjectValues:[mAppendString objectValues]];
    [AppendString selectItemWithObjectValue:@"\\n"];
    [item addSubview:AppendString];
    
    
    SendButton* mbtSend=[self getSubview:m_tabitemview Identity:@"btSend"];
    
    SendButton *btSend=[[[SendButton alloc]initWithFrame:[mbtSend frame]] autorelease];
    [btSend setTitle:[mbtSend title]];
    [btSend setIdentifier:@"btSend"];
    [btSend setDelegate:self];
    [btSend setBezelStyle:[mbtSend bezelStyle]];
    [item addSubview:btSend];
    
    
    
    ColorButtonView* mp=[self getSubview:m_tabitemview Identity:@"ColorButtonView"];
    ColorButtonView *p=[[[ColorButtonView alloc]initWithFrame:[mp frame]] autorelease];
    [p setBezelStyle:[mp bezelStyle]];
    [p setEnabled:NO];
    [p setIdentifier:@"ColorButtonView"];
    [item addSubview:p];
    
    
    SerialPortView* mp1=[self getSubview:m_tabitemview Identity:@"SerialPortView"];
    NSScrollView* mpScrollView = [self getSubview:m_tabitemview Identity:@"ScrollView"];
    
    NSScrollView* pScrollView = [[[NSScrollView alloc] initWithFrame:[mpScrollView frame]] autorelease];
    //NSScrollView* pScrollView = [[[NSScrollView alloc] in] autorelease];
    SerialPortView *p1=[[[SerialPortView alloc]initWithFrame:[mp1 frame]] autorelease];
    [p1 setIdentifier:@"SerialPortView"];
    [p1 setSelectable:YES];
    [p1 setEditable:NO];
    [p1 setUsesFindPanel:YES];
    
    [pScrollView setHasVerticalScroller:YES];
    [pScrollView setDocumentView:p1];

    //    [p1 setBezelStyle:[mp1 bezelStyle]];
    [item addSubview:pScrollView];
    
    NSTabViewItem *newItem = [[NSTabViewItem alloc] initWithIdentifier:name];
    [newItem setLabel:name];
    
    [m_tabview addTabViewItem:newItem];
    [m_tabview selectTabViewItem:newItem];
    [newItem setView:item];
    [newItem release];
    
//    SendButton *bt=[self getSubview:item Identity:@"btSend"];
//    [bt setDelegate:self];

    
}
-(id)getSubview:(NSView*)view Identity:(NSString *)Identity
{
    NSArray *arr=[view subviews];
    for (NSView *subview in arr) {
        if ([subview identifier]!=nil)
        {
            if ([[subview identifier] isEqualToString:Identity]) {
                return subview;
            }else
            {
                NSView *v=[self getSubview:subview Identity:Identity];
                if (v!=nil) {
                    return v;
                }
            }
        }else
        {
            NSView *v=[self getSubview:subview Identity:Identity];
            if (v!=nil) {
                return v;
            }
        }
    }
    return nil;
}
-(int)LoadConfig:(NSString *)strfile
{
    NSFileManager * fm = [NSFileManager defaultManager];
    
    //default parameter
    if (![fm fileExistsAtPath:strfile]) {
        [self performSelectorOnMainThread:@selector(ShowWarning:) withObject:@"Warning: No simplecom created!" waitUntilDone:NO];
    }
    else
    {
        [dicConfiguration setValuesForKeysWithDictionary:[NSDictionary dictionaryWithContentsOfFile:strfile]];
    }
    [self InitSimpleComs];
    return 0;
}

-(void)InitSimpleComs
{
    int count = 0;
    for (NSString* strSimpleComName in dicConfiguration){
        NSString* name = nil;
        if ([strSimpleComName length] == 0) {
            count++;
            name = [NSString stringWithFormat:@"NoName%d",count];
        }else{
            name = strSimpleComName;
        }
        
        NSDictionary* dic = [dicConfiguration valueForKey:strSimpleComName];
        SimpleComSetting *setting=[self CreateSettingbyDic:dic];
        if (setting!=nil) {
            [self CreatetabView:name];
            [m_tabview selectTabViewItemWithIdentifier:name];
            NSInteger index= [m_tabview indexOfTabViewItemWithIdentifier:name];
            NSTabViewItem *item=[m_tabview tabViewItemAtIndex:index];;
            SerialPortView* view=[self getSubview:[item view] Identity:@"SerialPortView"];
            [setting SetLogView:view];
            ColorButtonView* colorview=[self getSubview:[item view] Identity:@"ColorButtonView"];
            [setting setconnectStatusView:colorview];
            int ret=SimpleCom->Create(setting);
            if (ret <= count) {
                NSString* strMessage = [NSString stringWithFormat:@"Failed to create simplecom %@.", name];
                [self ShowWarning:strMessage];
            }
            [setting release];
        }
    }

    [self btScanSimpleCom:nil];
    [self btSaveConfig:nil];
    if([combSimpleCom numberOfItems]>0)
    {
        [combSimpleCom selectItemAtIndex:0];
        [self OnChangeInterfaceName:combSimpleCom];
    }
}

#pragma mark-inner function


-(IBAction)btRescanVisaDevice:(id)sender
{

    return;
}

-(IBAction)btScanSimpleCom:(id)sender
{
    [combSimpleCom setStringValue:@""];
    [combSimpleCom removeAllItems];
    NSArray *arr=[dicConfiguration allKeys];
    if (arr) {
        for (int i=0; i<[arr count]; i++){
            [combSimpleCom addItemWithObjectValue:[arr objectAtIndex:i]];
            //if (!i) [combSimpleCom setStringValue:[arr objectAtIndex:i]];
        }
    }
}
-(SimpleComSetting*)CreateSetting
{
    if ([[combSimpleComType stringValue] isEqualToString:INTERFACE_SERIAL])
    {
        
        NSString* strRscName = [combSerialPortDevice stringValue];//rsc_name,dev_name
        if (strRscName == nil)
        {
            NSAlert *alert = [[[NSAlert alloc] init] autorelease];
            [alert setMessageText:[NSString stringWithUTF8String:"Create interface"]];
            [alert setInformativeText:[NSString stringWithFormat:@"rsc_name is nil"]];
            
            [alert setAlertStyle:NSWarningAlertStyle];
            [alert runModal];
            return nil;
        }
        if ([txtSetting stringValue] == nil) {
            NSAlert *alert = [[[NSAlert alloc] init] autorelease];
            [alert setMessageText:[NSString stringWithUTF8String:"Create interface"]];
            [alert setInformativeText:[NSString stringWithFormat:@"Setting is nil"]];
            
            [alert setAlertStyle:NSWarningAlertStyle];
            [alert runModal];
            return nil;
        }
        SimpleComSetting *setting=[[SimpleComSetting alloc] init] ;
        NSString* strConfig = [NSString stringWithFormat:@"%@;%@", strRscName, [txtSetting stringValue]];
        [setting setInterfaceName:[txtSimpleComName stringValue]];
        [setting setType:eIITSerial];
        [setting setSetting:strConfig];
        [setting setisSaveLog:[btIsSaveLog state]];
        [setting setisSendLogtoPDCA:[btIsSendLogtoPDCA state]];
        [setting setisEcho:[btIsEcho state]];
        return setting;
     }
    return nil;
}
-(SimpleComSetting*)CreateSettingbyDic:(NSDictionary*)dic
{
    if ([[dic objectForKey:@"type"] intValue]==eIITSerial)
    {
        SimpleComSetting *setting=[[[SimpleComSetting alloc] init] autorelease];
        [setting setInterfaceName:[dic objectForKey:@"Name"]];
        [setting setType:eIITSerial];
        [setting setSetting:[dic objectForKey:@"RscName"]];
        [setting setisSaveLog:[[dic objectForKey:@"SAVELOG"] integerValue]];
        [setting setisSendLogtoPDCA:[[dic objectForKey:@"SENDLOGTOPDCA"] integerValue]];
        [setting setisEcho:[[dic objectForKey:@"ECHO"] integerValue]];
        return [setting retain];
    }else if ([[dic objectForKey:@"type"] intValue]==eIITLan)
    {
        SimpleComSetting *setting=[[[SimpleComSetting alloc] init] autorelease];
        [setting setInterfaceName:[dic objectForKey:@"Name"]];
        [setting setType:eIITLan];
        [setting setSetting:[dic objectForKey:@"RscName"]];
        [setting setisSaveLog:[[dic objectForKey:@"SAVELOG"] integerValue]];
        [setting setisSendLogtoPDCA:[[dic objectForKey:@"SENDLOGTOPDCA"] integerValue]];
        [setting setisEcho:[[dic objectForKey:@"ECHO"] integerValue]];
        return [setting retain];
    }
    return nil;
}
-(NSDictionary*)getDicFromWin
{
    NSString* name = [txtSimpleComName stringValue];
    if ([name length] == 0)
    {
        NSAlert *alert = [[[NSAlert alloc] init] autorelease];
        [alert setMessageText:[NSString stringWithUTF8String:"Create interface"]];
        [alert setInformativeText:[NSString stringWithFormat:@"Name is nil"]];
        [alert setAlertStyle:NSWarningAlertStyle];
        [alert runModal];
        return nil;
    }
    
    NSMutableDictionary *dic=[[[NSMutableDictionary alloc] init] autorelease];
    
    if ([[combSimpleComType stringValue] isEqualToString:INTERFACE_SERIAL])
    {
        NSString* strRscName = [combSerialPortDevice stringValue];//rsc_name,dev_name
        if (strRscName == nil)
        {
            NSAlert *alert = [[[NSAlert alloc] init] autorelease];
            [alert setMessageText:[NSString stringWithUTF8String:"Create interface"]];
            [alert setInformativeText:[NSString stringWithFormat:@"rsc_name is nil"]];
            
            [alert setAlertStyle:NSWarningAlertStyle];
            [alert runModal];
            return nil;
        }
        if ([txtSetting stringValue] == nil) {
            NSAlert *alert = [[[NSAlert alloc] init] autorelease];
            [alert setMessageText:[NSString stringWithUTF8String:"Create interface"]];
            [alert setInformativeText:[NSString stringWithFormat:@"Setting is nil"]];
            
            [alert setAlertStyle:NSWarningAlertStyle];
            [alert runModal];
            return nil;
        }
        NSString* strConfig = [NSString stringWithFormat:@"%@;%@", strRscName, [txtSetting stringValue]];
        [dic setObject:name forKey:@"Name"];
        [dic setObject:strConfig forKey:@"RscName"];
        [dic setObject:[NSNumber numberWithInteger:eIITSerial] forKey:@"type"];
        [dic setObject:[NSNumber numberWithInteger:[btIsSendLogtoPDCA state]] forKey:@"SENDLOGTOPDCA"];
        [dic setObject:[NSNumber numberWithInteger:[btIsSaveLog state]] forKey:@"SAVELOG"];
        [dic setObject:[NSNumber numberWithInteger:[btIsEcho state]] forKey:@"ECHO"];
        return [dic retain];
    }else if ([[combSimpleComType stringValue] isEqualToString:INTERFACE_LAN])
    {
        NSString* strRscName = [txtServerIP stringValue];//rsc_name,dev_name
        if (strRscName == nil)
        {
            NSAlert *alert = [[[NSAlert alloc] init] autorelease];
            [alert setMessageText:[NSString stringWithUTF8String:"Create interface"]];
            [alert setInformativeText:[NSString stringWithFormat:@"rsc_name is nil"]];
            
            [alert setAlertStyle:NSWarningAlertStyle];
            [alert runModal];
            return nil;
        }
        if ([txtPort integerValue]<100) {
            NSAlert *alert = [[[NSAlert alloc] init] autorelease];
            [alert setMessageText:[NSString stringWithUTF8String:"Create interface"]];
            [alert setInformativeText:[NSString stringWithFormat:@"Setting is nil"]];
            
            [alert setAlertStyle:NSWarningAlertStyle];
            [alert runModal];
            return nil;
        }
        NSString* strConfig = [NSString stringWithFormat:@"%@;%@", [txtServerIP stringValue], [txtPort stringValue]];
        [dic setObject:name forKey:@"Name"];
        [dic setObject:strConfig forKey:@"RscName"];
        [dic setObject:[NSNumber numberWithInteger:eIITLan] forKey:@"type"];
        [dic setObject:[NSNumber numberWithInteger:[btIsSendLogtoPDCA state]] forKey:@"SENDLOGTOPDCA"];
        [dic setObject:[NSNumber numberWithInteger:[btIsSaveLog state]] forKey:@"SAVELOG"];
        [dic setObject:[NSNumber numberWithInteger:[btIsEcho state]] forKey:@"ECHO"];
        return [dic retain];
    }
    return nil;
}
-(void)SetSettingWinbyDic:(NSDictionary *)dic
{
    if ([[dic objectForKey:@"type"] intValue]==eIITSerial) {
    
        [txtSimpleComName setStringValue:[dic objectForKey:@"Name"]];
        [combSimpleComType setStringValue:INTERFACE_SERIAL];
        
        NSArray * arrSetting=[[dic objectForKey:@"RscName"] componentsSeparatedByString:@";"];
        NSString* strDUTAddr=[arrSetting objectAtIndex:0];
        NSString* strDUTSetting=[arrSetting objectAtIndex:1];
        [combSerialPortDevice setStringValue:strDUTAddr];
        [txtSetting setStringValue:strDUTSetting];
        [btIsSendLogtoPDCA setState:[[dic objectForKey:@"SENDLOGTOPDCA"] integerValue]];
        [btIsSaveLog setState:[[dic objectForKey:@"SAVELOG"] integerValue]];
        [btIsEcho setState:[[dic objectForKey:@"ECHO"] integerValue]];
        return;
    
    }else if ([[dic objectForKey:@"type"] intValue]==eIITLan){
        
        [txtSimpleComName setStringValue:[dic objectForKey:@"Name"]];
        [combSimpleComType setStringValue:INTERFACE_LAN];
        
        NSArray * arrSetting=[[dic objectForKey:@"RscName"] componentsSeparatedByString:@";"];
        NSString* strDUTAddr=[arrSetting objectAtIndex:0];
        NSString* strDUTSetting=[arrSetting objectAtIndex:1];
        [txtServerIP setStringValue:strDUTAddr];
        [txtPort setStringValue:strDUTSetting];
        [btIsSendLogtoPDCA setState:[[dic objectForKey:@"SENDLOGTOPDCA"] integerValue]];
        [btIsSaveLog setState:[[dic objectForKey:@"SAVELOG"] integerValue]];
        [btIsEcho setState:[[dic objectForKey:@"ECHO"] integerValue]];
        return;
        
    }
}

-(IBAction)btCreateSimpleCom:(id)sender
{
    NSString* name = [txtSimpleComName stringValue];
    if ([name length] == 0)
    {
        NSAlert *alert = [[[NSAlert alloc] init] autorelease];
        [alert setMessageText:[NSString stringWithUTF8String:"Create interface"]];
        [alert setInformativeText:[NSString stringWithFormat:@"Name is nil"]];
        [alert setAlertStyle:NSWarningAlertStyle];
        [alert runModal];
        return;
    }
    [self btScanSimpleCom:nil];
    int count = (int)[combSimpleCom numberOfItems];
//    eInstrInterfaceType type = eIITNa;
    if ([[combSimpleComType stringValue] isEqualToString:INTERFACE_SERIAL])
    {
//        type = eIITSerial;
        if ([[txtSetting stringValue] length] && [[combSerialPortDevice stringValue] length])
        {
            
            NSString* strRscName = [combSerialPortDevice stringValue];
            if (SimpleCom->CheckInterface(name) >0)
            {
                NSDictionary *dic=[self getDicFromWin];
                [dicConfiguration setValue:dic forKey:name];
                id inface= SimpleCom->getInterface(name);
                id setting_old=[inface GetSimpleComSetting];
                SimpleComSetting *setting=[self CreateSettingbyDic:dic];
                [dic release];
//                SimpleComSetting *setting=[self CreateSetting];
                if (setting==nil) {
                    return;
                }
                [setting SetLogView:[setting_old getLogView]];
                [setting setconnectStatusView:[setting_old getconnectStatusView]];
                [inface SetSimpleComSetting:setting];
                [inface OpenSimpleCom];
                [setting release];
                return;
            }
            NSString *key=SimpleCom->CheckResource(strRscName);
            if (key==nil)
            {
                NSDictionary *dic=[self getDicFromWin];
                [dicConfiguration setValue:dic forKey:name];
                SimpleComSetting *setting=[self CreateSettingbyDic:dic];
                [dic release];
                if (setting==nil) {
                    return;
                }
                [self CreatetabView:name];
                [m_tabview selectTabViewItemWithIdentifier:name];
                NSInteger index= [m_tabview indexOfTabViewItemWithIdentifier:name];
                NSTabViewItem *item=[m_tabview tabViewItemAtIndex:index];;
                SerialPortView* view=[self getSubview:[item view] Identity:@"SerialPortView"];
                [setting SetLogView:view];
                ColorButtonView* colorview=[self getSubview:[item view] Identity:@"ColorButtonView"];
                [setting setconnectStatusView:colorview];
                int ret=SimpleCom->Create(setting);
                [setting release];
                if (ret <= count) {
                    NSString* strMessage = [NSString stringWithFormat:@"Failed to create simplecom %@.", name];
                    [self ShowWarning:strMessage];
                }else
                {
     
                    id inface= SimpleCom->getInterface(name);
                    [inface OpenSimpleCom];
                    
                }
                
            }else{
                NSAlert *alert = [[[NSAlert alloc] init] autorelease];
                [alert setMessageText:[NSString stringWithUTF8String:"Create interface"]];
                [alert setInformativeText:[NSString stringWithFormat:@"The Device is in use(%@)",key]];
                [alert setAlertStyle:NSWarningAlertStyle];
                [alert runModal];
            }
        }
        [self btScanSimpleCom:nil];
        return;
    }
//    else if ([[combSimpleComType stringValue] isEqualToString:INTERFACE_GPIB]) {
////        type = eIITGpib;
//    }
    else if ([[combSimpleComType stringValue] isEqualToString:INTERFACE_LAN]) {
        if ([[txtServerIP stringValue] length] && [txtPort intValue]>99)
        {
            if (SimpleCom->CheckInterface(name) >0)
            {
                    NSDictionary *dic=[self getDicFromWin];
                    [dicConfiguration setValue:dic forKey:name];
                    id inface= SimpleCom->getInterface(name);
                    id setting_old=[inface GetSimpleComSetting];
                    SimpleComSetting *setting=[self CreateSettingbyDic:dic];
                    [dic release];
                    if (setting==nil) {
                        return;
                    }
                    [setting SetLogView:[setting_old getLogView]];
                    [setting setconnectStatusView:[setting_old getconnectStatusView]];
                    [inface SetSimpleComSetting:setting];
                    [inface OpenSimpleCom];
                    [setting release];
                    return;
            }
            NSString* strConfig = [NSString stringWithFormat:@"%@;%@", [txtServerIP stringValue], [txtPort stringValue]];
                NSString *key=SimpleCom->CheckResource(strConfig);
                if (key==nil)
                {
                    NSDictionary *dic=[self getDicFromWin];
                    [dicConfiguration setValue:dic forKey:name];
                    SimpleComSetting *setting=[self CreateSettingbyDic:dic];
                    [dic release];
                    if (setting==nil) {
                        return;
                    }
                    [self CreatetabView:name];
                    [m_tabview selectTabViewItemWithIdentifier:name];
                    NSInteger index= [m_tabview indexOfTabViewItemWithIdentifier:name];
                    NSTabViewItem *item=[m_tabview tabViewItemAtIndex:index];;
                    SerialPortView* view=[self getSubview:[item view] Identity:@"SerialPortView"];
                    [setting SetLogView:view];
                    ColorButtonView* colorview=[self getSubview:[item view] Identity:@"ColorButtonView"];
                    [setting setconnectStatusView:colorview];
                    int ret=SimpleCom->Create(setting);
                    [setting release];
                    if (ret <= count) {
                        NSString* strMessage = [NSString stringWithFormat:@"Failed to create simplecom %@.", name];
                        [self ShowWarning:strMessage];
                    }else
                    {
                        
//                        id inface= SimpleCom->getInterface(name);
//                        [inface OpenSimpleCom];
                        
                    }
                    
                }else{
                    NSAlert *alert = [[[NSAlert alloc] init] autorelease];
                    [alert setMessageText:[NSString stringWithUTF8String:"Create interface"]];
                    [alert setInformativeText:[NSString stringWithFormat:@"The Device is in use(%@)",key]];
                    [alert setAlertStyle:NSWarningAlertStyle];
                    [alert runModal];
                }
            }
        [self btScanSimpleCom:nil];
        return;

        return;
    }
//    else if ([[combSimpleComType stringValue] isEqualToString:INTERFACE_USB]) {
////        type = eIITUsb;
//    }
    [self ShowWarning:@"Specified interface is not supported by current version."];
    return;
}

-(IBAction)btSaveConfig:(id)sender
{
    [dicConfiguration writeToFile:strCfgFile atomically:YES];
}

-(IBAction)btRemoveSimpleCom:(id)sender
{
    NSString* strSimpleComName = [combSimpleCom stringValue];
    SimpleCom->Remove((char*)[strSimpleComName UTF8String]);
    [dicConfiguration setValue:nil forKey:strSimpleComName];
    [self RemoveTabViewWithName:strSimpleComName];
    [self btScanSimpleCom:nil];
    [self btSaveConfig:nil];
}

-(IBAction)btRemoveAllSimpleComs:(id)sender
{
    SimpleCom->RemoveAll();
    for (NSString* key in dicConfiguration) {
        [dicConfiguration setValue:nil forKey:key];
    }
    [self RemoveAlltabView];
    [self btScanSimpleCom:nil];
    [self btSaveConfig:nil];
}

-(IBAction)btSetDebugOut:(id)sender
{
    BOOL State = [(NSButton*)sender state];
    [SimpleComSetting setDebugOut:State];
}

-(IBAction)btSetCheckBox:(id)sender
{
    int Tag = (int)[sender tag];
    BOOL State = (BOOL)[sender state];
    switch (Tag)
    {
        case 1://isSaveLog
            if (!State)
            {
                [btIsSendLogtoPDCA setState:NO];
            }
            break;
        case 2://isPuddingPDCA
            if (State)
            {
                [btIsSaveLog setState:YES];
            }
            break;
            
        default:
            break;
    }
}


#pragma mark--handle notifiction
-(void)handleNotification:(NSNotification*)nf
{
    NSDictionary *userInfo = [nf userInfo] ;
    if ([nf.name isEqualToString:kNotificationAttachMenu])
    {
        NSMenu * instrMenu = [userInfo objectForKey:@"menus"];
        [instrMenu addItem:menuItem];
    }
    return ;
}

#pragma mark--FCT MenuItem action function
-(IBAction)menu_ShowDebugPanel:(id)sender
{
    [mWinDebugPanel center];
    [mWinDebugPanel makeKeyAndOrderFront:sender];
}

-(IBAction)menu_ShowConfigPanel:(id)sender
{
    [mWinConfigPanel center];
    [mWinConfigPanel makeKeyAndOrderFront:sender];
}
-(IBAction)bt_IsSendLogtoPDCAClick:(id)sender
{
    if ([btIsSendLogtoPDCA state]>0) {
        [btIsSaveLog setState:[btIsSendLogtoPDCA state]];
    }
}
-(IBAction)bt_IsFixtrueClick:(id)sender;
{
    if ([sender state]==0) {
        [btIsStartCallLua setEnabled:NO];
        [btIsStopCallLua setEnabled:NO];
        [btIsScanCallLua setEnabled:NO];
        
        [txtStart setEnabled:NO];
        [txtStop setEnabled:NO];
        [txtScan setEnabled:NO];
        [txtOther setEnabled:NO];
    }else
    {
        [btIsStartCallLua setEnabled:YES];
        [btIsStopCallLua setEnabled:YES];
        [btIsScanCallLua setEnabled:YES];
        [txtStart setEnabled:YES];
        [txtStop setEnabled:YES];
        [txtScan setEnabled:YES];
        [txtOther setEnabled:YES];
    }
}
- (void)comboBoxSelectionDidChange:(NSNotification *)notification
{
    id sender=[notification object];
    if ([[sender identifier] isEqualToString:@"InterfaceType"]) {
        [self OnChangeInterfaceType:sender];
    }else if ([[sender identifier] isEqualToString:@"InterfaceName"])
    {
        [self OnChangeInterfaceName:sender];
    }
}

-(IBAction)OnChangeInterfaceType:(id)sender
{
    if ([[sender objectValueOfSelectedItem] isEqualToString:INTERFACE_SERIAL])
    {
        [combSerialPortDevice setHidden:NO];
        [txtSettingLable setHidden:NO];
        [txtSetting setHidden:NO];
        [txtSerail_log setHidden:NO];
        [btScanSerialport setHidden:NO];
        
        [txtServerIPLable setHidden:YES];
        [txtServerIP setHidden:YES];
        [txtPortLable setHidden:YES];
        [txtPort setHidden:YES];
        
        
    }else if ([[sender objectValueOfSelectedItem] isEqualToString:INTERFACE_LAN])
    {
        [combSerialPortDevice setHidden:YES];
        [txtSettingLable setHidden:YES];
        [txtSetting setHidden:YES];
        [txtSerail_log setHidden:YES];
        [btScanSerialport setHidden:YES];
        
        [txtServerIPLable setHidden:NO];
        [txtServerIP setHidden:NO];
        [txtPortLable setHidden:NO];
        [txtPort setHidden:NO];
    }
}
-(IBAction)btn_SearchDUTDevice:(id)sender
{
    [combSerialPortDevice removeAllItems] ;
    
    NSEnumerator *enumerator = [AMSerialPortList portEnumerator];
	AMSerialPort *aPort;
	while (aPort = [enumerator nextObject]) {
		// print port name
        [combSerialPortDevice addItemWithObjectValue:[aPort bsdPath]];
	}
    
    return ;
}

-(void)ShowWarning:(NSString*)msg
{
    NSRunAlertPanel(@"SimpleCom manager warning", @"%@", @"OK", nil, nil, msg);
}

-(void)logOut:(NSString*)msg
{
    DebugMsgCounter++;
    if (DebugMsgCounter > 10000) {
        [mLog setString:@""];
        DebugMsgCounter = 0;
    }
    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
    [dateFormatter setDateFormat:@"yyyy/MM/dd HH:mm:ss.SSS : "];
    int length = 0;
    NSAttributedString *theString;
    NSRange theRange;
    
    NSString * str = [NSString stringWithFormat:@"%@ %@\r",[dateFormatter stringFromDate:[NSDate date]],msg];
    theString = [[NSAttributedString alloc] initWithString:str];
    [[mLog textStorage] appendAttributedString: theString];
    length = (int)[[mLog textStorage] length];
    theRange = NSMakeRange(length, 0);
    [mLog scrollRangeToVisible:theRange];
    [dateFormatter release];
    [theString release];
    [mLog setNeedsDisplay:YES];
}

-(void)PrintLog:(NSString*)strMsg
{
    [self performSelectorOnMainThread:@selector(logOut:) withObject:strMsg waitUntilDone:YES];
}

#pragma mark--UI Action function

@end
