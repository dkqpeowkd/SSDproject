#pragma once

#include "ICommand.h"
#include "Log.h"  // Logger Ŭ���� ����� ���� �ʿ�!
#include <string>
#include <vector>
#include "Log.h"

class EraseRangeCommand : public ICommand {
public:

    const std::string& getCommandString() override;
    bool isMatch(const std::string& command) override;
    const std::string& getUsage() override;
    const std::string& getDescription() override;
    bool isValidArguments(const std::string& cmd, std::vector<std::string>& args) override;
    bool Execute(const std::string& cmd, std::vector<std::string>& args) override;
    EraseRangeCommand(Logger* logger) : log(logger) {}
    void logMessage(const std::string& msg, const char* format, ...) const {
      if (log) {
        char buffer[1024];
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);

        log->log(msg, "%s", buffer);
      }
    };

protected:
    virtual int callSystem(const std::string& cmd);
    virtual std::string readOutput();
    Logger* log;

private:
    const std::string cmd = "erase_range";
};
