#pragma once
#include <string>

class NandStorage {
 public:
  void Write(const std::string& lba, const std::string& dataPattern);
  unsigned int Read(const std::string& lba);
  void Erase(const std::string& lba, const std::string& scope);
  bool exists();

 private:
  bool checkNandFileExist();
  unsigned int dataPatternToDigit(const std::string& dataPattern);

 private:
  const int MAX_LBA = 100;
  const int LBA_SIZE = 4;  // 4Byte
};