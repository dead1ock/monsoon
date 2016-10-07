/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/

#include <assert.h>
#include <iostream>

#include "Platform/Log.h"

using namespace Monsoon;
using namespace Monsoon::Platform;

const VERBOSITY VerbosityLevel = ALL;


Log::Log(const char* filename)
	: mVerbosityLevel(VerbosityLevel)
{
	mLogFile.open(filename, std::ios_base::out);

	// Open Debug Console
#ifdef _DEBUG

#ifdef MONSOON_OS_WINDOWS
	AllocConsole();

	mConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	int hCrt = _open_osfhandle((long)mConsoleHandle, _O_TEXT);
	mConsoleFileHandle = _fdopen(hCrt, "w");
	setvbuf(mConsoleFileHandle, NULL, _IONBF, 1);
	*stdout = *mConsoleFileHandle;
#endif

#endif
}

Log::~Log()
{
	mLogFile.close();
}

void Log::Debug(VERBOSITY verbosity, const char* message)
{
#ifdef _DEBUG
	if (mVerbosityLevel >= verbosity) {
		std::cout << message << std::endl;
		mLogFile << "[Debug] " << message << std::endl;
	}
#endif
}

void Log::Debug(const char* message)
{
#ifdef _DEBUG
	std::cout << message << std::endl;
	mLogFile << "[Debug] " << message << std::endl;
#endif
}

void Log::SetVerbosity(VERBOSITY verbosityLevel) {
	mVerbosityLevel = verbosityLevel;
}