/**
* Copyright (c) 2014 Sonora Games
*
*/

#include "Platform/Clock.h"

using namespace Monsoon::Platform;

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
	return std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - mLastFrameTime).count();
}

const double Clock::getTime() const
{
	return std::chrono::high_resolution_clock::now().time_since_epoch().count();
}