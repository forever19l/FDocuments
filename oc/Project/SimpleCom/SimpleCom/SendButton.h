//
//  SendButton.h
//  SimpleCom
//
//  Created by ZL-Pro on 16/8/17.
//  Copyright (c) 2016年 ZL-Pro. All rights reserved.
//

#import <Cocoa/Cocoa.h>
@protocol IAMotorButtonDelegate <NSObject>
@optional
- (void)IAMotorButtonmouseUp:(NSControl *)control Event:(NSEvent *)theEvent;
- (void)IAMotorButtonmouseDown:(NSControl *)control Event:(NSEvent *)theEvent;
@end

@interface SendButton : NSButton
{
@private
    id delegate;
}
- (id)delegate;
- (void)setDelegate:(id)newDelegate;
@end
