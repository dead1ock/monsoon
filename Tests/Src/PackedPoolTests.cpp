/**
* Copyright (c) 2016 Dallin Wellington
*
*/

#include <gtest/gtest.h>

#include <Util/PackedPool.h>

using namespace Monsoon::Util;

TEST(PackedPool, EmptySize) {
	PackedPool<int, int> pool;
	EXPECT_EQ(0, pool.Size());
}

TEST(PackedPool, Add1) {
	PackedPool<int, int> pool;

	int value = 100;
	pool.Add(1, value);

	EXPECT_EQ(1, pool.Size());
}

TEST(PackedPool, Remove1) {
	PackedPool<int, int> pool;

	int value = 100;
	pool.Add(1, value);
	EXPECT_EQ(1, pool.Size());

	pool.Remove(1);
	EXPECT_EQ(0, pool.Size());
}

TEST(PackedPool, ReuseId) {
	PackedPool<int, int> pool;

	int value = 100;
	pool.Add(1, value);
	EXPECT_EQ(1, pool.Size());

	pool.Remove(1);
	EXPECT_EQ(0, pool.Size());

	int value2 = 200;
	pool.Add(1, value2);
	EXPECT_EQ(1, pool.Size());

	pool.Remove(1);
	EXPECT_EQ(0, pool.Size());
}

TEST(PackedPool, GetById) {
	PackedPool<int, int> pool;
	int value = 100;

	pool.Add(1, value);
	EXPECT_EQ(1, pool.Size());

	EXPECT_EQ(value, pool[1]);
}

TEST(PackedPool, IndexToId) {
	PackedPool<int, int> pool;
	int value = 100;

	pool.Add(1, value);
	EXPECT_EQ(1, pool.Size());

	EXPECT_EQ(value, pool.At(0));
}

TEST(PackedPool, Sort65536) {
	int* randInts = new int[65535];
	PackedPool<int, int> pool;

	for (int x = 0; x < 65535; x++) {
		randInts[x] = x;
		pool.Add(randInts[x], randInts[x]);
	}

	EXPECT_EQ(65535, pool.Size());

	pool.Sort([](const int& a, const int& b) { return (b < a); });
}

TEST(PackedPool, Sort4096) {
	int* randInts = new int[4096];
	PackedPool<int, int> pool;

	for (int x = 0; x < 4096; x++) {
		randInts[x] = x;
		pool.Add(randInts[x], randInts[x]);
	}

	EXPECT_EQ(4096, pool.Size());

	pool.Sort([](const int& a, const int& b) { return (b < a); });
}