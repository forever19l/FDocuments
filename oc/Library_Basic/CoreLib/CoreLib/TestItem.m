//
//  TestItem.m
//  iTMP
//
//  Created by Ryan on 12-5-20.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#import "TestItem.h"

@implementation TestItem
//@synthesize index,description,group,lower,upper,uut1_value,uut2_value,uut3_value,uut4_value,unit,remark,time;
@synthesize state1,state2,state3,state4,state5,state6,state7,state8,image;
//@synthesize uut5_value,uut6_value,uut7_value,uut8_value ;
@synthesize waiver;
//@synthesize testkey;

- (id)init
{
    self = [super init];
    if (self) {
		self.state1 = 1;
		self.state2 = 1;
		self.state3 = 1;
		self.state4 = 1;
		self.state5 = 1;
		self.state6 = 1;
		self.state7 = 1;
		self.state8 = 1;
		self.time = @"";
        self.time1 = @"";

    }
    return self;
}

- (void)dealloc
{
    if (testkey!=nil)
    {
        [testkey release];
    }

    if (index!=nil)
    {
        [index release];
    }

    if (description!=nil)
    {
        [description release];
    }

    if (group!=nil)
    {
        [group release];
    }

    if (lower!=nil)
    {
        [lower release];
    }

    if (upper!=nil)
    {
        [upper release];
    }

    if (uut1_value!=nil)
    {
        [uut1_value release];
    }

    if (uut2_value!=nil)
    {
        [uut2_value release];
    }

    if (uut3_value!=nil)
    {
        [uut3_value release];
    }

    if (uut4_value!=nil)
    {
        [uut4_value release];
    }

    if (uut5_value!=nil)
    {
        [uut5_value release];
    }

    if (uut6_value!=nil)
    {
        [uut6_value release];
    }

    if (uut7_value!=nil)
    {
        [uut7_value release];
    }

    if (uut8_value!=nil)
    {
        [uut8_value release];
    }

    if (unit!=nil)
    {
        [unit release];
    }

    if (remark!=nil)
    {
        [remark release];
    }

    if (time!=nil)
    {
        [time release];
    }
    if (time1!=nil)
    {
        [time1 release];
    }
    [super dealloc];
}


- (id)initWithName:(NSString *)aName
{
    return [self initWithName:aName withSeparator:@"\t"];
}

-(id)initWithName:(NSString *)name withSeparator:(NSString *)sep
{
    self = [self init];

    if (!self) return nil;

    name = [name stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
    NSArray * arr = [name componentsSeparatedByString:sep];

    if ([arr count] <2) {
		[self dealloc];
		return nil;
	}

    self.index = [arr objectAtIndex:0];
    self.description = [arr objectAtIndex:1];
    if ([arr count]>2) {
        self.lower = [arr objectAtIndex:2];     //lower limited
    }
    if ([arr count]>3)
    {
        self.upper = [arr objectAtIndex:3];     //upper limited
    }
    if([arr count]>4)
    {
        self.unit = [arr objectAtIndex:4];      //unit
    }
    if ([arr count]>5)
    {
        self.remark = [arr objectAtIndex:5];    //remark
    }
    self.state1 = 1;
    self.state2 = 1;
    self.state3 = 1;
    self.state4 = 1;
    self.state5 = 1;
    self.state6 = 1;
    self.state7 = 1;
    self.state8 = 1;
    self.time = @"";
    self.time1 = @"";
    
    self.testkey = @"";
    self.waiver = NO;

    return self;
}


+ (TestItem *)nodeDataWithName:(NSString *)name {
    return [[[TestItem alloc] initWithName:name] autorelease];
}

-(NSString*)index
{
    return index;
}

-(NSString*)description
{
    return description;
}

-(NSString*)group
{
    return group;
}

-(NSString*)lower
{
    return lower;
}

-(NSString*)upper
{
    return upper;
}

-(NSString*)uut1_value
{
    return uut1_value;
}

-(NSString*)uut2_value
{
    return uut2_value;
}

-(NSString*)uut3_value
{
    return uut3_value;
}

-(NSString*)uut4_value
{
    return uut4_value;
}

-(NSString*)uut5_value
{
    return uut5_value;
}

-(NSString*)uut6_value
{
    return uut6_value;
}

-(NSString*)uut7_value
{
    return uut7_value;
}

-(NSString*)uut8_value
{
    return uut8_value;
}

-(NSString*)unit
{
    return unit;
}

-(NSString*)remark
{
    return remark;
}

-(NSString*)time
{
    return time;
}
-(NSString*)time1
{
    return time1;
}

-(NSString *)testkey
{
    return testkey;
}

-(void)setTestkey:(NSString*)str
{
    if (testkey!=nil)
    {
        [testkey release];
    }
    testkey = str==nil?nil:[[NSString alloc] initWithString:str];
}

-(void)setIndex:(NSString*)str
{
    if (index!=nil)
    {
        [index release];
    }
    index = str==nil?nil:[[NSString alloc] initWithString:str];
}

-(void)setDescription:(NSString*)str
{
    if (description!=nil)
    {
        [description release];
    }
    description = str==nil?nil:[[NSString alloc]initWithString:str];
}

-(void)setGroup:(NSString*)str
{
    if (group!=nil)
    {
        [group release];
    }
    group = str==nil?nil:[[NSString alloc] initWithString:str];
}

-(void)setLower:(NSString*)str
{
    if (lower!=nil)
    {
        [lower release];
    }
    lower = str==nil?nil:[[NSString alloc] initWithString:str];
}

-(void)setUpper:(NSString*)str
{
    if (upper!=nil)
    {
        [upper release];
    }
    upper = str==nil?nil:[[NSString alloc] initWithString:str];
}

-(void)setUut1_value:(NSString*)str
{
    if (uut1_value!=nil)
    {
        [uut1_value release];
    }
    uut1_value = str==nil?nil:[[NSString alloc] initWithString:str];
}

-(void)setUut2_value:(NSString*)str
{
    if (uut2_value!=nil)
    {
        [uut2_value release];
    }
    uut2_value = str==nil?nil:[[NSString alloc] initWithString:str];
}

-(void)setUut3_value:(NSString*)str
{
    if (uut3_value!=nil)
    {
        [uut3_value release];
    }
    uut3_value = str==nil?nil:[[NSString alloc] initWithString:str];
}

-(void)setUut4_value:(NSString*)str
{
    if (uut4_value!=nil)
    {
        [uut4_value release];
    }
    uut4_value = str==nil?nil:[[NSString alloc] initWithString:str];
}

-(void)setUut5_value:(NSString*)str
{
    if (uut5_value!=nil)
    {
        [uut5_value release];
    }
    uut5_value = str==nil?nil:[[NSString alloc] initWithString:str];
}

-(void)setUut6_value:(NSString*)str
{
    if (uut6_value!=nil)
    {
        [uut6_value release];
    }
    uut6_value = str==nil?nil:[[NSString alloc] initWithString:str];
}

-(void)setUut7_value:(NSString*)str
{
    if (uut7_value!=nil)
    {
        [uut7_value release];
    }
    uut7_value = str==nil?nil:[[NSString alloc] initWithString:str];
}

-(void)setUut8_value:(NSString*)str
{
    if (uut8_value!=nil)
    {
        [uut8_value release];
    }
    uut8_value = str==nil?nil:[[NSString alloc] initWithString:str];
}

-(void)setUnit:(NSString*)str
{
    if (unit!=nil)
    {
        [unit release];
    }
    unit = str==nil?nil:[[NSString alloc] initWithString:str];
}

-(void)setRemark:(NSString*)str
{
    if (remark!=nil)
    {
        [remark release];
    }
    remark = str==nil?nil:[[NSString alloc] initWithString:str];
}

-(void)setTime:(NSString*)str
{
    if (time!=nil)
    {
        [time release];
    }
    time = str==nil?nil:[[NSString alloc] initWithString:str];
}
-(void)setTime1:(NSString*)str
{
    if (time1!=nil)
    {
        [time1 release];
    }
    time1 = str==nil?nil:[[NSString alloc] initWithString:str];
}
@end

@implementation KeyItem
//@synthesize name;
- (id)init
{
    self = [super init];
    if (self) {
    }
    
    return self;
}

- (void)dealloc
{
    if (name!=nil)
    {
        [name release];
    }
    [super dealloc];
}


- (id)initWithName:(NSString *)aName {
    self = [self init];
    self.name = aName;
    return self;
}


+ (KeyItem *)nodeDataWithName:(NSString *)name {
    return [[[KeyItem alloc] initWithName:name] autorelease];
}

-(NSString*)name
{
    return name;
}

-(void)setName:(NSString*)str
{
    if (name!=nil)
    {
        [name release];
    }
    name = str==nil?nil:[[NSString alloc] initWithString:str];
}

@end
