#pragma once

#include "ICommand.h"
#include <string>
#include <vector>

class FullReadCommand : public ICommand {
public:
    FullReadCommand() = default;

    const std::string& getCommandString() override;
    const std::string& getUsage() override;
    bool isValidArguments(const std::string& cmd, std::vector<std::string>& args) override;
    bool Execute(const std::string& cmd, std::vector<std::string>& args) override;

protected:
    virtual int callSystem(const std::string& cmd);
    virtual std::string readOutput();
};
