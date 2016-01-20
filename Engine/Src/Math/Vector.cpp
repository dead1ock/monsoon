/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/

#include <Math/Vector.h>
#include <math.h>

using namespace Monsoon;
using namespace Monsoon::Math;

//----------------------------------------------------
// Vector 2
//----------------------------------------------------
		
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

Vector2 Vector2::Unit()
{
	return (*this / Magnitude());
}

Vector2 Vector2::Proj(Vector2& other)
{
	return other.Unit() * Float(Dot(other) / other.Magnitude());
}

Vector2 Vector2::Perp(Vector2& other)
{
	return (*this - Proj(other));
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

Vector2 Vector2::operator-(Vector2& other)
{
	return Vector2(mX - other.mX, mY - other.mY);
}

void Vector2::operator-=(Vector2& other)
{
	mX -= other.mX;
	mY -= other.mY;
}

Float Vector2::operator*(Vector2& other)
{
	return Dot(other);
}

Vector2 Vector2::operator* (Float scalar)
{
	return Vector2(mX * scalar, mY * scalar);
}

void Vector2::operator*=(Float scalar)
{
	mX *= scalar;
	mY *= scalar;
}

Vector2 Vector2::operator/ (Float scalar)
{
	return Vector2(mX / scalar, mY / scalar);
}

void Vector2::operator/= (Float scalar)
{
	mX /= scalar;
	mY /= scalar;
}

//----------------------------------------------------
// Vector 3
//----------------------------------------------------

Vector3::Vector3(float x, float y, float z)
	: mX(x)
	, mY(y)
	, mZ(z)
{

}

Vector3::Vector3(const Vector4& vec4)
	: mX(vec4.mX)
	, mY(vec4.mY)
	, mZ(vec4.mZ)
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

Vector3 Vector3::Unit()
{
	return (*this / Magnitude());
}

Vector3 Vector3::Proj(Vector3& other)
{
	return other.Unit() * Float(Dot(other) / other.Magnitude());
}

Vector3 Vector3::Perp(Vector3& other)
{
	return (*this - Proj(other));
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

Vector3 Vector3::operator-(Vector3& other)
{
	return Vector3(mX - other.mX, mY - other.mY, mZ - other.mZ);
}

void Vector3::operator-=(Vector3& other)
{
	mX -= other.mX;
	mY -= other.mY;
	mZ -= other.mZ;
}

Float Vector3::operator*(Vector3& other)
{
	return Dot(other);
}

Vector3 Vector3::operator* (Float scalar)
{
	return Vector3(mX * scalar, mY * scalar, mZ * scalar);
}

void Vector3::operator*=(Float scalar)
{
	mX *= scalar;
	mY *= scalar;
	mZ *= scalar;
}

Vector3 Vector3::operator/ (Float scalar)
{
	return Vector3(mX / scalar, mY / scalar, mZ / scalar);
}

void Vector3::operator/= (Float scalar)
{
	mX /= scalar;
	mY /= scalar;
	mZ /= scalar;
}

//----------------------------------------------------
// Vector 4
//----------------------------------------------------

Vector4::Vector4(float x, float y, float z, float w)
	: mX(x)
	, mY(y)
	, mZ(z)
	, mW(w)
{

}

Vector4::Vector4(const Vector3& vec3, float w)
	: mX(vec3.mX)
	, mY(vec3.mY)
	, mZ(vec3.mZ)
	, mW(w)
{

}

Vector4::~Vector4()
{

}

Float Vector4::Magnitude()
{
	return sqrt((mX*mX) + (mY*mY) + (mZ*mZ) + (mW*mW));
}

Float Vector4::Dot(Vector4& other)
{
	return ((mX * other.mX) + (mY * other.mY) + (mZ * other.mZ) + (mW * other.mW));
}

Vector4 Vector4::Unit()
{
	return (*this / Magnitude());
}

Vector4 Vector4::Proj(Vector4& other)
{
	return other.Unit() * Float(Dot(other) / other.Magnitude());
}

Vector4 Vector4::Perp(Vector4& other)
{
	return (*this - Proj(other));
}

Vector4 Vector4::operator+(Vector4& other)
{
	return Vector4(mX + other.mX, mY + other.mY, mZ + other.mZ, mW + other.mW);
}

void Vector4::operator+=(Vector4& other)
{
	mX += other.mX;
	mY += other.mY;
	mZ += other.mZ;
	mW += other.mW;
}

Vector4 Vector4::operator-(Vector4& other)
{
	return Vector4(mX - other.mX, mY - other.mY, mZ - other.mZ, mW - other.mW);
}

void Vector4::operator-=(Vector4& other)
{
	mX -= other.mX;
	mY -= other.mY;
	mZ -= other.mZ;
	mW -= other.mW;
}

Vector4 Vector4::operator* (Float scalar)
{
	return Vector4(mX * scalar, mY * scalar, mZ * scalar, mW * scalar);
}

void Vector4::operator*=(Float scalar)
{
	mX *= scalar;
	mY *= scalar;
	mZ *= scalar;
	mW *= scalar;
}

Vector4 Vector4::operator/ (Float scalar)
{
	return Vector4(mX / scalar, mY / scalar, mZ / scalar, mW /scalar);
}

void Vector4::operator/= (Float scalar)
{
	mX /= scalar;
	mY /= scalar;
	mZ /= scalar;
	mW /= scalar;
}