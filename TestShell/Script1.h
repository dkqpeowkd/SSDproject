#pragma once

#include "ICommand.h"
#include <string>
#include <vector>

class Script1_FullWriteAndReadCompare : public ICommand {
public:
    const std::string& getCommandString() override;
    const std::string& getUsage() override;
    bool isValidArguments(const std::string& cmd, std::vector<std::string>& args) override;
    bool Execute(const std::string& cmd, std::vector<std::string>& args) override;
};
