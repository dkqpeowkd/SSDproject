#include "SsdInterface.h"

bool SsdInterface::Write(std::string lba, std::string dataPattern) {
  return true;
}

void SsdInterface::Read(std::string lba) { checkOutputFileExist(); }

std::string SsdInterface::GetResult() { return "nothing"; }

void SsdInterface::checkOutputFileExist() {
  std::ofstream file(outputFileName, std::ios::app);  // ������ ����
  if (file.is_open()) {
    file << "content" << "\n";
  }
}