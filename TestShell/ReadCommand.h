#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "ICommand.h"
#include "Log.h"


class ReadCommand : public ICommand {
public:
    ReadCommand(Logger* logger) : log(logger) {}
    const std::string& getCommandString() override;
    bool isMatch(const string& command) override;
    const std::string& getUsage() override;
    const std::string& getDescription() override;
    bool isValidArguments(const std::string& cmd, std::vector<std::string>& args) override;
    bool Execute(const std::string& cmd, std::vector<std::string>& args) override;
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
    Logger* log = nullptr;
private:
    const std::string cmd = "read";
};
