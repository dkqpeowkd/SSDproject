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

  void InvalidCommand(std::string errorMessage);

  std::string GetResult();

 private:
  const int MAX_LBA = 100;
  const int LBA_SIZE = 4;  // 4Byte

  NandStorage nandStorage;
  Validator validator;
  Recoder recoder;



};
