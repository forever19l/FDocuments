//
//  IAFeelingData.h
//  IANetFixture
//
//  Created by ZL-Pro on 16/6/26.
//  Copyright © 2016年 Louis. All rights reserved.
//
/*CSV存储类*/

#import <Foundation/Foundation.h>

@interface IAFeelingData : NSObject
{
    
}
+(BOOL)SavePLCDatatoFile:(void*)PLCdata
                LineNumber:(const char*)LineNumber
             StationNumber:(const char*)StationNumber
              SerialNumber:(const char*)mlbSN
                  Platfrom:(const char*)Platfrom
                   KeyName:(const char*)KeyName
                  Filename:(const char*)file;

+(BOOL)SaveLocalDatatoFile:(void*)loadcelldata
                LineNumber:(const char*)LineNumber
             StationNumber:(const char*)StationNumber
              SerialNumber:(const char*)mlbSN
                  Platfrom:(const char*)Platfrom
                   KeyName:(const char*)KeyName
                  Filename:(const char*)file;
+(int)ReadLocalDatatoFile:(void*)loadcelldata
               LineNumber:(char*)LineNumber
            StationNumber:(char*)StationNumber
             SerialNumber:(char*)mlbSN
                 Platfrom:(char*)Platfrom
                  KeyName:(char*)KeyName
                 Filename:(const char*)file;
@end
