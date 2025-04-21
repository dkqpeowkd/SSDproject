#pragma once
#include "ICommand.h"
#include <string>
#include <vector>
#include "Log.h"

class EraseCommand : public ICommand {
public:
    EraseCommand() = default;

    const std::string& getCommandString() override;
    const std::string& getUsage() override;
    bool isMatch(const std::string& command) override;
    bool isValidArguments(const std::string& cmd, std::vector<std::string>& args) override;
    bool Execute(const std::string& cmd, std::vector<std::string>& args) override;
    EraseCommand(Logger* logger) : log(logger) {}
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
    const std::string cmd = "erase";

    struct EraseCall {
        int lba;
        int size;
    };

    std::vector<EraseCall> groupAndChunk(const std::vector<EraseCall>& calls);
    void performEraseCalls(const std::vector<EraseCall>& calls);
};
