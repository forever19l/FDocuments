//
//  InterfaceNameView.m
//  SimpleCom
//
//  Created by ZL-Pro on 16/8/27.
//  Copyright (c) 2016年 ZL-Pro. All rights reserved.
//

#import "InterfaceNameView.h"

@implementation InterfaceNameView

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
    }
    return self;
}
- (void)keyUp:(NSEvent *)theEvent
{
    [super keyUp:theEvent];
    if ([self delegate]!=nil)
    {
        if([[self delegate] respondsToSelector:@selector(InterfaceNameViewKeyUp:Event:)])
        {
            [[self delegate] InterfaceNameViewKeyUp:self Event:theEvent];
        }
    }
}
- (void)drawRect:(NSRect)dirtyRect
{
    [super drawRect:dirtyRect];
    
    // Drawing code here.
}

@end
