/**
* Copyright (c) 2016 Dallin Wellington
*
*/

#include <gtest/gtest.h>

#include <Math/Vector.h>
#include <Math/Matrix4x4.h>

using namespace Monsoon::Math;

// --------------------------------------
// Vector2 Tests
// --------------------------------------

TEST(Vector2, Magnitude) {
	Vector2 vec = Vector2(10.0f, 5.0f);
	EXPECT_EQ(sqrt((10.0f * 10.0f) + (5.0f * 5.0f)), vec.Magnitude());
}

TEST(Vector2, Dot) {
	Vector2 vec1 = Vector2(10.0f, 5.0f);
	Vector2 vec2 = Vector2(2.0f, 7.0f);

	EXPECT_EQ((10.0f * 2.0f) + (5.0f * 7.0f), vec1 * vec2);
}

TEST(Vector2, Projection) {
	Vector2 vec1 = Vector2(1.0f, 100.0f);
	Vector2 vec2 = Vector2(100.0f, 0.0f);

	EXPECT_EQ(1.0f, vec1.Proj(vec2).Magnitude());
}

TEST(Vector2, Perpendicular) {
	Vector2 vec1 = Vector2(1.0f, 100.0f);
	Vector2 vec2 = Vector2(100.0f, 0.0f);

	EXPECT_EQ(100.0f, vec1.Perp(vec2).Magnitude());
}

TEST(Vector2, Unit) {
	Vector2 vec = Vector2(10.0f, 5.0f);

	//
	// Unit Vector Definition:
	//
	// Let v = <x, y>
	// |<x, y>| = <x/|v|,y/|v|>
	//
	EXPECT_EQ((10.0f / vec.Magnitude()), vec.Unit().mX);
	EXPECT_EQ((5.0f / vec.Magnitude()), vec.Unit().mY);
}

TEST(Vector2, VestorAddition) {
	Vector2 vec = Vector2(10.0f, 5.0f) + Vector2(2.0f, 7.0f);

	EXPECT_EQ(12, vec.mX);
	EXPECT_EQ(12, vec.mY);
}

TEST(Vector2, VectorSubtraction) {
	Vector2 vec = Vector2(10.0f, 5.0f) - Vector2(2.0f, 7.0f);

	EXPECT_EQ(8.0f, vec.mX);
	EXPECT_EQ(-2.0f, vec.mY);
}

TEST(Vector2, ScalarMultiplication) {
	Vector2 vec = Vector2(10.0f, 5.0f) * 2.0f;

	EXPECT_EQ(20.0f, vec.mX);
	EXPECT_EQ(10.0f, vec.mY);
}

TEST(Vector2, ScalarDivision) {
	Vector2 vec = Vector2(10.0f, 5.0f) / 2.0f;

	EXPECT_EQ(5.0f, vec.mX);
	EXPECT_EQ((5.0f / 2.0f), vec.mY);
}

// --------------------------------------
// Vector3 Tests
// --------------------------------------

TEST(Vector3, Magnitude) {
	Vector3 vec = Vector3(10.0f, 5.0f, 0.0f);
	EXPECT_EQ(sqrt((10.0f * 10.0f) + (5.0f * 5.0f)), vec.Magnitude());
}

TEST(Vector3, Dot) {
	Vector3 vec1 = Vector3(10.0f, 5.0f, 0.0f);
	Vector3 vec2 = Vector3(2.0f, 7.0f, 0.0f);

	EXPECT_EQ((10.0f * 2.0f) + (5.0f * 7.0f), vec1 * vec2);
}

TEST(Vector3, Projection) {
	Vector3 vec1 = Vector3(1.0f, 100.0f, 0.0f);
	Vector3 vec2 = Vector3(100.0f, 0.0f, 0.0f);

	EXPECT_EQ(1.0f, vec1.Proj(vec2).Magnitude());
}

TEST(Vector3, Perpendicular) {
	Vector3 vec1 = Vector3(1.0f, 100.0f, 0.0f);
	Vector3 vec2 = Vector3(100.0f, 0.0f, 0.0f);

	EXPECT_EQ(100.0f, vec1.Perp(vec2).Magnitude());
}

TEST(Vector3, Unit) {
	Vector3 vec = Vector3(10.0f, 5.0f, 2.0f);

	//
	// Unit Vector Definition:
	//
	// Let v = <x, y>
	// |<x, y>| = <x/|v|,y/|v|>
	//
	EXPECT_EQ((10.0f / vec.Magnitude()), vec.Unit().mX);
	EXPECT_EQ((5.0f / vec.Magnitude()), vec.Unit().mY);
	EXPECT_EQ((2.0f / vec.Magnitude()), vec.Unit().mZ);
}

TEST(Vector3, VestorAddition) {
	Vector3 vec = Vector3(10.0f, 5.0f, 2.0f) + Vector3(2.0f, 7.0f, -1.0f);

	EXPECT_EQ(12.0f, vec.mX);
	EXPECT_EQ(12.0f, vec.mY);
	EXPECT_EQ(1.0f, vec.mZ);
}

TEST(Vector3, VectorSubtraction) {
	Vector3 vec = Vector3(10.0f, 5.0f, 2.0f) - Vector3(2.0f, 7.0f, -1.0f);

	EXPECT_EQ(8.0f, vec.mX);
	EXPECT_EQ(-2.0f, vec.mY);
	EXPECT_EQ(3.0f, vec.mZ);
}

TEST(Vector3, ScalarMultiplication) {
	Vector3 vec = Vector3(10.0f, 5.0f, 2.0f) * 2.0f;

	EXPECT_EQ(20.0f, vec.mX);
	EXPECT_EQ(10.0f, vec.mY);
	EXPECT_EQ(4.0f, vec.mZ);
}

TEST(Vector3, ScalarDivision) {
	Vector3 vec = Vector3(10.0f, 5.0f, 2.0f) / 2.0f;

	EXPECT_EQ(5.0f, vec.mX);
	EXPECT_EQ((5.0f / 2.0f), vec.mY);
	EXPECT_EQ(1.0f, vec.mZ);
}

// --------------------------------------
// Vector4 Tests
// --------------------------------------

// --------------------------------------
// Matrix4x4 Tests
// --------------------------------------

/*
 * [I] * [I] = [I]
 */
TEST(Matrix4x4, IdentitySquared) {
	Matrix4x4 m1, m2;
	Matrix4x4 m3 = m1 * m2;

	EXPECT_EQ(1.0f, m3(0,0));
	EXPECT_EQ(1.0f, m3(1,1));
	EXPECT_EQ(1.0f, m3(2,2));
	EXPECT_EQ(1.0f, m3(3,3));
}

/*
 * [A] * [I] = [I] * [A] = [A]
 */
TEST(Matrix4x4, IdentityMultiplicationProperty) {
	float matrix[4][4] = {
		{ 1.0f, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 0.0f },
		{ 10.0f, -30.0f, 50.0f, 1.0f }
	};

	Matrix4x4 m1(matrix);
	Matrix4x4 i;
	Matrix4x4 left = m1 * i;
	Matrix4x4 right = i * m1;

	EXPECT_EQ(true, m1 == left);
	EXPECT_EQ(true, m1 == right);
}
