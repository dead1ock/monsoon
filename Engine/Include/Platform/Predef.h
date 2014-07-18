/**
* Copyright (c) 2014 Sonora Games
*
*/

#ifndef _MONSOON_PREDEF_H_
#define _MONSOON_PREDEF_H_

#include <boost/predef/architecture.h>
#include <boost/predef/compiler.h>
#include <boost/predef/os.h>
#include <boost/predef/other.h>

//
// OS
//
#ifdef BOOST_OS_WINDOWS
	#define  MONSOON_OS_WINDOWS
#elif BOOST_OS_LINUX
	#define MONSOON_OS_LINUX
#endif

//
// Endianness
//
#ifdef BOOST_ENDIAN_BIG_BYTE
	#define MONSOON_ENDIAN_BIG
#elif BOOST_ENDIAN_LITTLE_BYTE
	#define MONSOON_ENDINA_LITTLE
#endif

//
// 64-bit Detection
//
#ifdef BOOST_ARCH_X86_64
	#define MONSOON_64
#endif

#endif // _MONSOON_PLATFORM_PREDEF_H_