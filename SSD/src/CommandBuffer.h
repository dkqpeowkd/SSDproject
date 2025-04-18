#pragma once

#include "SsdTYpe.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_map>

class CommandBuffer {
 public:
  CommandBuffer(const std::string& bufferDir = "buffer");

  void Init();
  void AddCommand(const std::string& command);
  int GetValidBufferCount() { return MAX_BUFFER_SIZE; }
  void Flush();
  void SaveBuffer();
  void DestroyBuffer();
  std::string Read(std::string lba_);
  std::vector<std::string> GetCommandBuffer();


 private:
  std::string bufferDirectory;
  std::vector<std::string> commands;

  void ClearBuffer();


  void ExecuteCommand(const std::string& command);
  bool IsFlushNeeded() const;
  bool IsWriteOrEraseCommand(const std::string& command) const;
  std::unordered_map<int, std::string> BuildLbaMapFromFilenames();
};