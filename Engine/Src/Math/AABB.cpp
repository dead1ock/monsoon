/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/

#include "Math/AABB.h"
#include <algorithm>

using namespace Monsoon::Math;

AABB::AABB()
: mX(0.0f)
, mY(0.0f)
, mZ(0.0f)
, mWidth(0.0f)
, mHeight(0.0f)
, mDepth(1.0f)
{

}

AABB::AABB(float x, float y, float width, float height)
: mX(x)
, mY(y)
, mZ(0.0f)
, mWidth(width)
, mHeight(height)
, mDepth(0.0f)
{

}

AABB::AABB(float x, float y, float z, float width, float height, float depth) 
: mX(x)
, mY(y)
, mZ(z)
, mWidth(width)
, mHeight(height)
, mDepth(depth)
{

}

AABB::~AABB()
{

}

bool AABB::Intersects(AABB& other)
{
	return (abs(mX - other.mX) * 2 < (mWidth + other.mWidth)) && (abs(mY - other.mY) * 2 < (mHeight + other.mHeight));
}