#pragma once
#include <string>

class NandStorage {
 public:
  bool Write(const std::string& lba, const std::string& dataPattern);
  unsigned int Read(const std::string& lba);

  bool IsExistNand();

 private:
  bool createNandFile();
  bool initializeNandFile();
  unsigned int hexStringToUnsignedInt(const std::string& dataPattern);
};