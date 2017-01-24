//
//  PathManager.h
//  CoreLib
//
//  Created by Manuel Petit on 5/14/14.
//
//

#import <Foundation/Foundation.h>

static NSString *bundlesPath;
static NSString *configurationPath;

@interface PathManager : NSObject

+ (PathManager *)sharedManager;

- (NSString *)logPath;
- (NSString *)blobPath;
- (NSString *)configPath;

@end
