#include "gmock/gmock.h"
#include "../src/SsdInterface.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <string>
namespace fs = std::filesystem;

// - 콘솔 출력 없음
TEST(SSD, Write_Pass) {
  SsdInterface* ssdInterface = new SsdInterface();
  std::string lba = "0";
  std::string dataPattern = "0x12345678";
  ssdInterface->Write(lba, dataPattern);

  EXPECT_NE("nothing", ssdInterface->GetResult());

  delete ssdInterface;
}

// - ssd_nand.txt 파일에 해당 LBA 위치에 값 저장
// - ssd_nand.txt에서 해당 LBA 값 읽어 ssd_output.txt에 기록
TEST(SSD, Read_Mapped) {
  SsdInterface* ssdInterface = new SsdInterface();
  std::string lba = "0";
  std::string dataPattern = "0x12345678";
  ssdInterface->Write(lba, dataPattern);
  ssdInterface->Read(lba);

  EXPECT_EQ(dataPattern, ssdInterface->GetResult());

  delete ssdInterface;
}

// - 미기록 LBA는 0x00000000 기록
TEST(SSD, Read_Unmapped) {
  SsdInterface* ssdInterface = new SsdInterface();
  
  std::string lba = "0";
  ssdInterface->Read(lba);

  std::string ZERO_PATTERN = "0x00000000";

  EXPECT_EQ(ZERO_PATTERN, ssdInterface->GetResult());

  delete ssdInterface;
}

TEST(SSD, CreateOutputFileIfNotExists) {
  const std::string filename = "ssd_output.txt";

  // 파일이 있으면 삭제
  if (std::filesystem::exists(filename)) {
    fs::remove(filename);
  }

  ASSERT_FALSE(fs::exists(filename)) << "파일이 이미 존재함";

  SsdInterface * ssdInterface = new SsdInterface();
  ssdInterface->Read("0");

  ASSERT_TRUE(fs::exists(filename)) << "파일이 생성되지 않았음";

  // cleanup
  fs::remove(filename);
}

// - 콘솔 출력 없음
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

// - LBA 범위: 0~99, 10진수
// - 잘못된 LBA/Value 입력 시 ssd_output.txt에 "ERROR" 기록
TEST(SSD, Write_Fail_OutOfRange) {
  SsdInterface* ssdInterface = new SsdInterface();
  std::string lba = "100";
  std::string dataPattern = "0x12345678";
  ssdInterface->Write(lba, dataPattern);

  EXPECT_EQ("ERROR", ssdInterface->GetResult());

  delete ssdInterface;
}

// - Value: 0x로 시작하는 8자리 16진수(총 10글자, 예: 0x1234ABCD)
// - 잘못된 LBA/Value 입력 시 ssd_output.txt에 "ERROR" 기록
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

// - 잘못된 LBA 입력 시 ssd_output.txt에 "ERROR" 기록
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
