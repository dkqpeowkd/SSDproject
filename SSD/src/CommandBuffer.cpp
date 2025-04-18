#include "CommandBuffer.h"

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
  ClearBuffer();

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