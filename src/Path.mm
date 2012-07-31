//
//  Path.mm
//  Climber
//
//  Created by Arthur Fox on 25/07/2012.
//  Copyright (c) 2012 OMA. All rights reserved.
//

//Include this in the OS X version and exclude it in Windows

#include "Path.h"

#include <Foundation/Foundation.h>
#include <ApplicationServices/ApplicationServices.h>

std::string Path::pathForFile(const std::string &filename) 
{
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    
    NSString* objcFilename = [NSString stringWithCString:filename.c_str() encoding:NSUTF8StringEncoding];
    NSString* path = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:objcFilename];
    
    std::string finalPath = [path cStringUsingEncoding:NSUTF8StringEncoding];
    
    [pool release];
    return finalPath;
}