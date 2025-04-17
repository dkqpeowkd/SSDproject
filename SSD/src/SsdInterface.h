#pragma once
#include <string>
#include <fstream>

class SsdInterface {
 public:
  bool Write(std::string lba, std::string dataPattern);
  void Read(std::string lba);

  std::string GetResult();

  private:
  void checkOutputFileExist();
  const std::string outputFileName = "ssd_output.txt";
};
