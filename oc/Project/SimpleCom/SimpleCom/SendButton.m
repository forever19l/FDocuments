//
//  SendButton.m
//  SimpleCom
//
//  Created by ZL-Pro on 16/8/17.
//  Copyright (c) 2016年 ZL-Pro. All rights reserved.
//

#import "SendButton.h"

@implementation SendButton

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
    }
    return self;
}
- (id)delegate
{
	return delegate;
}
- (void)setDelegate:(id)newDelegate
{
	id old = nil;
	if (newDelegate != delegate) {
		old = delegate;
		delegate = [newDelegate retain];
		[old release];
	}
}
- (void)mouseDown:(NSEvent *)theEvent
{
    if ([super isEnabled])
    {
        if (delegate!=nil)
        {
            if([delegate respondsToSelector:@selector(IAMotorButtonmouseDown:Event:)])
            {
                [delegate IAMotorButtonmouseDown:self Event:theEvent];
            }
        }
    }
    [super mouseDown:theEvent];
    if ([super isEnabled])
    {
        if (delegate!=nil)
        {
            if([delegate respondsToSelector:@selector(IAMotorButtonmouseUp:Event:)])
            {
                [delegate IAMotorButtonmouseUp:self Event:theEvent];
            }
        }
    }
    
}
- (void)drawRect:(NSRect)dirtyRect
{
    [super drawRect:dirtyRect];
    
    // Drawing code here.
}

@end
