#include "ScriptCommand.h"

const string& ScriptCommand::getCommandString()
{
    // TODO: ���⿡ return ���� �����մϴ�.
    return scriptCommand;
}

const string& ScriptCommand::getUsage()
{
    // TODO: ���⿡ return ���� �����մϴ�.
    return usage;
}

bool ScriptCommand::isValidArguments(const string& cmd, vector<string>& args)
{
    if (cmd != scriptCommand)
        return false;
    if (args.size() != numArgs)
        return false;
    return true;
}

bool ScriptCommand::Execute(const string& cmd, vector<string>& args)
{
    if (cmd != scriptCommand)
        return false;
    if (args.size() != 0)
        return false;

    return executeScript();
}

void ScriptCommand::addExecution(shared_ptr<ICommand> command, vector<string> args)
{
    scripts.emplace_back(command, args);
}

bool ScriptCommand::executeScript()
{
    for (auto cmd : scripts) {
        shared_ptr<ICommand> command = cmd.first;
        vector<string>& args = cmd.second;
        command->Execute(command->getCommandString(), args);
    }
    return false;
}


