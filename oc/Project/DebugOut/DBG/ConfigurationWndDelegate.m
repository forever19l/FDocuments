//
//  ConfigurationWndDelegate.m
//  DUT
//
//  Created by Ryan on 11/12/15.
//  Copyright © 2015 ___Intelligent Automation___. All rights reserved.
//

#import "ConfigurationWndDelegate.h"

@implementation ConfigurationWndDelegate

-(void)awakeFromNib
{
}

-(IBAction)btOK:(id)sender
{
    for (id key in [dicTableView keyEnumerator])
    {
        [dicConfiguration setValue:[dicTableView valueForKey:key] forKey:key];
    }
    [winMain endSheet:winConfiguration returnCode:NSModalResponseOK];
}

-(IBAction)btCancel:(id)sender
{
     [winMain endSheet:winConfiguration returnCode:NSModalResponseCancel];
}

-(void)InitCtrls:(NSMutableDictionary *)dic withSolts:(int)number
{
    dicConfiguration = dic;
    
    if (dicTableView) {
        [dicTableView release];
    }
    dicTableView = [[NSMutableDictionary alloc] initWithDictionary:dic copyItems:YES];
    m_Solts = number;
    [tableView reloadData];
}

#pragma table view datasource

- (NSInteger)numberOfRowsInTableView:(NSTableView *)tableView;
{
    return m_Solts;
}

/* This method is required for the "Cell Based" TableView, and is optional for the "View Based" TableView. If implemented in the latter case, the value will be set to the view at a given row/column if the view responds to -setObjectValue: (such as NSControl and NSTableCellView).
 */
- (nullable id)tableView:(NSTableView *)tableView objectValueForTableColumn:(nullable NSTableColumn *)tableColumn row:(NSInteger)row
{
    if ([[tableColumn identifier] isEqualToString:@"index"])
    {
        return [NSNumber numberWithLong:row];
    }
    else
    {
        id v = [dicTableView valueForKey:[NSString stringWithFormat:@"%@%ld",[tableColumn identifier],row]];
        if (!v) {
            v=@"None";
        }
        return v;
    }
}

/* NOTE: This method is not called for the View Based TableView.
 */
- (void)tableView:(NSTableView *)tableView setObjectValue:(nullable id)object forTableColumn:(nullable NSTableColumn *)tableColumn row:(NSInteger)row
{
    if (![[tableColumn identifier] isEqualToString:@"index"])
    {
        [dicTableView setObject:object forKey:[NSString stringWithFormat:@"%@%ld",[tableColumn identifier],row]];
    }
}

@end
