#pragma once

#include "ICommand.h"
#include "TestShell.h"
#include "PromptInput.h"


using std::pair;
class ScriptCommand;

class ScriptCommand : public ICommand {
public:
    ScriptCommand(const string& command, const string& usage) : scriptCommand{command}, usage{usage} {}
    const string& getCommandString() override;
    const string& getUsage() override;
    bool isValidArguments(const string& cmd, vector<string>& args) override;
    bool Execute(const string& cmd, vector<string>& args) override;
    void addExecution(shared_ptr<ICommand> command, vector<string> args);
    bool executeScript();
private:
    string scriptCommand = "";
    string usage = "";
    unsigned int numArgs = 0;
    vector<string> commandLines;
    vector<pair<shared_ptr<ICommand>, vector<string>>> scripts = {};
};

