#pragma once
#include <string>
#include <fstream>
#include "NandStorage.h"
#include "validator.h"
#include "recoder.h"
#include "CommandBuffer.h"
#include "SsdComponentFactory.h"

class SsdController {
 public:
  SsdController(std::unique_ptr<SsdComponentFactory> factory)
      : nandStorage(factory->createNandStorage()),
        recoder(factory->createRecoder()),
        validator(factory->createValidator()){};

  void Write(std::string lba, std::string dataPattern);
  void Read(std::string lba);
  void Erase(std::string lba, std::string scope);
  bool IsBufferingLba(std::string lba) { return true; };
  void Flush();

  void InvalidCommand(std::string errorMessage);

  void ResetResult() { recoder->ResetResult(); };
  void ClearCommandBuffer();
  std::string GetResult() { return recoder->GetResult(); };

 private:
  std::unique_ptr<NandStorage> nandStorage;
  std::unique_ptr<Recoder> recoder;
  std::unique_ptr<Validator> validator;
  CommandBuffer commandBuffer;

  std::string unsignedIntToPrefixedHexString(unsigned int readData);

  bool isBufferFull(){
    return (commandBuffer.GetValidBufferCount() == MAX_BUFFER_SIZE);
  };

  bool isValidLba(const std::string& lba);
  bool isValidScope(const std::string& scope);
  bool isValidData(const std::string& dataPattern);
};
