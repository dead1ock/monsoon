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
	PackedPool<int, int> pool;
	EXPECT_EQ(NULL, pool.At(0));
}

TEST(PackedPool, EmptyRemove) {
	
}

TEST(PackedPool, EmptyExists) {

}