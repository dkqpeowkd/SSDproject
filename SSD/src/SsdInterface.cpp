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

  if (commandBuffer.GetValidBufferCount() == 5) {
    Flush();
  }

  std::string writeCommand = "W " + lba + " " + dataPattern;
  commandBuffer.AddCommand(writeCommand);
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

  bool isReadDone = false;
  unsigned int readData;
  if (commandBuffer.GetValidBufferCount() > 0) {
    isReadDone = commandBuffer.Read(lba, readData);
  }

  if (isReadDone == false) {
    readData = nandStorage.Read(lba);
  }

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

  if (commandBuffer.GetValidBufferCount() == 5) {
    Flush();
  }

  std::string eraseCommand = "E " + lba + " " + scope;
  commandBuffer.AddCommand(eraseCommand);
}

void SsdInterface::Flush() { 
  int validBufferCount = commandBuffer.GetValidBufferCount();
  // get buffer;
  // process command;

  commandBuffer.DestroyBuffer();
}

void SsdInterface::processErase(std::string lba, std::string scope) {
  int writeCount = std::stoi(scope);
  int lastLba = std::stoi(lba) + writeCount;
  if (lastLba > MAX_LBA) {
    validator.SetErrorReason("### Last LBA is over Max LBA ###");
    return recoder.RecordErrorPatternToOutputFile(validator.GetErrorReason());
  }

  for (int writeOffset = 0; writeOffset < writeCount; writeOffset++) {
    std::string currentLba = std::to_string(std::stoi(lba) + writeOffset);

    nandStorage.Write(currentLba, ZERO_PATTERN);
    validator.SetErrorReason(" ### Write Fail (about File) ### ");
    recoder.RecordErrorPatternToOutputFile(validator.GetErrorReason());
  }
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
