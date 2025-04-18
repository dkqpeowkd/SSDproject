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

  if (isBufferFull() == true) {
    Flush();
  }

  std::string writeCommand = "W " + lba + " " + dataPattern;
  commandBuffer.AddCommand(writeCommand);
}

void SsdInterface::ClearCommandBuffer() { commandBuffer.ClearBuffer(); }

void SsdInterface::Read(std::string lba) { 
  if (validator.IsNumberWithinRange(lba, 0, MAX_LBA) == false) {
    return recoder.RecordErrorPatternToOutputFile(validator.GetErrorReason());
  }

  std::string stringReadData;
  if (commandBuffer.GetValidBufferCount() > 0) {
    stringReadData = commandBuffer.Read(lba);
  }

  if (stringReadData == FAIL_BUFFER_READ_MESSAGE) {
    std::ifstream nandFile(NAND_FILE_NAME, std::ios::binary);
    if (nandStorage.IsExistNand() == false) {
      recoder.RecordZeroPatternToOutputFile();
      return;
    }  

    unsigned int readData = nandStorage.Read(lba);
    stringReadData = unsignedIntToPrefixedHexString(readData);
  }

  return recoder.RecordSuccessPatternToOutputFile(stringReadData);
}

void SsdInterface::Erase(std::string lba, std::string scope) {
  if (validator.IsNumberWithinRange(scope, 1, 10, true) == false) {
    return recoder.RecordErrorPatternToOutputFile(validator.GetErrorReason());
  }

  if (validator.IsNumberWithinRange(lba, 0, MAX_LBA) == false) {
    return recoder.RecordErrorPatternToOutputFile(validator.GetErrorReason());
  }

  int numLba = stoi(lba);
  int numScope = stoi(scope);
  std::string endLba = std::to_string(numLba + numScope);

  if (validator.IsNumberWithinRange(endLba, 0, MAX_LBA) == false) {
    return recoder.RecordErrorPatternToOutputFile(validator.GetErrorReason());
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

void SsdInterface::Flush() { 
  std::vector<std::string> commands = commandBuffer.GetCommandBuffer();
  const int bufferItemCount = commands.size();

  for (int bufferSlot = 0; bufferSlot < bufferItemCount; bufferSlot++) {
    std::string command = commands[bufferSlot];

    std::istringstream issCommand(command);
    std::string commandType;
    issCommand >> commandType;

    if (commandType == "W") {
      std::string lba;
      std::string dataPattern;
      issCommand >> lba >> dataPattern;

      if (nandStorage.Write(lba, dataPattern) == false) {
        validator.SetErrorReason(" ### Write Fail (about File) ### ");
        recoder.RecordErrorPatternToOutputFile(validator.GetErrorReason());
      }
    } else if (commandType == "E") {
      std::string lba;
      std::string scope;
      issCommand >> lba >> scope;

      processErase(lba, scope);
    } else {
      validator.SetErrorReason(" ### Buffer Is Brocken ### (commandType: " +
                               commandType);
      return recoder.RecordErrorPatternToOutputFile(validator.GetErrorReason());
    }
  }

  commandBuffer.ClearBuffer();
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

    if (nandStorage.Write(currentLba, ZERO_PATTERN) == false) {
      validator.SetErrorReason(" ### Write Fail (about File) ### ");
      recoder.RecordErrorPatternToOutputFile(validator.GetErrorReason());
    }
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
