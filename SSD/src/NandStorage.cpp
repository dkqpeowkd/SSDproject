#pragma once
#include <string>
#include <fstream>
#include "SsdType.h"

#include <cctype>
#include <sstream>
#include <iomanip>
#include "NandStorage.h"

void NandStorage::Write(const std::string& lba, const std::string& dataPattern) {
  if (IsExistNand() == false) {
    std::ofstream outfile(NAND_FILE_NAME, std::ios::binary);

    nandFileInitialize();
  }

  int offset = std::stoi(lba) * LBA_SIZE;
  unsigned int numDataPattern = dataPatternToDigit(dataPattern);

  std::fstream nandFile(NAND_FILE_NAME,
                        std::ios::binary | std::ios::in | std::ios::out);
  nandFile.seekp(offset, std::ios::beg);
  nandFile.write(reinterpret_cast<const char*>(&numDataPattern),
                 sizeof(numDataPattern));
}

void NandStorage::nandFileInitialize() {
  std::fstream nandFile(NAND_FILE_NAME,
                        std::ios::binary | std::ios::in | std::ios::out);
  const int nandOffset = MAX_LBA * LBA_SIZE;
  nandFile.seekp(nandOffset, std::ios::beg);

  constexpr int ZERO = 0;
  nandFile.write(reinterpret_cast<const char*>(&ZERO), sizeof(ZERO));
}

bool NandStorage::IsExistNand() {
  std::ifstream file(NAND_FILE_NAME, std::ios::binary);
  return file.good();
}

unsigned int NandStorage::Read(const std::string& lba) {
  std::ifstream file(NAND_FILE_NAME, std::ios::binary);

  int offset = std::stoi(lba) * LBA_SIZE;
  file.seekg(offset, std::ios::beg);

  unsigned int value = 0;
  file.read(reinterpret_cast<char*>(&value), sizeof(value));

  return value;
}

unsigned int NandStorage::dataPatternToDigit(const std::string& dataPattern) {
  std::string dataPatterWithout0x = dataPattern.substr(2);

  unsigned int numUint;
  std::stringstream ss;
  ss << std::hex << dataPatterWithout0x;
  ss >> numUint;

  return numUint;
}