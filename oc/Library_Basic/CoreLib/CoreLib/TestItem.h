//
//  TestItem.h
//  iTMP
//
//  Created by Ryan on 12-5-20.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface TestItem : NSObject {
@private
    NSString * index;
    NSString * description;
	NSString * group;
    NSString * lower;
    NSString * upper;
    NSString * uut1_value;
    NSString * uut2_value;
    NSString * uut3_value;
    NSString * uut4_value;
    NSString * uut5_value;
    NSString * uut6_value;
    NSString * uut7_value;
    NSString * uut8_value;
    
    NSString * unit;
    NSString * remark;
    NSString * time;
    NSString * time1;
    int state1;
    int state2;
    int state3;
    int state4;
    int state5;
    int state6;
    int state7;
    int state8;
    
    NSImage * image;

    NSString * testkey;
    BOOL waiver;    
}

- (id)initWithName:(NSString *)name;
-(id)initWithName:(NSString *)name withSeparator:(NSString *)sep;
//+ (TestItem *)nodeDataWithName:(NSString *)name;

-(NSString*)index;
-(NSString*)description;
-(NSString*)group;
-(NSString*)lower;
-(NSString*)upper;
-(NSString*)uut1_value;
-(NSString*)uut2_value;
-(NSString*)uut3_value;
-(NSString*)uut4_value;
-(NSString*)uut5_value;
-(NSString*)uut6_value;
-(NSString*)uut7_value;
-(NSString*)uut8_value;
-(NSString*)unit;
-(NSString*)remark;
-(NSString*)time;
-(NSString*)time1;
-(NSString*)testkey;

-(void)setTestkey:(NSString*)str;
-(void)setIndex:(NSString*)str;
-(void)setDescription:(NSString*)str;
-(void)setGroup:(NSString*)str;
-(void)setLower:(NSString*)str;
-(void)setUpper:(NSString*)str;
-(void)setUut1_value:(NSString*)str;
-(void)setUut2_value:(NSString*)str;
-(void)setUut3_value:(NSString*)str;
-(void)setUut4_value:(NSString*)str;
-(void)setUut5_value:(NSString*)str;
-(void)setUut6_value:(NSString*)str;
-(void)setUut7_value:(NSString*)str;
-(void)setUut8_value:(NSString*)str;
-(void)setUnit:(NSString*)str;
-(void)setRemark:(NSString*)str;
-(void)setTime:(NSString*)str;
-(void)setTime1:(NSString*)str;

@property(readwrite,assign)int state1,state2,state3,state4,state5,state6,state7,state8;
@property(readwrite,retain)NSImage * image;



@property(readwrite)        BOOL waiver;

//@property(readwrite, retain) NSString * description;
@end

@interface KeyItem : NSObject {
@private
    NSString * name;
}
- (id)initWithName:(NSString *)name;
+ (KeyItem *)nodeDataWithName:(NSString *)name;

-(NSString*)name;

-(void)setName:(NSString*)str;

//@property(readwrite, copy) NSString *name;
@end
