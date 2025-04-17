#pragma once
#include <memory>
#include "ICommand.h"

using std::shared_ptr;

class HelpCommand : public ICommand {
public:
    HelpCommand() {}
    const string& getCommandString() override;
    const string& getUsage() override;
    bool isValidArguments(const string& cmd, vector<string>& args) override;
    bool Execute(const string& cmd, vector<string>& args) override;
    void addHelp(string help);
private:
    const string command = "help";
    const string usage = "help :\n\tDisplay Help for commands\nusage:\n\tPrompt>help";
    string result = "";
    vector<string> cmdHelp;
};

