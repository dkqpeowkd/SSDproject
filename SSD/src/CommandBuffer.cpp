#include "CommandBuffer.h"
#include "SsdType.h"
#include <sstream>
#include <algorithm>

CommandBuffer::CommandBuffer(const std::string& bufferDir)
    : bufferDirectory(bufferDir) {}

void CommandBuffer::Init() {
  std::filesystem::create_directory(bufferDirectory);
  for (int i = 1; i <= MAX_BUFFER_SIZE; ++i) {
    std::ofstream file(bufferDirectory + "/" + std::to_string(i) + "_empty");
  }
}

void CommandBuffer::AddCommand(const std::string& command) {
  if (!IsWriteOrEraseCommand(command)) return;

  if (commands.size() >= MAX_BUFFER_SIZE) {
    Flush();
  }
  commands.push_back(command);
  SaveBuffer();
}

void CommandBuffer::Flush() {
  for (const auto& cmd : commands) {
    try {
      ExecuteCommand(cmd);
    } catch (const std::exception& e) {
      std::cerr << "Command execution failed: " << e.what() << std::endl;
    }
  }
  commands.clear();
  ClearBuffer();
}

void CommandBuffer::SaveBuffer() {
  DestroyBuffer();

  if (!std::filesystem::exists(bufferDirectory)) {
    std::filesystem::create_directories(bufferDirectory);
  }

  for (int i = 0; i < MAX_BUFFER_SIZE; ++i) {
    std::string newPath;

    if (i < commands.size()) {
      newPath =
          bufferDirectory + "/" + std::to_string(i + 1) + "_" + commands[i];
    } else {
      newPath = bufferDirectory + "/" + std::to_string(i + 1) + "_empty";
    }

   std::ofstream createFile(newPath);
  }
}

void CommandBuffer::ClearBuffer() {
  for (int i = 1; i <= MAX_BUFFER_SIZE; ++i) {
    for (const auto& entry :
         std::filesystem::directory_iterator(bufferDirectory)) {
      std::string filename = entry.path().filename().string();
      if (filename.find(std::to_string(i) + "_") == 0) {
        std::filesystem::rename(
            entry.path(), bufferDirectory + "/" + std::to_string(i) + "_empty");
        break;
      }
    }
  }
}

void CommandBuffer::DestroyBuffer() {
  if (std::filesystem::exists(bufferDirectory)) {
    std::filesystem::remove_all(bufferDirectory);
  }
}

bool CommandBuffer::IsFlushNeeded() const {
  return commands.size() >= MAX_BUFFER_SIZE;
}

bool CommandBuffer::IsWriteOrEraseCommand(const std::string& command) const {
  return !command.empty() && (command[0] == 'W' || command[0] == 'E');
}

void CommandBuffer::ExecuteCommand(const std::string& command) {
  // 실제 SSD 명령 처리 로직을 여기에 구현
  std::cout << "Executing: " << command << std::endl;
  // 예시로만 구현 (실제로는 SsdInterface에 연결해야 함)
}

std::unordered_map<int, std::string> CommandBuffer::BuildLbaMapFromFilenames() {
  commands = GetCommandBuffer();
  std::unordered_map<int, std::string> lbaMap;

 for (const auto& filename : commands) {
    size_t underscorePos = filename.find('_');
    if (underscorePos == std::string::npos) continue;

    std::string indexPart = filename.substr(0, underscorePos);     // "1"
    std::string commandPart = filename.substr(underscorePos + 1);  // "W 5 AAA"

    std::istringstream iss(commandPart);
    std::string cmd;
    int lba;

    iss >> cmd >> lba;
    if (lba < 0 || lba > 99) continue;

    if (cmd == "W") {
      std::string value;
      iss >> value;
      lbaMap[lba] = value;
    } else if (cmd == "E") {
      int scope;
      iss >> scope;
      if (scope < -10 || scope > 10) continue;

      int start = lba;
      int end = lba + scope - 1;
      if (scope < 0) std::swap(start, end);

      for (int i = start; i <= end; ++i) {
        if (i >= 0 && i <= 99) {
          lbaMap[i] = "0x00000000";
        }
      }
    }
  }

  return lbaMap;
}

std::string CommandBuffer::Read(std::string lba_) {
  int lba = std::stoi(lba_);
  std::unordered_map<int, std::string> lbaMap = BuildLbaMapFromFilenames();
  auto it = lbaMap.find(lba);
  return it != lbaMap.end() ? it->second : FAIL_BUFFER_READ_MESSAGE;
}

std::vector<std::string> CommandBuffer::GetCommandBuffer() {
  std::vector<std::string> filenames;

  for (const auto& entry :
       std::filesystem::directory_iterator(bufferDirectory)) {
    if (entry.is_regular_file()) {
      filenames.push_back(entry.path().filename().string());
    }
  }

  return filenames;
}