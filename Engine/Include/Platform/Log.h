/**
* Copyright (c) 2014 Sonora Games
*
*/

#ifndef _MONSOON_LOG_H_
#define _MONSOON_LOG_H_

#include <fstream>
#include <stdio.h>
#include <io.h>

#include "Platform/Platform.h"

#ifdef MONSOON_OS_WINDOWS
#include <fcntl.h>
#include <Windows.h>
#endif

namespace Monsoon {
	namespace Platform {

		/**
		* 
		*/
		class DYNLIB Log
		{
		public:
			Log(const char* filename = "log.txt");
			virtual ~Log();

			void Debug(const char* message);
			void Fatal(const char* message);
			void Info(const char* message);

		private:
			std::fstream mLogFile;

#ifdef MONSOON_OS_WINDOWS
			HANDLE mConsoleHandle;
			FILE* mConsoleFileHandle;

#endif 
		};
	}
}
#endif // _MONSOON_LOG_H_