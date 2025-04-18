#pragma once

#include "ICommand.h"
#include <string>
#include <vector>
#include "Log.h"

class FullWriteCommand : public ICommand {
public:
    FullWriteCommand() = default;

    const std::string& getCommandString() override;
    const std::string& getUsage() override;
    bool isValidArguments(const std::string& cmd, std::vector<std::string>& args) override;
    bool Execute(const std::string& cmd, std::vector<std::string>& args) override;
    FullWriteCommand(Logger* logger) : log(logger) {}
    void logMessage(const std::string& msg, const std::string& msg2) const {
      if (log) log->log(msg, msg2);
    }
     protected:
    virtual int callSystem(const std::string& cmd);
    virtual std::string readOutput();
    Logger* log;
    };
