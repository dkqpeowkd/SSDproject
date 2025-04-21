#pragma once

#include "SsdTYpe.h"
#include "Command.h"
#include "CommandFactory.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <map>

class CommandBuffer {
 public:
  CommandBuffer(const std::string& bufferDir = "buffer");

  int GetValidBufferCount();
  void AddCommand(const std::string& command);
  std::string Read(std::string lba_);

  bool IsBufferFull() { return commands.size() == MAX_BUFFER_SIZE; }
  void Flush(NandStorage& nandStorage, Recoder& recoder, Validator& validator);

  void ClearBuffer();
  void DestroyBuffer();
  std::vector<std::unique_ptr<ICommand>> GetCommandBuffer();

 private:
  void InitializeEmptyCommandBuffer();
  void SaveBuffer();
  std::vector<std::unique_ptr<ICommand>> MakeCommandBufferFromIndexedCommands(
      const std::vector<std::pair<int, std::string>>& indexedCommands);
  std::vector<std::pair<int, std::string>> MakeCommandsWithIndexFromFIleNames();
  bool IsWriteOrEraseCommand(const std::string& command) const;
  std::map<int, std::string> UpdateCommandsAndBuildLbaToValueList();
  std::map<int, std::string> MakeLbaToValueListFromFileNames();
  std::vector<std::unique_ptr<ICommand>> ConvertLbaMapToBuffers(
     const std::map<int, std::string>& lbaMap);

 private:
  std::string bufferDirectory;
  std::vector<std::unique_ptr<ICommand>> commands;
};