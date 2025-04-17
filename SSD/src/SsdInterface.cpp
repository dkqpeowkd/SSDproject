#include "SsdInterface.h"

#include <cctype>
#include <sstream>

void SsdInterface::Write(std::string lba, std::string dataPattern) {
  if (isValidNumberZeroToNintyNine(lba) == false || isValidDataPattern(dataPattern) == false) {
    recordErrorPatternToOutputFile();
    return;
  }

  int offset = std::stoi(lba) * 4;
  unsigned int numDataPatter = dataPatterToDigit(dataPattern);

  if (checkNandFileExist() == false) {
    std::ofstream outfile(NAND_FILE_NAME, std::ios::binary);
  }

  std::fstream nandFile(NAND_FILE_NAME, std::ios::binary | std::ios::in | std::ios::out);
  nandFile.seekp(offset, std::ios::beg);
  nandFile.write(reinterpret_cast<const char*>(&numDataPatter),
             sizeof(numDataPatter));


}

void SsdInterface::Read(std::string lba) { 
  if (isValidNumberZeroToNintyNine(lba) == false) return recordErrorPatternToOutputFile();
  return  recordZeroPatternToOutputFile(); 
  
        
  
}

std::string SsdInterface::GetResult() { return output; }

void SsdInterface::recordZeroPatternToOutputFile() {
  output = ZERO_PATTERN;

  std::ofstream file(OUTPUT_FILE_NAME, std::ios::trunc); 
  if (file.is_open()) {
    file << output << "\n";
  }
}

void SsdInterface::recordErrorPatternToOutputFile() {
  output = ERROR_PATTERN;

  std::ofstream file(OUTPUT_FILE_NAME, std::ios::trunc);
  if (file.is_open()) {
    file << output << "\n";
  }
}

bool SsdInterface::isValidNumberZeroToNintyNine(const std::string& str) {
  for (char ch : str) {
    if (!std::isdigit(ch)) return false;
  }
  int num = std::stoi(str);

  try {
    int num = std::stoi(str);
    return num >= 0 && num <= 99;
  } catch (...) {
    return false;
  }
}

bool SsdInterface::checkNandFileExist() {
  std::ifstream nandFile(NAND_FILE_NAME);
  return nandFile.is_open();
}
  
bool SsdInterface::isValidDataPattern(std::string dataPattern) {
  if (dataPattern.length() != 10) return false;

  if (dataPattern[0] != '0' || dataPattern[1] != 'x') return false;

  return true;
}

unsigned int SsdInterface::dataPatterToDigit(std::string dataPattern) {
  std::string dataPatterWithout0x = dataPattern.substr(2);

  unsigned int numUint;
  std::stringstream ss;
  ss << std::hex << dataPatterWithout0x;
  ss >> numUint;
  
  return numUint;
}