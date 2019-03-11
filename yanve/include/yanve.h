// header.h : include file for standard system include files,
// or project specific include files
//

#pragma once



#include "platform/targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

#include <common.h>

// reference additional headers your program requires here
namespace yanve
{
YANVE_API void print();
}
