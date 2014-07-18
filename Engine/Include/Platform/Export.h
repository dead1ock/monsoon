/**
* Copyright (c) 2014 Sonora Games
*
*/

#ifndef _MONSOON_EXPORT_H_
#define _MONSOON_EXPORT_H_

#include "predef.h"


#ifdef MONSOON_DYNLIB_EXPORT
#define DYNLIB __declspec(dllexport)
#else
#define DYNLIB __declspec(dllimport)
#endif

#define DRIVER DYNLIB

#endif // _MONSOON_EXPORT_H_