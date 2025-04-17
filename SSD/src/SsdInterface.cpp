#include "SsdInterface.h"

bool SsdInterface::Write(std::string lba, std::string dataPattern) {
  return true;
}

void SsdInterface::Read(std::string lba) {}

std::string SsdInterface::GetResult() { return "nothing"; }