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
  const std::string OUTPUT_FILE_NAME = "ssd_output.txt";
  const std::string ZERO_PATTERN = "0x00000000";
  std::string output = "0x00000000";
};
