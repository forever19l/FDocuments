//
//  TMLibrary.h
//  TMLibrary
//
//  Created by Ryan on 5/18/15.
//  Copyright (c) 2015 ___Intelligent Automation___. All rights reserved.
//

#import <Cocoa/Cocoa.h>

//! Project version number for TMLibrary.
FOUNDATION_EXPORT double TMLibraryVersionNumber;

//! Project version string for TMLibrary.
FOUNDATION_EXPORT const unsigned char TMLibraryVersionString[];

// In this header, you should import all the public headers of your framework using statements like #import <TMLibrary/PublicHeader.h>


int tmPrintf(const char * fmt,...);
int tmError(const char * fmt,...);
int tmGetCurrentPath(char * path);
