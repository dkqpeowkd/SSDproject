#pragma once

#include "ICommand.h"
#include "Log.h"

interface ExitCommand : public ICommand {
    ExitCommand() = default;
    const string& getCommandString() override { return command;}
    bool isMatch(const string& command) override { return this->command == command; }
    const string& getUsage() override  { return usage; }
    const string& getDescription() override { return description; }
    bool isValidArguments(const string& cmd, vector<string>& args) { return args.size() == 0; }
    bool Execute(const string& cmd, vector<string>& args) {
        if (false == isValidArguments(cmd, args))
            return false;
        isActive = false;
        return true;
    }
    bool isSystemActive() { return isActive; }
    ExitCommand(Logger * logger) : log(logger) {}
    void logMessage(const std::string& msg, const std::string& msg2) const {
      if (log) log->log(msg, msg2);
    }

   private:
    const string command = "exit";
    const string usage = "Prompt>exit\n";
    const string description = "Exit the program\n";
    bool isActive = true;
    Logger* log;
};
