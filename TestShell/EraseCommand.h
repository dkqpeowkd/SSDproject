#pragma once

#include "ICommand.h"
#include <string>
#include <vector>

class EraseCommand : public ICommand {
public:
    EraseCommand() = default;

    const std::string& getCommandString() override;
    bool isMatch(const string& command) override;
    const std::string& getUsage() override;
    bool isValidArguments(const std::string& cmd, std::vector<std::string>& args) override;
    bool Execute(const std::string& cmd, std::vector<std::string>& args) override;

protected:
    virtual int callSystem(const std::string& cmd);
    virtual std::string readOutput();
private:
    const std::string cmd = "erase";
};
