/**
* Copyright (c) 2014 Sonora Games
*
*/

#ifndef _MONSOON_DYNLIB_H_
#define _MONSOON_DYNLIB_H_

#include <boost/noncopyable.hpp>

#include "export.h"

#ifdef MONSOON_OS_WINDOWS
	#include <Windows.h>
#elif MONSOON_OS_LINUX
#endif

namespace Monsoon {
	namespace Platform {
		
		/**
		* Manages the loading, freeing, and symbol lookup of dynamic libraries across multiple
		* platforms.
		*/
		class DynLib : public boost::noncopyable
		{
		public:
			DynLib() {

			}

			~DynLib() {

			}

			bool Load(const char* name) {
				#ifdef MONSOON_OS_WINDOWS

					mDLLHandle = LoadLibrary(name);
					if(!mDLLHandle)
						return false;

				#elif MONSOON_OS_LINUX

				#endif

				return true;
			}

			template<typename T>
			T GetProcAddress(const char* name) {
				#ifdef MONSOON_OS_WINDOWS
				return (T) GetProcAddress(mDLLHandle, name);
				#elif MONSOON_OS_LINUX
				#endif
			}

			void Free() {
				#ifdef MONSOON_OS_WINDOWS
				FreeLibrary(mDLLHandle);
				#elif MONSOON_OS_LINUX
				#endif
			}

		private:

#ifdef MONSOON_OS_WINDOWS
			HMODULE mDLLHandle;
#elif MONSOON_OS_LINUX
			void* mDynObjHandle;
#endif
			
		};

	} // namespace platform
} // namespace monsoon

#endif // _MONSOON_PLATFORM_DYNLIB_H_