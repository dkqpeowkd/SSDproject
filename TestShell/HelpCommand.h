#pragma once
#include <memory>
#include "ICommand.h"
#include "Log.h"

using std::shared_ptr;

class HelpCommand : public ICommand {
public:
    HelpCommand() {}
    HelpCommand(Logger* log) : log{ log } {}
    const string& getCommandString() override;
    bool isMatch(const string& command) override;
    const string& getUsage() override;
    const string& getDescription() override;
    bool isValidArguments(const string& cmd, vector<string>& args) override;
    bool Execute(const string& cmd, vector<string>& args) override;
    void addSupportedCommand(vector<shared_ptr<ICommand>> supported);
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
    const string command = "help";
    const string usage = "help\n";
    const string description = "Display Help for commands\n";
    string result = "";
    vector<shared_ptr<ICommand>> supportedCommand = {};
    Logger* log = nullptr;

    void printLines(const string& lines);
};

