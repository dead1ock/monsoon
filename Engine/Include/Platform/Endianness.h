/**
* Copyright (c) 2014 Sonora Games
*
*/

#ifndef _MONSOON_ENDIANNESS_H_
#define _MONSOON_ENDIANNESS_H_

#include <algorithm>

namespace monsoon {
	namespace platform {
		namespace endianness {
			/*
			* Swaps bytes in memory from start to T, recursively.
			*/
			template<size_t T>
			inline void swap(char* start) {
				std::swap(*start, *(start + T - 1));
				EndianSwap<T - 2>(start + 1);
				
			}

			// Do nothing on 0 and 1 sizes...
			template<> inline void swap<0>(char*) { }
			template<> inline void swap<1>(char*) { }

			/*
			* Apply endian swap for type T.
			*/
			template<typename T> inline void applySwap(T* value) {
				swap<sizeof(T)>((char *)(value));
			}

		} // namespace Endianness

#ifdef MONSOON_ENDIANNESS_BIG
		template<typename T> inline void endianSwap(T& value) { Endianness::applySwap(value); }
#else
		template<typename T> inline void endianSwap(T& value) { }
#endif // MONSOON_ENDIANNESS_BIG

	} // namespace Foundation
} // namespace Monsoon

#endif // _MONSOON_ENDIANNESS_H_