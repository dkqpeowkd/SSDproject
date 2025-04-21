#include "ScriptCommand.h"

bool ScriptCommand::isMatch(const string& cmd)
{
    if (cmd == scriptCommand)
        return true;
    if (scriptCommand.substr(0, 2) == cmd)
        return true;
    return false;
}

const string& ScriptCommand::getCommandString()
{
    return scriptCommand;
}

const string& ScriptCommand::getUsage()
{
    if (usage.length() == 0)
        return NOT_PROVIDED;
    return usage;
}

const std::string& ScriptCommand::getDescription()
{
    if (description.length() == 0)
        return NOT_PROVIDED;
    return description;
}

bool ScriptCommand::isValidArguments(const string& cmd, vector<string>& args)
{
    if (isMatch(cmd) == false)
        return false;
    return true;
}

bool ScriptCommand::Execute(const string& cmd, vector<string>& args)
{
    if (isValidArguments(cmd, args) == false)
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
        if (false == command->Execute(command->getCommandString(), args)) {
            //std::cout << "Failed to execute " << scriptCommand << "->" << command->getCommandString() << std::endl;
            return false;
        }
    }
    return true;
}