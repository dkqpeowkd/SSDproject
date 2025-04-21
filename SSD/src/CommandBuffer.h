#pragma once

#include "SsdTYpe.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>

class CommandBuffer {
 public:
  CommandBuffer(const std::string& bufferDir = "buffer");

  void Init();
  void AddCommand(const std::string& command);
  int GetValidBufferCount();
  void SaveBuffer();
  void DestroyBuffer();
  std::string Read(std::string lba_);
  std::vector<std::string> GetCommandBuffer();

  void ClearBuffer();


 private:
  std::string bufferDirectory;
  std::vector<std::string> commands;

  bool IsWriteOrEraseCommand(const std::string& command) const;
  std::unordered_map<int, std::string> BuildLbaMapFromFilenames();
  void processWrite(const std::string& command);
  void processErase(const std::string& command);
  std::vector<std::string> convertLbaMapToBuffers(
      std::unordered_map<int, std::string> lbaMap);
};