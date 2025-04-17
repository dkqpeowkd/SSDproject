#include "gmock/gmock.h"
#include "../src/SsdType.h"
#include "../src/SsdInterface.h"

#include <fstream>
#include <iostream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

constexpr const char* VALID_VALUE = "0x12345678";
constexpr const char* INVALID_VALUE_1 = "0x1";
constexpr const char* INVALID_VALUE_2 = "1234567890";

constexpr const char* VALID_LBA_BEGIN = "0";
constexpr const char* VALID_LBA_END = "99";
constexpr const char* INVALID_LBA = "100";

class SSDTest : public ::testing::Test {
 protected:
  SsdInterface* ssdInterface;

  void SetUp() override { ssdInterface = new SsdInterface(); }

  void TearDown() override {
    delete ssdInterface;
    if (fs::exists(OUTPUT_FILE_NAME)) fs::remove(OUTPUT_FILE_NAME);
    if (fs::exists(NAND_FILE_NAME)) fs::remove(NAND_FILE_NAME);
  }
};

TEST_F(SSDTest, Write_Pass_0) {
  ssdInterface->Write(VALID_LBA_BEGIN, VALID_VALUE);
  EXPECT_NE(ERROR_PATTERN, ssdInterface->GetResult());
}

TEST_F(SSDTest, Write_Pass_1) {
  ssdInterface->Write(VALID_LBA_END, VALID_VALUE);
  EXPECT_NE(ERROR_PATTERN, ssdInterface->GetResult());
}

TEST_F(SSDTest, Read_Mapped_0) {
  ssdInterface->Write(VALID_LBA_BEGIN, VALID_VALUE);
  ssdInterface->Read(VALID_LBA_BEGIN);
  EXPECT_EQ(VALID_VALUE, ssdInterface->GetResult());
}

TEST_F(SSDTest, Read_Mapped_1) {
  ssdInterface->Write(VALID_LBA_END, VALID_VALUE);
  ssdInterface->Read(VALID_LBA_END);
  EXPECT_EQ(VALID_VALUE, ssdInterface->GetResult());
}

TEST_F(SSDTest, Read_Unmapped) {
  ssdInterface->Read(VALID_LBA_BEGIN);
  EXPECT_EQ(ZERO_PATTERN, ssdInterface->GetResult());
}

TEST_F(SSDTest, CreateOutputFileIfNotExists) {
  ASSERT_FALSE(fs::exists(OUTPUT_FILE_NAME));
  ssdInterface->Read(VALID_LBA_BEGIN);
  ASSERT_TRUE(fs::exists(OUTPUT_FILE_NAME));
}

TEST_F(SSDTest, Write_Pass_After_Read) {
  EXPECT_EQ(ZERO_PATTERN, ssdInterface->GetResult());
  ssdInterface->Write(VALID_LBA_BEGIN, VALID_VALUE);
  EXPECT_EQ(ZERO_PATTERN, ssdInterface->GetResult());
}

TEST_F(SSDTest, Write_Fail_OutOfRange) {
  ssdInterface->Write(INVALID_LBA, VALID_VALUE);
  EXPECT_EQ(ERROR_PATTERN, ssdInterface->GetResult());
}

TEST_F(SSDTest, Write_Fail_InvalidPattern_0) {
  ssdInterface->Write(VALID_LBA_BEGIN, INVALID_VALUE_1);
  EXPECT_EQ(ERROR_PATTERN, ssdInterface->GetResult());
}

TEST_F(SSDTest, Write_Fail_InvalidPattern_1) {
  ssdInterface->Write(VALID_LBA_BEGIN, INVALID_VALUE_2);
  EXPECT_EQ(ERROR_PATTERN, ssdInterface->GetResult());
}

TEST_F(SSDTest, Read_Unmapped_After_Write_1) {
  ssdInterface->Write(VALID_LBA_BEGIN, VALID_VALUE);
  ssdInterface->Read(VALID_LBA_END);
  EXPECT_EQ(ZERO_PATTERN, ssdInterface->GetResult());
}

TEST_F(SSDTest, Read_Fail_OutOfRange) {
  ssdInterface->Read(INVALID_LBA);
  EXPECT_EQ(ERROR_PATTERN, ssdInterface->GetResult());
}

int main() { 
	testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
