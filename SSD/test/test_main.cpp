#include "gmock/gmock.h"
#include "../src/SsdType.h"
#include "../src/SsdController.h"
#include "TestSsdComponentFactory.h"

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

constexpr const char* VALID_ERASE_SCOPE_1 = "1";
constexpr const char* VALID_ERASE_SCOPE_6 = "6";
constexpr const char* VALID_ERASE_SCOPE_10 = "10";
constexpr const char* INVALID_ERASE_SCOPE_11 = "11";


constexpr const char* VALID_LBA_BEGIN = "0";
constexpr const char* VALID_LBA_95 = "95";
constexpr const char* VALID_LBA_END = "99";
constexpr const char* INVALID_LBA = "100";

class SSDTest : public ::testing::Test {
 protected:
  SsdController* ssdControllerWithTest;

  void SetUp() override {
    std::unique_ptr<SsdComponentFactory> testFactory =
        std::make_unique<TestSsdComponentFactory>();
    ssdControllerWithTest = new SsdController(std::move(testFactory));
  }

  void TearDown() override {
    delete ssdControllerWithTest;
    if (fs::exists(OUTPUT_FILE_NAME)) fs::remove(OUTPUT_FILE_NAME);
    if (fs::exists(NAND_FILE_NAME)) fs::remove(NAND_FILE_NAME);
    if (fs::exists(COMMAND_BUFFER_FOLDER_NAME))
      fs::remove_all(COMMAND_BUFFER_FOLDER_NAME);
  }
};

TEST_F(SSDTest, ErasePassLBA0To1) {
  for (int i = 0; i < 100; i++) {
    ssdControllerWithTest->Write(std::to_string(i), VALID_VALUE_1);
  }
  ssdControllerWithTest->Erase(VALID_LBA_BEGIN, VALID_ERASE_SCOPE_1);
  ssdControllerWithTest->Read(VALID_LBA_BEGIN);
  EXPECT_EQ(ZERO_PATTERN, ssdControllerWithTest->GetResult());
}

TEST_F(SSDTest, ErasePassLBA0To10) {
  for (int i = 0; i < 100; i++) {
    ssdControllerWithTest->Write(std::to_string(i), VALID_VALUE_1);
  }
  ssdControllerWithTest->Erase(VALID_LBA_BEGIN, VALID_ERASE_SCOPE_10);
  for (int i = 0; i < 10; i++) {
    ssdControllerWithTest->Read(std::to_string(i));
    EXPECT_EQ(ZERO_PATTERN, ssdControllerWithTest->GetResult());
  }
}

TEST_F(SSDTest, EraseErrorLBA0To11) {
  for (int i = 0; i < 100; i++) {
    ssdControllerWithTest->Write(std::to_string(i), VALID_VALUE_1);
  }
  ssdControllerWithTest->Erase(VALID_LBA_BEGIN, INVALID_ERASE_SCOPE_11);
  EXPECT_EQ(ERROR_PATTERN, ssdControllerWithTest->GetResult());
}

TEST_F(SSDTest, EraseErrorLBA95To6) {
  for (int i = 0; i < 100; i++) {
    ssdControllerWithTest->Write(std::to_string(i), VALID_VALUE_1);
  }
  ssdControllerWithTest->Erase(VALID_LBA_95, VALID_ERASE_SCOPE_6);
  EXPECT_EQ(ERROR_PATTERN, ssdControllerWithTest->GetResult());
}

TEST_F(SSDTest, EraseErrorInvalidLBA) {
  for (int i = 0; i < 100; i++) {
    ssdControllerWithTest->Write(std::to_string(i), VALID_VALUE_1);
  }
  ssdControllerWithTest->Erase(INVALID_LBA, VALID_ERASE_SCOPE_10);
  EXPECT_EQ(ERROR_PATTERN, ssdControllerWithTest->GetResult());
}

TEST_F(SSDTest, Write_Pass_0) {
  ssdControllerWithTest->Write(VALID_LBA_BEGIN, VALID_VALUE_1);
  EXPECT_NE(ERROR_PATTERN, ssdControllerWithTest->GetResult());
}

TEST_F(SSDTest, Write_Pass_1) {
  ssdControllerWithTest->Write(VALID_LBA_END, VALID_VALUE_1);
  EXPECT_NE(ERROR_PATTERN, ssdControllerWithTest->GetResult());
}

TEST_F(SSDTest, Read_Mapped_0) {
  ssdControllerWithTest->Write(VALID_LBA_BEGIN, VALID_VALUE_1);
  ssdControllerWithTest->Read(VALID_LBA_BEGIN);
  EXPECT_EQ(VALID_VALUE_1, ssdControllerWithTest->GetResult());
}

TEST_F(SSDTest, Read_Mapped_1) {
  ssdControllerWithTest->Write(VALID_LBA_END, VALID_VALUE_1);
  ssdControllerWithTest->Read(VALID_LBA_END);
  EXPECT_EQ(VALID_VALUE_1, ssdControllerWithTest->GetResult());
}

TEST_F(SSDTest, Read_Unmapped) {
  ssdControllerWithTest->Read(VALID_LBA_BEGIN);
  EXPECT_EQ(ZERO_PATTERN, ssdControllerWithTest->GetResult());
}

TEST_F(SSDTest, CreateOutputFileIfNotExists) {
  ASSERT_FALSE(fs::exists(OUTPUT_FILE_NAME));
  ssdControllerWithTest->Read(VALID_LBA_BEGIN);
  ASSERT_TRUE(fs::exists(OUTPUT_FILE_NAME));
}

TEST_F(SSDTest, Write_Pass_After_Read) {
  EXPECT_EQ(ZERO_PATTERN, ssdControllerWithTest->GetResult());
  ssdControllerWithTest->Write(VALID_LBA_BEGIN, VALID_VALUE_1);
  EXPECT_EQ(ZERO_PATTERN, ssdControllerWithTest->GetResult());
}

TEST_F(SSDTest, Write_Fail_OutOfRange) {
  ssdControllerWithTest->Write(INVALID_LBA, VALID_VALUE_1);
  EXPECT_EQ(ERROR_PATTERN, ssdControllerWithTest->GetResult());
}

TEST_F(SSDTest, Write_Fail_InvalidPattern_0) {
  ssdControllerWithTest->Write(VALID_LBA_BEGIN, INVALID_VALUE_1);
  EXPECT_EQ(ERROR_PATTERN, ssdControllerWithTest->GetResult());
}

TEST_F(SSDTest, Write_Fail_InvalidPattern_1) {
  ssdControllerWithTest->ClearCommandBuffer();
  ssdControllerWithTest->Write(VALID_LBA_BEGIN, INVALID_VALUE_2);
  EXPECT_EQ(ERROR_PATTERN, ssdControllerWithTest->GetResult());
}

TEST_F(SSDTest, Read_Unmapped_After_Write_1) {
  ssdControllerWithTest->ClearCommandBuffer();
  ssdControllerWithTest->Write(VALID_LBA_BEGIN, VALID_VALUE_1);
  ssdControllerWithTest->Read(VALID_LBA_END);
  EXPECT_EQ(ZERO_PATTERN, ssdControllerWithTest->GetResult());
}

TEST_F(SSDTest, Read_Fail_OutOfRange) {
  ssdControllerWithTest->Read(INVALID_LBA);
  EXPECT_EQ(ERROR_PATTERN, ssdControllerWithTest->GetResult());
}

TEST_F(SSDTest, Erase_Pass_MinusSize) {
  std::string minusSize = "-5";
  ssdControllerWithTest->Erase(VALID_LBA_END, minusSize);
  EXPECT_NE(ERROR_PATTERN, ssdControllerWithTest->GetResult());
}

TEST_F(SSDTest, Erase_Fail_MinusLba) {
  std::string minusLba = "-3";
  ssdControllerWithTest->Erase(minusLba, VALID_ERASE_SCOPE_1);
  EXPECT_EQ(ERROR_PATTERN, ssdControllerWithTest->GetResult());
}

TEST_F(SSDTest, Erase_Fail_MinusSize) {
  std::string minusSize = "-5";
  ssdControllerWithTest->Erase(VALID_LBA_BEGIN, minusSize);
  EXPECT_EQ(ERROR_PATTERN, ssdControllerWithTest->GetResult());
}

TEST_F(SSDTest, Erase_Fail_MinusLbaAndSize) {
  std::string minusLba = "-3";
  std::string minusSize = "-5";
  ssdControllerWithTest->Erase(minusLba, minusSize);
  EXPECT_EQ(ERROR_PATTERN, ssdControllerWithTest->GetResult());
}

int main() { 
	testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
