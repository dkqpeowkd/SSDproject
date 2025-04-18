#include "gmock/gmock.h"
#include "../src/SsdType.h"
#include "../src/SsdInterface.h"

#include <fstream>
#include <iostream>
#include <filesystem>
#include <string>
#include <stdexcept>
#include <sstream>

namespace fs = std::filesystem;

constexpr const char* VALID_VALUE_1 = "0x12345678";
constexpr const char* VALID_VALUE_2 = "0xabcdef01";
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
  ssdInterface->Write(VALID_LBA_BEGIN, VALID_VALUE_1);
  EXPECT_NE(ERROR_PATTERN, ssdInterface->GetResult());
}

TEST_F(SSDTest, Write_Pass_1) {
  ssdInterface->Write(VALID_LBA_END, VALID_VALUE_1);
  EXPECT_NE(ERROR_PATTERN, ssdInterface->GetResult());
}

TEST_F(SSDTest, Read_Mapped_0) {
  ssdInterface->Write(VALID_LBA_BEGIN, VALID_VALUE_1);
  ssdInterface->Read(VALID_LBA_BEGIN);
  EXPECT_EQ(VALID_VALUE_1, ssdInterface->GetResult());
}

TEST_F(SSDTest, Read_Mapped_1) {
  ssdInterface->Write(VALID_LBA_END, VALID_VALUE_1);
  ssdInterface->Read(VALID_LBA_END);
  EXPECT_EQ(VALID_VALUE_1, ssdInterface->GetResult());
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
  ssdInterface->Write(VALID_LBA_BEGIN, VALID_VALUE_1);
  EXPECT_EQ(ZERO_PATTERN, ssdInterface->GetResult());
}

TEST_F(SSDTest, Write_Fail_OutOfRange) {
  ssdInterface->Write(INVALID_LBA, VALID_VALUE_1);
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
  ssdInterface->Write(VALID_LBA_BEGIN, VALID_VALUE_1);
  ssdInterface->Read(VALID_LBA_END);
  EXPECT_EQ(ZERO_PATTERN, ssdInterface->GetResult());
}

TEST_F(SSDTest, Read_Fail_OutOfRange) {
  ssdInterface->Read(INVALID_LBA);
  EXPECT_EQ(ERROR_PATTERN, ssdInterface->GetResult());
}

TEST_F(SSDTest, Write_Buffer) { 
  ssdInterface->Write(VALID_LBA_BEGIN, VALID_VALUE_1);
  EXPECT_EQ(true, ssdInterface->IsBufferingLba(VALID_LBA_BEGIN));
  EXPECT_EQ(false, ssdInterface->IsBufferingLba(VALID_LBA_END));
}

TEST_F(SSDTest, Erase_Buffer) {
  ssdInterface->Write(VALID_LBA_BEGIN, VALID_VALUE_1);
  ssdInterface->Erase(VALID_LBA_BEGIN, 1);
  EXPECT_EQ(false, ssdInterface->IsBufferingLba(VALID_LBA_END));
  EXPECT_NE(VALID_VALUE_1, ssdInterface->Read(VALID_LBA_BEGIN));
}

TEST_F(SSDTest, Explicit_Flush_Buffer) {
  ssdInterface->Write(VALID_LBA_BEGIN, VALID_VALUE_1);
  ssdInterface->Flush();
  EXPECT_EQ(false, ssdInterface->IsBufferingLba(VALID_LBA_END));
  EXPECT_EQ(VALID_VALUE_1, ssdInterface->Read(VALID_LBA_BEGIN));
}

TEST_F(SSDTest, Implicit_Flush_Buffer) {
  for (int cmdCount = 0; cmdCount <= 6; cmdCount++) {
    std::string lba = std::to_string(std::stoi(VALID_LBA_BEGIN) + cmdCount);
    ssdInterface->Write(lba, VALID_VALUE_1);
  }

  for (int cmdCount = 0; cmdCount <= 5; cmdCount++) {
    std::string lba = std::to_string(std::stoi(VALID_LBA_BEGIN) + cmdCount);
    EXPECT_EQ(false, ssdInterface->IsBufferingLba(lba));
    EXPECT_EQ(VALID_VALUE_1, ssdInterface->Read(lba));
  }
}

TEST_F(SSDTest, WtoW_Ignore_Buffer) {
  ssdInterface->Write(VALID_LBA_BEGIN, VALID_VALUE_1);
  ssdInterface->Write(VALID_LBA_BEGIN, VALID_VALUE_2);
  EXPECT_EQ(1, ssdInterface->GetValidBufferCount());
  EXPECT_EQ(VALID_VALUE_2, ssdInterface->Read(VALID_LBA_BEGIN));
}

TEST_F(SSDTest, WtoE_Ignore_Buffer) {
  ssdInterface->Write(VALID_LBA_BEGIN, VALID_VALUE_1);
  ssdInterface->Erase(VALID_LBA_BEGIN, 1);
  EXPECT_EQ(1, ssdInterface->GetValidBufferCount());
  EXPECT_EQ(ZERO_PATTERN, ssdInterface->Read(VALID_LBA_BEGIN));
}

TEST_F(SSDTest, Invalid_Range_Buffer) {
  ssdInterface->Write(VALID_LBA_BEGIN, VALID_VALUE_1);
  EXPECT_EQ(false, ssdInterface->IsBufferingLba(INVALID_LBA));
}

int main() { 
	testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
