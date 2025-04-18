#pragma once
#include <memory>
#include "ICommand.h"
#include "Log.h"

using std::shared_ptr;

class HelpCommand : public ICommand {
public:
    HelpCommand() = default;
    const string& getCommandString() override;
    const string& getUsage() override;
    bool isValidArguments(const string& cmd, vector<string>& args) override;
    bool Execute(const string& cmd, vector<string>& args) override;
    void addHelp(string help);
    HelpCommand(Logger* logger) : log(logger) {}
    void logMessage(const std::string& msg, const std::string& msg2) const {
      if (log) log->log(msg, msg2);
    }

   private:
    const string command = "help";
    const string usage = "help :\n\tDisplay Help for commands\nusage:\n\tPrompt>help";
    string result = "";
    vector<string> cmdHelp;
    Logger* log;
};

