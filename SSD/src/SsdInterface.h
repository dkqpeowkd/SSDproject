#pragma once
#include <string>
#include <fstream>
#include "NandStorage.h"
#include "validator.h"
#include "Recoder.h"
#include "CommandBuffer.h"

class SsdInterface {
 public:
  void Write(std::string lba, std::string dataPattern);
  void Read(std::string lba);
  void Erase(std::string lba, std::string scope);
  bool IsBufferingLba(std::string lba) { return true; };
  void Flush();

  void InvalidCommand(std::string errorMessage);

  void ResetResult() { recoder.ResetResult(); };
  void ClearCommandBuffer();
  std::string GetResult() { return recoder.GetResult(); };

 private:
  NandStorage nandStorage;
  Validator validator;
  Recoder recoder;
  CommandBuffer commandBuffer;

  void processErase(std::string lba, std::string scope);
  std::string unsignedIntToPrefixedHexString(unsigned int readData);
  bool isBufferFull(){
    return (commandBuffer.GetValidBufferCount() == MAX_BUFFER_SIZE);
  };
};
