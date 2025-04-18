#include "SsdType.h"
#include "SsdInterface.h"

#include <cctype>
#include <sstream>
#include <iomanip>

void SsdInterface::Write(std::string lba, std::string dataPattern) {
  if (validator.IsNumberWithinRange(lba, 0, MAX_LBA) == false ||
      validator.IsValidDataPattern(dataPattern) == false) {
    recoder.RecordErrorPatternToOutputFile(validator.GetErrorReason());
    return;
  }

  nandStorage.Write(lba, dataPattern);
}

void SsdInterface::Read(std::string lba) { 
  if (validator.IsNumberWithinRange(lba, 0, MAX_LBA) == false) {
    return recoder.RecordErrorPatternToOutputFile(validator.GetErrorReason());
  }

  std::ifstream nandFile(NAND_FILE_NAME, std::ios::binary);
  if (nandStorage.IsExistNand() == false) {
    recoder.RecordZeroPatternToOutputFile();
    return;
  }

  unsigned int readData = nandStorage.Read(lba);
  std::string stringReadData = unsignedIntToPrefixedHexString(readData);

  return recoder.RecordSuccessPatternToOutputFile(stringReadData);
}

void SsdInterface::Erase(std::string lba, std::string scope) {
  if (validator.IsNumberWithinRange(lba, 0, MAX_LBA) == false) {
    return recoder.RecordErrorPatternToOutputFile(validator.GetErrorReason());
  }

  if (validator.IsNumberWithinRange(scope, 1, 10) == false) {
    return recoder.RecordErrorPatternToOutputFile(validator.GetErrorReason());
  }

  int writeCount = std::stoi(scope);
  int lastLba = std::stoi(lba) + writeCount;
  if (lastLba > MAX_LBA) {
    validator.SetErrorReason("### Last LBA is over Max LBA ###");
    return recoder.RecordErrorPatternToOutputFile(validator.GetErrorReason());
  }

  for (int writeOffset = 0; writeOffset < writeCount; writeOffset++) {
    std::string currentLba = std::to_string(std::stoi(lba) + writeOffset);

    nandStorage.Write(currentLba, ZERO_PATTERN);
  }
}

void SsdInterface::Flush() { 
    return;
}

std::string SsdInterface::unsignedIntToPrefixedHexString(unsigned int readData) {
  std::stringstream ss;
  ss << std::uppercase << std::hex << std::setw(8) << std::setfill('0')
     << readData;
  return "0x" + ss.str();
}

void SsdInterface::InvalidCommand(std::string errorMessage) {
  validator.SetErrorReason(errorMessage);
  recoder.RecordErrorPatternToOutputFile(validator.GetErrorReason());
}
