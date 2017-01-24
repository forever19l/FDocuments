//
//  PathManager.h
//  TMLibrary
//
//  Created by Ryan on 6/8/15.
//  Copyright (c) 2015 ___Intelligent Automation___. All rights reserved.
//

#ifndef __TMLibrary__PathManager__
#define __TMLibrary__PathManager__

#include <stdio.h>

class CPathManager {
public:
    CPathManager();
    CPathManager(char * module);
    virtual ~CPathManager();
    
public:
    const char * vaultPath();
    const char * mainPath();
    const char * logPath();
    const char * blobPath();
    const char * profilePath();
    const char * configPath();
    const char * scriptPath();
    const char * driverPath();
    
    
public:
    static CPathManager * sharedManager();
    
protected:
    char m_MainPath[128];
    char m_ModuleName[128];
    char m_ValutPath[128];
};

#endif /* defined(__TMLibrary__PathManager__) */
