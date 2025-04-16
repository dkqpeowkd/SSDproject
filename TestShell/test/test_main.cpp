#include "gmock/gmock.h"
#include <iostream>

TEST(TS, TC) { EXPECT_EQ(1, 1); }

int main() { 
	testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
