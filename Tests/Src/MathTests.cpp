/**
* Copyright (c) 2016 Dallin Wellington
*
*/

#include <gtest/gtest.h>

#include <Math/Vector.h>

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

// --------------------------------------
// Vector4 Tests
// --------------------------------------