#include <sstream>
#include <memory>
#include "CommandFactory.h"

std::unique_ptr<ICommand> CommandFactory::Create(
    const std::string& commandStr) {
  std::istringstream iss(commandStr);
  std::string type;
  int lba;
  iss >> type >> lba;

  if (type == "W") {
    std::string value;
    iss >> value;
    return std::make_unique<WriteCommand>(lba, value);
  }

  if (type == "E") {
    int size;
    iss >> size;
    return std::make_unique<EraseCommand>(lba, size);
  }

  return nullptr;
}