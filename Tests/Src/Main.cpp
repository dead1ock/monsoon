/**
* Copyright (c) 2016 Dallin Wellington
*
*/

#include <gtest/gtest.h>

int main(int argc, char* argv[]) {
	testing::InitGoogleTest(&argc, argv);
	int ret = RUN_ALL_TESTS();
	system("pause");
	return ret;
}
