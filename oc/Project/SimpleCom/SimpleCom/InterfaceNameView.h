//
//  InterfaceNameView.h
//  SimpleCom
//
//  Created by ZL-Pro on 16/8/27.
//  Copyright (c) 2016年 ZL-Pro. All rights reserved.
//

#import <Cocoa/Cocoa.h>
@protocol InterfaceNameViewDelegate <NSObject>
@optional
- (void)InterfaceNameViewKeyUp:(NSControl *)control Event:(NSEvent *)theEvent;
@end
@interface InterfaceNameView : NSTextField

@end
