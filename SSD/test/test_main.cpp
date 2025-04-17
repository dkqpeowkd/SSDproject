#include "gmock/gmock.h"
#include "../src/SsdInterface.h"

#include <iostream>
#include <string>

// - �ܼ� ��� ����
TEST(SSD, Write_Pass) {
  SsdInterface* ssdInterface = new SsdInterface();
  std::string lba = "0";
  std::string dataPattern = "0x12345678";
  ssdInterface->Write(lba, dataPattern);

  EXPECT_EQ("nothing", ssdInterface->GetResult());

  delete ssdInterface;
}

// - ssd_nand.txt ���Ͽ� �ش� LBA ��ġ�� �� ����
// - ssd_nand.txt���� �ش� LBA �� �о� ssd_output.txt�� ���
TEST(SSD, Read_Mapped) {
  SsdInterface* ssdInterface = new SsdInterface();
  std::string lba = "0";
  std::string dataPattern = "0x12345678";
  ssdInterface->Write(lba, dataPattern);
  ssdInterface->Read(lba);

  EXPECT_EQ(dataPattern, ssdInterface->GetResult());

  delete ssdInterface;
}

// - �̱�� LBA�� 0x00000000 ���
TEST(SSD, Read_Unmapped) {
  SsdInterface* ssdInterface = new SsdInterface();
  
  std::string lba = "0";
  ssdInterface->Read(lba);

  std::string ZERO_PATTERN = "0x00000000";

  EXPECT_EQ(ZERO_PATTERN, ssdInterface->GetResult());

  delete ssdInterface;
}

// - �ܼ� ��� ����
TEST(SSD, Write_Pass_After_Read) {
  SsdInterface* ssdInterface = new SsdInterface();

  std::string ZERO_PATTERN = "0x00000000";

  EXPECT_EQ(ZERO_PATTERN, ssdInterface->GetResult());

  std::string lba = "0";
  std::string dataPattern = "0x12345678";
  ssdInterface->Write(lba, dataPattern);

  EXPECT_EQ("nothing", ssdInterface->GetResult());

  delete ssdInterface;
}

// - LBA ����: 0~99, 10����
// - �߸��� LBA/Value �Է� �� ssd_output.txt�� "ERROR" ���
TEST(SSD, Write_Fail_OutOfRange) {
  SsdInterface* ssdInterface = new SsdInterface();
  std::string lba = "100";
  std::string dataPattern = "0x12345678";
  ssdInterface->Write(lba, dataPattern);

  EXPECT_EQ("ERROR", ssdInterface->GetResult());

  delete ssdInterface;
}

// - Value: 0x�� �����ϴ� 8�ڸ� 16����(�� 10����, ��: 0x1234ABCD)
// - �߸��� LBA/Value �Է� �� ssd_output.txt�� "ERROR" ���
TEST(SSD, Write_Fail_InvalidPattern_0) {
  SsdInterface* ssdInterface = new SsdInterface();
  std::string lba = "0";
  std::string dataPattern = "0x1";
  ssdInterface->Write(lba, dataPattern);

  EXPECT_EQ("ERROR", ssdInterface->GetResult());

  delete ssdInterface;
}

TEST(SSD, Write_Fail_InvalidPattern_1) {
  SsdInterface* ssdInterface = new SsdInterface();
  std::string lba = "0";
  std::string dataPattern = "1234567890";
  ssdInterface->Write(lba, dataPattern);

  EXPECT_EQ("ERROR", ssdInterface->GetResult());

  delete ssdInterface;
}

// - �߸��� LBA �Է� �� ssd_output.txt�� "ERROR" ���
TEST(SSD, Read_Fail_OutOfRange) {
  SsdInterface* ssdInterface = new SsdInterface();
  std::string lba = "100";
  ssdInterface->Read(lba);

  EXPECT_EQ("ERROR", ssdInterface->GetResult());

  delete ssdInterface;
}

int main() { 
	testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
