#pragma once
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "../ReadCommand.h"


using ::testing::Return;
using ::testing::_;

class MockReadCommand : public ReadCommand {
public:
    MOCK_METHOD(int, callSystem, (const std::string&), (override));
    MOCK_METHOD(std::string, readOutput, (), (override));
};

int main() { 
	testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
