#include "SsdType.h"
#include "SsdInterface.h"

#include <cctype>
#include <sstream>
#include <iomanip>

void SsdInterface::Write(std::string lba, std::string dataPattern) {
  if (isValidNumberZeroToNintyNine(lba) == false || isValidDataPattern(dataPattern) == false) {
    recordErrorPatternToOutputFile();
    return;
  }

  int offset = std::stoi(lba) * 4;
  unsigned int numDataPatter = dataPatterToDigit(dataPattern);

  if (checkNandFileExist() == false) {
    std::ofstream outfile(NAND_FILE_NAME, std::ios::binary);

    std::fstream nandFile(NAND_FILE_NAME,
                          std::ios::binary | std::ios::in | std::ios::out);
    nandFile.seekp((MAX_LBA - 1) * LBA_SIZE, std::ios::beg);

    int zero = 0;
    nandFile.write(reinterpret_cast<const char*>(&zero), sizeof(zero));
  }

  std::fstream nandFile(NAND_FILE_NAME, std::ios::binary | std::ios::in | std::ios::out);
  nandFile.seekp(offset, std::ios::beg);
  nandFile.write(reinterpret_cast<const char*>(&numDataPatter),
             sizeof(numDataPatter));
}

void SsdInterface::Read(std::string lba) { 
  if (isValidNumberZeroToNintyNine(lba) == false) return recordErrorPatternToOutputFile();

  std::ifstream nandFile(NAND_FILE_NAME, std::ios::binary);
  if (nandFile.is_open() == false) {
    recordZeroPatternToOutputFile();

    return;
  }

  int offset = std::stoi(lba) * LBA_SIZE;
  nandFile.seekg(offset, std::ios::beg);

  int readData;
  nandFile.read(reinterpret_cast<char*>(&readData), LBA_SIZE);

  std::stringstream ss;
  ss << std::uppercase << std::hex << std::setw(8) << std::setfill('0')
     << readData;
  std::string stringReadData = "0x" + ss.str();

  return recordSuccessPatternToOutputFile(stringReadData);
}

void SsdInterface::InvalidCommand(std::string errorMessage) {
  errorReason = errorMessage;
  recordErrorPatternToOutputFile();
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
#ifdef _DEBUG
    file << errorReason << "\n";
#endif
  }
}

void SsdInterface::recordSuccessPatternToOutputFile(const std::string & value) {
  output = value;

  std::ofstream file(OUTPUT_FILE_NAME, std::ios::trunc);
  if (file.is_open()) {
    file << output << "\n";
  }
}

bool SsdInterface::isValidNumberZeroToNintyNine(const std::string& str) {
  try {
    int num = std::stoi(str);

    if (num >= 0 && num <= 99) {
      return true;
    }
    else {
      errorReason = "### LBA is out of range (0~99) ###";
      return false;
    }
  } catch (...) {
    errorReason = "### LBA is not decimal ###";
    return false;
  }
}

bool SsdInterface::checkNandFileExist() {
  std::ifstream nandFile(NAND_FILE_NAME);
  return nandFile.is_open();
}
  
bool SsdInterface::isValidDataPattern(std::string dataPattern) {
  if (dataPattern.length() != 10) {
    errorReason = "### DataPattern Length is not 10 ###";
    return false;
  }

  if (dataPattern[0] != '0' || dataPattern[1] != 'x') {
    errorReason = "### DataPattern Is not started with '0x' ###";
    return false;
  }

  for (size_t i = 2; i < dataPattern.length(); ++i) {
    char c = dataPattern[i];

    if (c >= '0' && c <= '9') {
      continue;
    }

    if (c >= 'a' && c <= 'f') {
      continue;
    }

    if (c >= 'A' && c <= 'F') {
      continue;
    }

    errorReason = "### DataPattern Is not Hex number ###";
    return false;
  }

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