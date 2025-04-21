#include "CommandBuffer.h"
#include "SsdType.h"
#include <sstream>
#include <algorithm>

CommandBuffer::CommandBuffer(const std::string& bufferDir)
    : bufferDirectory(bufferDir) {
  if (!std::filesystem::exists(bufferDirectory)) {
    Init();
  } else {
    BuildLbaMapFromFilenames();
  }

}

void CommandBuffer::Init() {
  std::filesystem::create_directory(bufferDirectory);
  for (int i = 1; i <= MAX_BUFFER_SIZE; ++i) {
    std::ofstream file(bufferDirectory + "/" + std::to_string(i) + "_empty");
  }
}

void CommandBuffer::AddCommand(const std::string& command) {
  if (!IsWriteOrEraseCommand(command)) return;

  if (command[0] == 'W') {
    processWrite(command);
  } else if (command[0] == 'E') {
    processErase(command);
  }

  SaveBuffer();

  if (GetValidBufferCount() >= MAX_BUFFER_SIZE) {
    ClearBuffer();
  }

  commands.push_back(command);

  SaveBuffer();
}

void CommandBuffer::processWrite(const std::string& command) {
  std::istringstream iss(command);
  std::string type;
  int lba;
  std::string value;

  iss >> type >> lba >> value;

  commands.erase(std::remove_if(commands.begin(), commands.end(),
                                [lba](const std::string& cmd) {
                                  std::istringstream iss(cmd);
                                  std::string t;
                                  int l;
                                  iss >> t >> l;
                                  return t == "W" && l == lba;
                                }),
                 commands.end());

  
}

void CommandBuffer::processErase(const std::string& command) {}

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
  commands.clear();
  DestroyBuffer();
  Init();
}

void CommandBuffer::DestroyBuffer() {
  
  if (std::filesystem::exists(bufferDirectory)) {
    std::filesystem::remove_all(bufferDirectory);
  }
}

bool CommandBuffer::IsWriteOrEraseCommand(const std::string& command) const {
  return !command.empty() && (command[0] == 'W' || command[0] == 'E');
}

std::unordered_map<int, std::string> CommandBuffer::BuildLbaMapFromFilenames() {
  commands = GetCommandBuffer();
  std::unordered_map<int, std::string> lbaMap;

 for (const auto& command : commands) {

    std::istringstream iss(command);
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
  std::vector<std::pair<int, std::string>> indexedCommands;

  for (const auto& entry :
       std::filesystem::directory_iterator(bufferDirectory)) {
    if (!entry.is_regular_file()) continue;

    std::string filename = entry.path().filename().string();
    size_t underscorePos = filename.find('_');
    if (underscorePos == std::string::npos) continue;

    std::string indexStr = filename.substr(0, underscorePos);
    std::string commandStr = filename.substr(underscorePos + 1);

    if (!std::all_of(indexStr.begin(), indexStr.end(), ::isdigit)) continue;

    int index = std::stoi(indexStr);
    indexedCommands.emplace_back(index, commandStr);
  }

  // 인덱스 기준 오름차순 정렬
  std::sort(indexedCommands.begin(), indexedCommands.end());

  // 정리된 커맨드만 추출
  std::vector<std::string> result;
  for (const auto& [_, cmd] : indexedCommands) {
    if (cmd.find("empty") == std::string::npos) {
    result.push_back(cmd);
    }
  }

  return result;
}

int CommandBuffer::GetValidBufferCount() {
  int count = 0;

    if (!std::filesystem::exists(bufferDirectory)) {
    std::filesystem::create_directories(bufferDirectory);
  }

  for (const auto& entry :
       std::filesystem::directory_iterator(bufferDirectory)) {
    if (!entry.is_regular_file()) continue;

    std::string filename = entry.path().filename().string();
    if (filename.find("_empty") == std::string::npos) {
      ++count;
    }
  }

  return count;
}