/**
* Copyright (c) 2014 Sonora Games
*
*/

#include <Math/Vector.h>
#include <math.h>

using namespace Monsoon;
using namespace Monsoon::Math;
		
Vector2::Vector2(float x, float y)
	: mX(x)
	, mY(y)
{

}

Vector2::~Vector2()
{

}

Float Vector2::Magnitude()
{
	return sqrt((mX*mX) + (mY*mY));
}

Float Vector2::Dot(Vector2& other)
{
	return ((mX * other.mX) + (mY * other.mY));
}

Vector2 Vector2::operator+(Vector2& other)
{
	return Vector2(mX + other.mX, mY + other.mY);
}

void Vector2::operator+=(Vector2& other)
{
	mX += other.mX;
	mY += other.mY;
}

//----------------------------------------------------
//
//----------------------------------------------------

Vector3::Vector3(float x, float y, float z)
	: mX(x)
	, mY(y)
	, mZ(z)
{

}

Vector3::~Vector3()
{

}

Float Vector3::Magnitude()
{
	return sqrt((mX*mX) + (mY*mY) + (mZ*mZ));
}

Float Vector3::Dot(Vector3& other)
{
	return ((mX * other.mX) + (mY * other.mY) + (mZ * other.mZ));
}

Vector3 Vector3::Cross(Vector3& other)
{
	return Vector3((mY*other.mZ) - (mZ * other.mY), (mZ*other.mX) - (mX*other.mZ), (mX*other.mY) - (mY*other.mX));
}

Vector3 Vector3::operator+(Vector3& other)
{
	return Vector3(mX + other.mX, mY + other.mY, mZ + other.mZ);
}

void Vector3::operator+=(Vector3& other)
{
	mX += other.mX;
	mY += other.mY;
	mZ += other.mZ;
}