#include "SsdType.h"
#include "SsdController.h"

#include <cctype>
#include <sstream>
#include <iomanip>

void SsdController::Write(std::string lba, std::string dataPattern) {
  if (isValidLba(lba) == false || isValidData(dataPattern) == false) {
    recoder->RecordErrorPatternToOutputFile(validator->GetErrorReason());
    return;
  }

  if (isBufferFull() == true) {
    Flush();
  }

  std::string writeCommand = "W " + lba + " " + dataPattern;
  commandBuffer.AddCommand(writeCommand);
}

void SsdController::ClearCommandBuffer() { commandBuffer.ClearBuffer(); }

void SsdController::Read(std::string lba) { 
  if (isValidLba(lba) == false) {
    return recoder->RecordErrorPatternToOutputFile(validator->GetErrorReason());
  }

  std::string stringReadData = FAIL_BUFFER_READ_MESSAGE;
  if (commandBuffer.GetValidBufferCount() > 0) {
    stringReadData = commandBuffer.Read(lba);
  }

  if (stringReadData == FAIL_BUFFER_READ_MESSAGE) {
    std::ifstream nandFile(NAND_FILE_NAME, std::ios::binary);
    if (nandStorage->IsExistNand() == false) {
      recoder->RecordZeroPatternToOutputFile();
      return;
    }  

    unsigned int readData = nandStorage->Read(lba);
    stringReadData = unsignedIntToPrefixedHexString(readData);
  }

  return recoder->RecordSuccessPatternToOutputFile(stringReadData);
}

void SsdController::Erase(std::string lba, std::string scope) {
  if (isValidScope(scope) == false) {
    return recoder->RecordErrorPatternToOutputFile(validator->GetErrorReason());
  }

  if (validator->IsNumberWithinRange(lba, 0, MAX_LBA) == false) {
    return recoder->RecordErrorPatternToOutputFile(validator->GetErrorReason());
  }

  int numLba = stoi(lba);
  int numScope = stoi(scope);
  std::string endLba = std::to_string(numLba + numScope);

  if (validator->IsNumberWithinRange(endLba, 0, MAX_LBA) == false) {
    return recoder->RecordErrorPatternToOutputFile(validator->GetErrorReason());
  }

  if (numScope < 0) {
    lba = endLba;
    scope = std::to_string(abs(numScope));
  }

  if (isBufferFull() == true) {
    Flush();
  }

  std::string eraseCommand = "E " + lba + " " + scope;
  commandBuffer.AddCommand(eraseCommand);
}

void SsdController::Flush() {
  commandBuffer.Flush(*nandStorage, *recoder, *validator);
}

std::string SsdController::unsignedIntToPrefixedHexString(unsigned int readData) {
  std::stringstream ss;
  ss << std::uppercase << std::hex << std::setw(8) << std::setfill('0')
     << readData;
  return "0x" + ss.str();
}

void SsdController::InvalidCommand(std::string errorMessage) {
  validator->SetErrorReason(errorMessage);
  recoder->RecordErrorPatternToOutputFile(validator->GetErrorReason());
}

bool SsdController::isValidLba(const std::string& lba) {
  return validator->IsNumberWithinRange(lba, 0, MAX_LBA);
}

bool SsdController::isValidScope(const std::string& scope) {
  return validator->IsNumberWithinRange(scope, 1, 10, true);
}

bool SsdController::isValidData(const std::string& dataPattern) {
  return validator->IsValidDataPattern(dataPattern);
}