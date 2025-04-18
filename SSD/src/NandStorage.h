#pragma once
#include <string>

class NandStorage {
 public:
  void Write(const std::string& lba, const std::string& dataPattern);
  unsigned int Read(const std::string& lba);

  bool IsExistNand();

 private:
  void nandFileInitialize();
  unsigned int dataPatternToDigit(const std::string& dataPattern);
};