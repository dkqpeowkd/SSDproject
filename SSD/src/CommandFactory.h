#pragma once
#include <memory>
#include "Command.h"


class CommandFactory {
 public:
  static std::unique_ptr<ICommand> Create(const std::string& commandStr);
};

