#pragma once
#include <string>
#include <fstream>

class SsdInterface {
 public:
  void Write(std::string lba, std::string dataPattern);
  void Read(std::string lba);

  void InvalidCommand(std::string errorMessage);

  std::string GetResult();

 private:
  const int MAX_LBA = 100;
  const int LBA_SIZE = 4;  // 4Byte

  std::string output = "0x00000000";
  std::string errorReason = "";

  void recordZeroPatternToOutputFile();
  void recordErrorPatternToOutputFile();
  void recordSuccessPatternToOutputFile(const std::string& value);

  bool checkNandFileExist();

  bool isValidNumberZeroToNintyNine(const std::string& str);
  bool isValidDataPattern(std::string dataPattern);

  unsigned int dataPatterToDigit(std::string dataPattern);
};
