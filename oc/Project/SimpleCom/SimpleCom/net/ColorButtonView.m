//
//  ColorButtonView.m
//  TigerV
//
//  Created by Apple on 14-10-28.
//  Copyright (c) 2014年 zl. All rights reserved.
//

#import "ColorButtonView.h"

@implementation ColorButtonView
- (instancetype)init
{
    self = [super init];
    if (self) {
        
    }
    return self;
}
- (instancetype)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        
    }
    return self;
}
-(void)SocketStatus:(NSNumber*)number
{
    if ([number boolValue]) {
        [self setState:1];
    }else
    {
        [self setState:0];
    }
}
- (void)drawRect:(NSRect)dirtyRect
{
    NSColor *startColor;
    NSColor *endColor;
    if ([self state])
    {
        startColor=[NSColor colorWithCalibratedRed:0.4 green:1 blue:0.4 alpha:1];
        endColor= [NSColor colorWithCalibratedRed:0 green:0.4 blue:0 alpha:1];
    }else
    {
        startColor=[NSColor colorWithCalibratedRed:0.9 green:1 blue:1 alpha:1];
        endColor= [NSColor colorWithCalibratedRed:0.6 green:0.6 blue:0.6 alpha:1];
    }
    NSColor *colors[2] = {startColor,endColor};
    CGFloat components[2*4];
    for (int i = 0; i < 2; i++) {
        CGColorRef tmpcolorRef = colors[i].CGColor;
        const CGFloat *tmpcomponents = CGColorGetComponents(tmpcolorRef);
        for (int j = 0; j < 4; j++) {
            components[i * 4 + j] = tmpcomponents[j];
        }
    }

    CGColorSpaceRef space = CGColorSpaceCreateDeviceRGB();
    CGGradientRef gradient = CGGradientCreateWithColorComponents(space, components, NULL,2);
    CGColorSpaceRelease(space),space=NULL;//release
    
    CGPoint start;
    start.x=dirtyRect.size.height/2;
    start.y=dirtyRect.size.height/2;
    CGPoint end;
    end.x=dirtyRect.size.height/2;
    end.y=dirtyRect.size.height/2;
    CGFloat startRadius = 0.0f;
    CGFloat endRadius = dirtyRect.size.height/2;
    CGContextRef context = (CGContextRef)[[NSGraphicsContext currentContext] graphicsPort];
    CGContextDrawRadialGradient(context, gradient, start, startRadius, end, endRadius, 0);
    CGGradientRelease(gradient);
    gradient=NULL;
}

-(void)awakeFromNib
{
    //ConnectState=NO;
}

-(void)UpdateColorViewOnMainThread
{
    [self setNeedsDisplay:YES];
}

-(void)UpdateColorView
{
    [self performSelectorOnMainThread:@selector(UpdateColorViewOnMainThread) withObject:nil waitUntilDone:YES];
}

-(void)SetButtonColor:(BOOL)State
{
    //ConnectState=State;
    [self UpdateColorView];
}

@end
