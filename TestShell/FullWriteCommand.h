#pragma once

#include "ICommand.h"
#include <string>
#include <vector>
#include "Log.h"

class FullWriteCommand : public ICommand {
public:
    FullWriteCommand() = default;

    const std::string& getCommandString() override;
    bool isMatch(const string& command) override;
    const std::string& getUsage() override;
    bool isValidArguments(const std::string& cmd, std::vector<std::string>& args) override;
    bool Execute(const std::string& cmd, std::vector<std::string>& args) override;
    FullWriteCommand(Logger* logger) : log(logger) {}
    void logMessage(const std::string& msg, const char* format, ...) const {
      if (log) {
        char buffer[1024];
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);

        log->log(msg, "%s", buffer);
      }
    }
     protected:
    virtual int callSystem(const std::string& cmd);
    virtual std::string readOutput();
    Logger* log;
private:
    const std::string cmd = "fullwrite";
};
