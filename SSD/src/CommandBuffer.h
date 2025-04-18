#pragma once

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
  int GetValidBufferCount();
  void SaveBuffer();
  void DestroyBuffer();
  std::string CommandBuffer::Read(std::string lba_);
  std::vector<std::string> GetCommandBuffer();

  void ClearBuffer();


 private:
  const int MAX_BUFFER_SIZE = 5;
  std::string bufferDirectory;
  std::vector<std::string> commands;

  bool IsWriteOrEraseCommand(const std::string& command) const;
  std::unordered_map<int, std::string> BuildLbaMapFromFilenames();
};