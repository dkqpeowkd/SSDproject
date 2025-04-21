#pragma once

#include "ICommand.h"
#include "Log.h"

interface ExitCommand : public ICommand {
    ExitCommand() = default;
    const string& getCommandString() { return command;}
    bool isMatch(const string& command) override { return this->command == command; }
    const string& getUsage() { return usage; }
    bool isValidArguments(const string& cmd, vector<string>& args) { return args.size() == 0; }
    bool Execute(const string& cmd, vector<string>& args) {
        if (false == isValidArguments(cmd, args))
            return false;
        isActive = false;
        return true;
    }
    bool isSystemActive() { return isActive; }
    ExitCommand(Logger * logger) : log(logger) {}
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

   private:
    const string command = "exit";
    const string usage = "exit :\n\tExit the program\nusage:\n\tPrompt>exit";
    bool isActive = true;
    Logger* log;
};
