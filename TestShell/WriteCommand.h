#pragma once

#include "ICommand.h"
#include <string>
#include <vector>

class WriteCommand : public ICommand {
public:
    const std::string& getCommandString() override;
    const std::string& getUsage() override;
    bool isValidArguments(const std::string& cmd, std::vector<std::string>& args) override;
    bool Execute(const std::string& cmd, std::vector<std::string>& args) override;

private:
    const std::string command = "write";
    const std::string usage = "write <LBA> <PATTERN> : 해당 LBA에 패턴을 저장합니다.";
};
