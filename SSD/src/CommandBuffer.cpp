#include "CommandBuffer.h"
#include "SsdType.h"
#include <sstream>
#include <algorithm>

CommandBuffer::CommandBuffer(const std::string& bufferDir)
    : bufferDirectory(bufferDir) {
  const bool bufferExists = std::filesystem::exists(bufferDirectory);

  if (bufferExists) {
    UpdateCommandsAndBuildLbaToValueList();
    return;
  } 
   
  InitializeEmptyCommandBuffer();
}

void CommandBuffer::InitializeEmptyCommandBuffer() {
  std::filesystem::create_directory(bufferDirectory);
  for (int i = 1; i <= MAX_BUFFER_SIZE; ++i) {
    std::ofstream file(bufferDirectory + "/" + std::to_string(i) + "_empty");
  }
}

void CommandBuffer::AddCommand(const std::string& command) {
  if (!IsWriteOrEraseCommand(command)) return;
  auto cmd = CommandFactory::Create(command);
  if (cmd) {
    commands.push_back(std::move(cmd));
  }
  SaveBuffer();

  std::map<int, std::string> lbaMap = UpdateCommandsAndBuildLbaToValueList();
  commands = ConvertLbaMapToBuffers(lbaMap);

  SaveBuffer();
}
std::vector<std::unique_ptr<ICommand>> CommandBuffer::ConvertLbaMapToBuffers(
    const std::map<int, std::string>& lbaMap) {
  std::vector<std::unique_ptr<ICommand>> commands;

  auto it = lbaMap.begin();
  while (it != lbaMap.end()) {
    if (it->second == "0x00000000") {
      int start = it->first;
      int count = 1;

      auto next = std::next(it);
      while (next != lbaMap.end() && next->first == it->first + 1) {
        ++count;
        ++it;
        ++next;
      }

      commands.push_back(std::make_unique<EraseCommand>(start, count));
      it = next;
    } else {
      ++it;
    }
  }

  // Write 명령어 추가
  for (const auto& [lba, value] : lbaMap) {
    if (value != "0x00000000") {
      commands.push_back(std::make_unique<WriteCommand>(lba, value));
    }
  }

  return commands;
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
          bufferDirectory + "/" + std::to_string(i + 1) + "_" + commands[i].get()->ToString();
    } else {
      newPath = bufferDirectory + "/" + std::to_string(i + 1) + "_empty";
    }

   std::ofstream createFile(newPath);
  }
}

void CommandBuffer::ClearBuffer() { 
  commands.clear();
  DestroyBuffer();
  InitializeEmptyCommandBuffer();
}

void CommandBuffer::DestroyBuffer() {  
  if (std::filesystem::exists(bufferDirectory)) {
    std::filesystem::remove_all(bufferDirectory);
  }
}

bool CommandBuffer::IsWriteOrEraseCommand(const std::string& command) const {
  return !command.empty() && (command[0] == 'W' || command[0] == 'E');
}

std::map<int, std::string> CommandBuffer::UpdateCommandsAndBuildLbaToValueList() {
  commands = GetCommandBuffer();
  return MakeLbaToValueListFromFileNames();
}

 std::map<int, std::string> CommandBuffer::MakeLbaToValueListFromFileNames() {
  std::map<int, std::string> lbaMap;

  for (const auto& command : commands) {
    command.get()->ApplyCommandBuffer(lbaMap);
  }

  return lbaMap;
 }

std::string CommandBuffer::Read(std::string lba_) {
  int lba = std::stoi(lba_);
  std::map<int, std::string> lbaMap = UpdateCommandsAndBuildLbaToValueList();
  auto it = lbaMap.find(lba);
  return it != lbaMap.end() ? it->second : FAIL_BUFFER_READ_MESSAGE;
}

std::vector<std::unique_ptr<ICommand>> CommandBuffer::GetCommandBuffer() {
  auto indexedCommands = MakeCommandsWithIndexFromFIleNames();
  return MakeCommandBufferFromIndexedCommands(indexedCommands);
}

std::vector<std::unique_ptr<ICommand>>
CommandBuffer::MakeCommandBufferFromIndexedCommands(
    const std::vector<std::pair<int, std::string>>& indexedCommands) {
  std::vector<std::unique_ptr<ICommand>> result;

  for (const auto& [_, cmdStr] : indexedCommands) {
    if (cmdStr.find("empty") != std::string::npos) continue;

    auto cmd = CommandFactory::Create(cmdStr);
    if (cmd) {
      result.push_back(std::move(cmd));
    }
  }

  return result;
}

std::vector<std::pair<int, std::string>>
CommandBuffer::MakeCommandsWithIndexFromFIleNames() {
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

  std::sort(indexedCommands.begin(), indexedCommands.end());

  return indexedCommands;
 }

int CommandBuffer::GetValidBufferCount() {
  int count = 0;
  const bool bufferExists = std::filesystem::exists(bufferDirectory);

  if (bufferExists==false) {
    std::filesystem::create_directories(bufferDirectory);
    return 0;
  }

  for (const auto& file :
       std::filesystem::directory_iterator(bufferDirectory)) {
    if (file.is_regular_file()==false) continue;

    std::string filename = file.path().filename().string();
    if (filename.find("_empty") == std::string::npos) {
      ++count;
    }
  }

  return count;
}