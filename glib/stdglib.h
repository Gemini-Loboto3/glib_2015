#pragma once
#define _CRT_NON_CONFORMING_SWPRINTFS
#define _CRT_SECURE_NO_DEPRECATE

//#include "forcelib.h"
#include <windows.h>
#include <tchar.h>

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

#ifndef WINVER
#define WINVER 0x0501
#endif

#ifndef _WIN32_WINNT                
#define _WIN32_WINNT 0x0501
#endif						

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0410
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0600
#endif
