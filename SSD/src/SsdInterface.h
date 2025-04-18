#pragma once
#include <string>
#include <fstream>
#include "NandStorage.h"
#include "validator.h"
#include "Recoder.h"

class SsdInterface {
 public:
  void Write(std::string lba, std::string dataPattern);
  void Read(std::string lba);
  void Erase(std::string lba, std::string scope);
  bool IsBufferingLba(std::string lba) { return true; };

  void InvalidCommand(std::string errorMessage);

  void ResetResult() { recoder.ResetResult(); };
  std::string GetResult() { return recoder.GetResult(); };

 private:
  const int MAX_LBA = 100;
  const int LBA_SIZE = 4;  // 4Byte

  NandStorage nandStorage;
  Validator validator;
  Recoder recoder;

  std::string unsignedIntToPrefixedHexString(unsigned int readData);
};
