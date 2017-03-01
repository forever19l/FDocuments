//
//  ColorButtonView.h
//  TigerV
//
//  Created by Apple on 14-10-28.
//  Copyright (c) 2014年 zl. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface ColorButtonView : NSButton
{
    NSImage *Green_Image;
    NSImage *Gray_Image;
    
    BOOL ConnectState;
}
- (void)drawRect:(NSRect)dirtyRect;
-(void)SetButtonColor:(BOOL)State;
//-(void)SocketStatus:(NSNumber*)number;
@end
