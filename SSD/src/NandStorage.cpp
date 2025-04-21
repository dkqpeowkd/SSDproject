#include <string>
#include <fstream>
#include "SsdType.h"

#include <cctype>
#include <sstream>
#include <iomanip>
#include "NandStorage.h"

bool NandStorage::Write(const std::string& lba, const std::string& dataPattern) {
  if (IsExistNand() == false) {
    if (createNandFile() == false) {
      return false;
    }

    if (initializeNandFile() == false) {
      return false;
    }
  }

  int offset = std::stoi(lba) * LBA_SIZE;
  unsigned int numDataPattern = hexStringToUnsignedInt(dataPattern);

  std::fstream nandFile(NAND_FILE_NAME,
                        std::ios::binary | std::ios::in | std::ios::out);
  nandFile.seekp(offset, std::ios::beg);
  nandFile.write(reinterpret_cast<const char*>(&numDataPattern),
                 sizeof(numDataPattern));

  return true;
}

bool NandStorage::createNandFile() {
  std::ofstream outfile(NAND_FILE_NAME, std::ios::binary);
  return outfile.is_open();
}

bool NandStorage::initializeNandFile() {
  std::fstream nandFile(NAND_FILE_NAME, std::ios::binary | std::ios::out);
  if (nandFile.is_open() == false) {
    return false;
  }

  constexpr int ZERO = 0;
  for (int i = 0; i < MAX_LBA; ++i) {
    nandFile.seekp(i * LBA_SIZE, std::ios::beg);
    nandFile.write(reinterpret_cast<const char*>(&ZERO), sizeof(ZERO));
    if (nandFile.fail() == true) {
      nandFile.close();
      return false;
    }
  }

  nandFile.close();

  return true;
}

bool NandStorage::IsExistNand() {
  std::ifstream file(NAND_FILE_NAME, std::ios::binary);
  return file.good();
}

unsigned int NandStorage::Read(const std::string& lba) {
  std::ifstream file(NAND_FILE_NAME, std::ios::binary);
  if (!file.is_open()) {
    return 0;
  }

  int offset = std::stoi(lba) * LBA_SIZE;
  file.seekg(offset, std::ios::beg);

  unsigned int value = 0;
  file.read(reinterpret_cast<char*>(&value), sizeof(value));
  if (file.fail()) {
    return 0;
  }

  return value;
}

unsigned int NandStorage::hexStringToUnsignedInt(const std::string& dataPattern) {
  std::string dataPatterWithout0x = dataPattern.substr(2);

  unsigned int numUint;
  std::stringstream ss;
  ss << std::hex << dataPatterWithout0x;
  ss >> numUint;

  return numUint;
}