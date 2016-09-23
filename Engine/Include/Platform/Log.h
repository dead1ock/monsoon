/**
* Copyright (c) 2014-2016 Dallin Wellington
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

		enum VERBOSITY {
			CRITICAL = 0,
			WARNING,
			INFO,
			ALL // This should always be the last element of the enum.
		};

		/**
		* Handles displaying and storing info/debug messages.
		*/
		class DYNLIB Log
		{
		public:
			Log(const char* filename = "application_log.txt");
			virtual ~Log();

			/*
			 * Outputs the info message to the console and
			 * log file. This information is included in debug and
			 * release builds.
			 */
			void Info(const char* message);

			/*
			 * Outputs debug information to the console and
			 * log file. This information is only included in
			 * debug builds.
			 */
			void Debug(VERBOSITY verbosity, const char* message);

			/**
			 * Sets the log verbosity level. Statements will be printed
			 * if their verbosity are less than or equal to the global 
			 * verbosity level.
			 *
			 * @param verbosityLevel The new verbosity level.
			 */
			void SetVerbosity(VERBOSITY verbosityLevel);

		private:
			std::fstream mLogFile;
			U8 mVerbosityLevel;

#ifdef MONSOON_OS_WINDOWS
			HANDLE mConsoleHandle;
			FILE* mConsoleFileHandle;

#endif 
		};
	}
}
#endif // _MONSOON_LOG_H_