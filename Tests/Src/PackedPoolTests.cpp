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

TEST(PackedPool, EmptyAt) {
	//PackedPool<int, int> pool;
	//EXPECT_EQ(NULL, pool.At(0));
	FAIL();
}

TEST(PackedPool, EmptyRemove) {
	PackedPool<int, int> pool;

	pool.Remove(rand() % 1000);
	EXPECT_EQ(0, pool.Size());
}

TEST(PackedPool, EmptyExists) {
	//PackedPool<int, int> pool;
	//EXPECT_EQ(false, pool.Exists(rand() % 10000));
	FAIL();
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
	FAIL();
}

TEST(PackedPool, Add1000Random) {
	FAIL();
}

TEST(PackedPool, Remove1000Random) {
	FAIL();
}
