#pragma once
#include <string>
#include <fstream>

class SsdInterface {
 public:
  bool Write(std::string lba, std::string dataPattern);
  void Read(std::string lba);

  std::string GetResult();

  private:
  void recordZeroPatternToOutputFile();
  void recordErrorPatternToOutputFile();
  void recordSuccessPatternToOutputFile(const std::string& value);
  bool isValidNumberZeroToNintyNine(const std::string& str);
  const std::string OUTPUT_FILE_NAME = "ssd_output.txt";
  const std::string ZERO_PATTERN = "0x00000000";
  const std::string ERROR_PATTERN = "ERROR";
  std::string output = "0x00000000";
};
