/**
 * Copyright (c) 2014-2015 Dallin Wellington
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

	#define MONSOON_MAX_ENTITIES 65536
	 typedef U32 Entity;

#ifdef MONSOON_64
	 typedef U64 MEM_ADDRESS;
#else
	 typedef U32 MEM_ADDRESS;
#endif // MONSOON_64

	 typedef float Float32;
	 typedef float Float64;

#ifdef MONSOON_DOUBLE_PRECISION
	 typedef double Float;
#else
	 typedef float Float;
#endif

	#define PI 3.14159265

 } // namespace Monsoon

#endif // _MONSON_TYPES_H_