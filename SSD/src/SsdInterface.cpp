#include "SsdType.h"
#include "SsdInterface.h"

#include <cctype>
#include <sstream>
#include <iomanip>

void SsdInterface::Write(std::string lba, std::string dataPattern) {
  if (validator.isValidNumberZeroToNintyNine(lba) == false ||
      validator.isValidDataPattern(dataPattern) == false) {
    recoder.recordErrorPatternToOutputFile(validator.getErrorReason());
    return;
  }

  nandStorage.Write(lba, dataPattern);
}



void SsdInterface::Read(std::string lba) { 
  if (validator.isValidNumberZeroToNintyNine(lba) == false) return recoder.recordErrorPatternToOutputFile(validator.getErrorReason());

  std::ifstream nandFile(NAND_FILE_NAME, std::ios::binary);
  if (nandStorage.exists() == false) {
    recoder.recordZeroPatternToOutputFile();

    return;
  }

  unsigned int readData = nandStorage.Read(lba);

  std::stringstream ss;
  ss << std::uppercase << std::hex << std::setw(8) << std::setfill('0')
     << readData;
  std::string stringReadData = "0x" + ss.str();

  return recoder.recordSuccessPatternToOutputFile(stringReadData);
}

void SsdInterface::InvalidCommand(std::string errorMessage) {
  validator.setErrorReason(errorMessage);
  recoder.recordErrorPatternToOutputFile(validator.getErrorReason());
}

void SsdInterface::ResetResult() { recoder.resetResult(); }
std::string SsdInterface::GetResult() { return recoder.getResult(); }





