/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/

#include "Platform/Clock.h"

using namespace Monsoon::Platform;
using namespace std::chrono;

Clock::Clock() 
{
	
}

Clock::~Clock()
{
}

void Clock::Update()
{
	mLastFrameTime = std::chrono::high_resolution_clock::now();
}

const double Clock::getDeltaTime() const
{
	return std::chrono::duration_cast<duration<double>>(std::chrono::high_resolution_clock::now() - mLastFrameTime).count();
}

const double Clock::getTime() const
{
	return duration_cast<duration<double>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}