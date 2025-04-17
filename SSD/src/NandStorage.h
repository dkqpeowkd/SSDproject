#pragma once
#include <string>

class NandStorage {
 public:
  void Write(std::string lba, std::string dataPattern);
  unsigned int Read(std::string lba);
  bool exists();

 private:
  bool checkNandFileExist();
  unsigned int dataPatterToDigit(std::string dataPattern);

 private:
  const int MAX_LBA = 100;
  const int LBA_SIZE = 4;  // 4Byte
};