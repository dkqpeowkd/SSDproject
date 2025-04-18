#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "ICommand.h"
#include "Log.h"

class ReadCommand : public ICommand {
public:
    ReadCommand() = default;

    const std::string& getCommandString() override;
    const std::string& getUsage() override;
    bool isValidArguments(const std::string& cmd, std::vector<std::string>& args) override;
    bool Execute(const std::string& cmd, std::vector<std::string>& args) override;
    ReadCommand(Logger* logger) : log(logger) {}
    void logMessage(const std::string& msg, const std::string& msg2) const {
      if (log) log->log(msg, msg2);
    }

   protected:
    virtual int callSystem(const std::string& cmd);
    virtual std::string readOutput();
    Logger* log;
};
