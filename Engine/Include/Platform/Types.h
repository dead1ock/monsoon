/**
 * Copyright (c) 2014 Sonora Games
 *
 */

#ifndef _MONSOON_TYPES_H_
#define _MONSOON_TYPES_H_

#include <string>
#include <memory>

 namespace Monsoon
 {
	 //
	 // Atomic Types
	 //
	 typedef signed char I8;
	 typedef unsigned char U8;

	 typedef signed short I16;
	 typedef unsigned short U16;

	 typedef unsigned int U32;
	 typedef signed int I32;

	 typedef unsigned long long U64;
	 typedef signed long long I64;

#ifdef MONSOON_64
	 typedef U64 MEM_ADDRESS;
#else
	 typedef U32 MEM_ADDRESS;
#endif // MONSOON_64

 } // namespace Monsoon

#endif // _MONSON_TYPES_H_