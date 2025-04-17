#include "SsdType.h"
#include "SsdInterface.h"

#include <cctype>
#include <sstream>
#include <iomanip>

void SsdInterface::Write(std::string lba, std::string dataPattern) {
  if (validator.isValidNumberZeroToNintyNine(lba) == false ||
      validator.isValidDataPattern(dataPattern) == false) {
    recoder.RecordErrorPatternToOutputFile(validator.getErrorReason());
    return;
  }

  nandStorage.Write(lba, dataPattern);
}

void SsdInterface::Read(std::string lba) { 
  if (validator.isValidNumberZeroToNintyNine(lba) == false) {
    return recoder.RecordErrorPatternToOutputFile(validator.getErrorReason());
  }

  std::ifstream nandFile(NAND_FILE_NAME, std::ios::binary);
  if (nandStorage.exists() == false) {
    recoder.RecordZeroPatternToOutputFile();
    return;
  }

  unsigned int readData = nandStorage.Read(lba);
  std::string stringReadData = unsignedIntToPrefixedHexString(readData);

  return recoder.RecordSuccessPatternToOutputFile(stringReadData);
}

std::string SsdInterface::unsignedIntToPrefixedHexString(unsigned int readData) {
  std::stringstream ss;
  ss << std::uppercase << std::hex << std::setw(8) << std::setfill('0')
     << readData;
  return "0x" + ss.str();
}

void SsdInterface::InvalidCommand(std::string errorMessage) {
  validator.setErrorReason(errorMessage);
  recoder.RecordErrorPatternToOutputFile(validator.getErrorReason());
}
