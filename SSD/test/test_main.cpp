#include "gmock/gmock.h"
#include "../src/SsdType.h"
#include "../src/SsdInterface.h"

#include <fstream>
#include <iostream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

// - �ܼ� ��� ����
TEST(SSD, Write_Pass_0) {
  SsdInterface* ssdInterface = new SsdInterface();
  std::string lba = "0";
  std::string dataPattern = "0x12345678";
  ssdInterface->Write(lba, dataPattern);

  EXPECT_NE(ERROR_PATTERN, ssdInterface->GetResult());

  delete ssdInterface;
}

TEST(SSD, Write_Pass_1) {
  SsdInterface* ssdInterface = new SsdInterface();
  std::string lba = "99";
  std::string dataPattern = "0x12345678";
  ssdInterface->Write(lba, dataPattern);

  EXPECT_NE(ERROR_PATTERN, ssdInterface->GetResult());

  delete ssdInterface;
}

// - ssd_nand.txt ���Ͽ� �ش� LBA ��ġ�� �� ����
// - ssd_nand.txt���� �ش� LBA �� �о� ssd_output.txt�� ���
TEST(SSD, Read_Mapped_0) {
  SsdInterface* ssdInterface = new SsdInterface();
  std::string lba = "0";
  std::string dataPattern = "0x12345678";
  ssdInterface->Write(lba, dataPattern);
  ssdInterface->Read(lba);

  EXPECT_EQ(dataPattern, ssdInterface->GetResult());

  delete ssdInterface;
}

TEST(SSD, Read_Mapped_1) {
  SsdInterface* ssdInterface = new SsdInterface();
  std::string lba = "99";
  std::string dataPattern = "0x12345678";
  ssdInterface->Write(lba, dataPattern);
  ssdInterface->Read(lba);

  EXPECT_EQ(dataPattern, ssdInterface->GetResult());

  delete ssdInterface;
}

// - �̱�� LBA�� 0x00000000 ���
TEST(SSD, Read_Unmapped) {
  if (fs::exists(NAND_FILE_NAME)) {
    fs::remove(NAND_FILE_NAME);
  }

  SsdInterface* ssdInterface = new SsdInterface();
  
  std::string lba = "0";
  ssdInterface->Read(lba);

  EXPECT_EQ(ZERO_PATTERN, ssdInterface->GetResult());

  delete ssdInterface;
}

TEST(SSD, CreateOutputFileIfNotExists) {
  // ������ ������ ����
  if (std::filesystem::exists(OUTPUT_FILE_NAME)) {
    fs::remove(OUTPUT_FILE_NAME);
  }

  ASSERT_FALSE(fs::exists(OUTPUT_FILE_NAME)) << "������ �̹� ������";

  SsdInterface * ssdInterface = new SsdInterface();
  ssdInterface->Read("0");

  ASSERT_TRUE(fs::exists(OUTPUT_FILE_NAME)) << "������ �������� �ʾ���";

  // cleanup
  fs::remove(OUTPUT_FILE_NAME);
}

// - �ܼ� ��� ����
TEST(SSD, Write_Pass_After_Read) {
  SsdInterface* ssdInterface = new SsdInterface();

  EXPECT_EQ(ZERO_PATTERN, ssdInterface->GetResult());

  std::string lba = "0";
  std::string dataPattern = "0x12345678";
  ssdInterface->Write(lba, dataPattern);

  EXPECT_EQ(ZERO_PATTERN, ssdInterface->GetResult());

  delete ssdInterface;
}

// - LBA ����: 0~99, 10����
// - �߸��� LBA/Value �Է� �� ssd_output.txt�� "ERROR" ���
TEST(SSD, Write_Fail_OutOfRange) {
  SsdInterface* ssdInterface = new SsdInterface();
  std::string lba = "100";
  std::string dataPattern = "0x12345678";
  ssdInterface->Write(lba, dataPattern);

  EXPECT_EQ(ERROR_PATTERN, ssdInterface->GetResult());

  delete ssdInterface;
}

// - Value: 0x�� �����ϴ� 8�ڸ� 16����(�� 10����, ��: 0x1234ABCD)
// - �߸��� LBA/Value �Է� �� ssd_output.txt�� "ERROR" ���
TEST(SSD, Write_Fail_InvalidPattern_0) {
  SsdInterface* ssdInterface = new SsdInterface();
  std::string lba = "0";
  std::string dataPattern = "0x1";
  ssdInterface->Write(lba, dataPattern);

  EXPECT_EQ(ERROR_PATTERN, ssdInterface->GetResult());

  delete ssdInterface;
}

TEST(SSD, Write_Fail_InvalidPattern_1) {
  SsdInterface* ssdInterface = new SsdInterface();
  std::string lba = "0";
  std::string dataPattern = "1234567890";
  ssdInterface->Write(lba, dataPattern);

  EXPECT_EQ(ERROR_PATTERN, ssdInterface->GetResult());

  delete ssdInterface;
}

// - �߸��� LBA �Է� �� ssd_output.txt�� "ERROR" ���
TEST(SSD, Read_Fail_OutOfRange) {
  SsdInterface* ssdInterface = new SsdInterface();
  std::string lba = "100";
  ssdInterface->Read(lba);

  EXPECT_EQ(ERROR_PATTERN, ssdInterface->GetResult());

  delete ssdInterface;
}

int main() { 
	testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
