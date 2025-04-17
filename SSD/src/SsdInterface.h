#pragma once
#include <string>

class SsdInterface {
 public:
  bool Write(std::string lba, std::string dataPattern);
  void Read(std::string lba);

  std::string GetResult();
};