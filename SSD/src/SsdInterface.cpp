#include "SsdInterface.h"

bool SsdInterface::Write(std::string lba, std::string dataPattern) {
  return true;
}

void SsdInterface::Read(std::string lba) { recordZeroPatternToOutputFile(); }

std::string SsdInterface::GetResult() { return output; }

void SsdInterface::recordZeroPatternToOutputFile() {
  output = ZERO_PATTERN;

  std::ofstream file(OUTPUT_FILE_NAME, std::ios::trunc); 
  if (file.is_open()) {
    file << output << "\n";
  }
}