#include "../src/SsdInterface.h"
#include "../src/CommandBuffer.h"
#include "gtest/gtest.h"
#include "../src/SsdType.h"
#include <filesystem>

namespace fs = std::filesystem;

constexpr const char* VALID_VALUE_1 = "0x12345678";
constexpr const char* VALID_VALUE_2 = "0xabcdef01";
constexpr const char* INVALID_VALUE_1 = "0x1";
constexpr const char* INVALID_VALUE_2 = "1234567890";

constexpr const char* DATA_IS_NOT_IN_BUFFER = "DATA_IS_NOT_IN_BUFFER";

constexpr const char* VALID_ERASE_SCOPE_1 = "1";
constexpr const char* VALID_ERASE_SCOPE_6 = "6";
constexpr const char* VALID_ERASE_SCOPE_10 = "10";
constexpr const char* INVALID_ERASE_SCOPE_11 = "11";

constexpr const char* VALID_LBA_BEGIN = "0";
constexpr const char* VALID_LBA_95 = "95";
constexpr const char* VALID_LBA_END = "99";
constexpr const char* INVALID_LBA = "100";

class CommandBufferTest : public ::testing::Test {
 protected:
  SsdInterface* ssdInterface;

  void SetUp() override { 
      bufferDir = "buffer";
      ssdInterface = new SsdInterface();
  }

  void TearDown() override {
    delete ssdInterface;
    if (fs::exists(OUTPUT_FILE_NAME)) fs::remove(OUTPUT_FILE_NAME);
    if (fs::exists(NAND_FILE_NAME)) fs::remove(NAND_FILE_NAME);
  }

  std::string bufferDir;
};

TEST_F(CommandBufferTest, InitCreatesBufferFiles) {
  CommandBuffer buffer(bufferDir);
  buffer.Init();

  for (int i = 1; i <= 5; ++i) {
    std::string filePath = bufferDir + "/" + std::to_string(i) + "_empty";
    ASSERT_TRUE(std::filesystem::exists(filePath));
  }
  buffer.DestroyBuffer();
}

TEST_F(CommandBufferTest, AddCommandStoresCommandInFiles) {
  CommandBuffer buffer(bufferDir);
  buffer.Init();
  buffer.AddCommand("W 1 ABC");
  buffer.AddCommand("E 2 3");
  buffer.SaveBuffer();

  ASSERT_TRUE(std::filesystem::exists(bufferDir + "/1_W 1 ABC"));
  ASSERT_TRUE(std::filesystem::exists(bufferDir + "/2_E 2 3"));
  ASSERT_TRUE(std::filesystem::exists(bufferDir + "/3_empty"));
  ASSERT_TRUE(std::filesystem::exists(bufferDir + "/4_empty"));
  ASSERT_TRUE(std::filesystem::exists(bufferDir + "/5_empty"));

  buffer.DestroyBuffer();
}

TEST_F(CommandBufferTest, FlushClearsBufferFiles) {
  CommandBuffer buffer(bufferDir);
  buffer.Init();
  buffer.AddCommand("W 1 0x12345678");
  buffer.AddCommand("E 2 3");
  buffer.SaveBuffer();
  buffer.Flush();

  for (int i = 1; i <= 5; ++i) {
    std::string filePath = bufferDir + "/" + std::to_string(i) + "_empty";
    ASSERT_TRUE(std::filesystem::exists(filePath));
  }
  buffer.DestroyBuffer();
}

TEST_F(CommandBufferTest, Write_Buffer_in_Buffer) {
  CommandBuffer buffer(bufferDir);
  buffer.Init();
  buffer.AddCommand("W 0 0x12345678");
  buffer.AddCommand("E 2 3");
  buffer.SaveBuffer();
  EXPECT_EQ(VALID_VALUE_1, buffer.Read(VALID_LBA_BEGIN));
  EXPECT_EQ(DATA_IS_NOT_IN_BUFFER,buffer.Read(VALID_LBA_END));
  EXPECT_EQ(ZERO_PATTERN, buffer.Read("2"));
  EXPECT_EQ(ZERO_PATTERN, buffer.Read("3"));
  EXPECT_EQ(ZERO_PATTERN, buffer.Read("4"));
  EXPECT_EQ(DATA_IS_NOT_IN_BUFFER, buffer.Read("5"));
}

TEST_F(CommandBufferTest, Write_Buffer) {
  ssdInterface->Write(VALID_LBA_BEGIN, VALID_VALUE_1);
  ssdInterface->Read(VALID_LBA_BEGIN);
  EXPECT_EQ(VALID_VALUE_1, ssdInterface->GetResult());
}

TEST_F(CommandBufferTest, Erase_Buffer) {
  ssdInterface->Write(VALID_LBA_BEGIN, VALID_VALUE_1);
  ssdInterface->Erase(VALID_LBA_BEGIN, "1");
  EXPECT_EQ(false, ssdInterface->IsBufferingLba(VALID_LBA_END));
  ssdInterface->Read(VALID_LBA_BEGIN);
  EXPECT_NE(VALID_VALUE_1, ssdInterface->GetResult());
}

TEST_F(CommandBufferTest, Implicit_Flush_Buffer) {
  for (int cmdCount = 0; cmdCount <= 6; cmdCount++) {
    std::string lba = std::to_string(std::stoi(VALID_LBA_BEGIN) + cmdCount);
    ssdInterface->Write(lba, VALID_VALUE_1);
  }

  for (int cmdCount = 0; cmdCount <= 5; cmdCount++) {
    std::string lba = std::to_string(std::stoi(VALID_LBA_BEGIN) + cmdCount);
    EXPECT_EQ(false, ssdInterface->IsBufferingLba(lba));
    ssdInterface->Read(lba);
    EXPECT_EQ(VALID_VALUE_1, ssdInterface->GetResult());
  }
}

TEST_F(CommandBufferTest, WtoW_Ignore_Buffer) {
  CommandBuffer buffer(bufferDir);
  buffer.AddCommand("W 0 0x12345678");
  buffer.AddCommand("W 0 0xabcdef01");
  buffer.SaveBuffer();
  EXPECT_EQ(1, buffer.GetValidBufferCount());
  ssdInterface->Read(VALID_LBA_BEGIN);
  EXPECT_EQ(VALID_VALUE_2, ssdInterface->GetResult());
  buffer.DestroyBuffer();
}

TEST_F(CommandBufferTest, WtoE_Ignore_Buffer) {
  CommandBuffer buffer(bufferDir);
  buffer.AddCommand("W 0 0x12345678");
  buffer.AddCommand("E 0 1");
  buffer.SaveBuffer();
  EXPECT_EQ(1, buffer.GetValidBufferCount());
  ssdInterface->Read(VALID_LBA_BEGIN);
  EXPECT_EQ(ZERO_PATTERN, ssdInterface->GetResult());
  buffer.DestroyBuffer();
}

TEST_F(CommandBufferTest, Invalid_Range_Buffer) {
  ssdInterface->Write(VALID_LBA_BEGIN, VALID_VALUE_1);
  EXPECT_EQ(false, ssdInterface->IsBufferingLba(INVALID_LBA));
}