/**
* Copyright (c) 2014 Sonora Games
*
*/

#include <assert.h>
#include <iostream>

#include "Platform/Log.h"

using namespace Monsoon;
using namespace Monsoon::Platform;

Log::Log(const char* filename)
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

void Log::Debug(const char* message)
{
#ifdef _DEBUG
	std::cout << message << std::endl;
	mLogFile << "[Debug] " << message << std::endl;
#endif
}

void Log::Fatal(const char* message)
{
	mLogFile << "[Fatal] " << message << std::endl;
	mLogFile.close();
	assert(false && "A fatal error has occured, see the log file.");
}

void Log::Info(const char* message)
{
#ifdef _DEBUG
	std::cout << message << std::endl;
#endif

	mLogFile << "[Info] " << message << std::endl;
}

