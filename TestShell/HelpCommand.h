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
    void logMessage(const std::string& msg, const std::string& msg2) const {
      if (log) log->log(msg, msg2);
    }

private:
    const string command = "help";
    const string usage = "Prompt>help\n";
    const string description = "Display Help for commands\n";
    string result = "";
    vector<shared_ptr<ICommand>> supportedCommand = {};
    Logger* log = nullptr;

    void printLines(const string& lines);
};

