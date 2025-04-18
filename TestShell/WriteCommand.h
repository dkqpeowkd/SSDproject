#pragma once

#include "ICommand.h"
#include <string>
#include <vector>
#include "Log.h"

class WriteCommand : public ICommand {
public:
    WriteCommand() = default;
    const std::string& getCommandString() override;
    const std::string& getUsage() override;
    bool isValidArguments(const std::string& cmd, std::vector<std::string>& args) override;
    bool Execute(const std::string& cmd, std::vector<std::string>& args) override;
    WriteCommand(Logger* logger) : log(logger) {}
    void logMessage(const std::string& msg, const std::string& msg2) const {
      if (log) log->log(msg, msg2);
    }

   private:
    const std::string command = "write";
    const std::string usage = "write <LBA> <PATTERN> : 해당 LBA에 패턴을 저장합니다.";
    Logger* log;
};

