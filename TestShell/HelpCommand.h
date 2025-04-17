#pragma once
#include "ICommand.h"
class HelpCommand : public ICommand {
    const string& getCommandString() override;
    const string& getUsage() override;
    bool isValidArguments(const string& cmd, vector<string>& args) override;
    bool Execute(const string& cmd, vector<string>& args) override;

private:
    const string command = "help";
    const string usage = "";
    string result = "";
};

