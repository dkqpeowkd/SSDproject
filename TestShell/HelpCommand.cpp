#include "HelpCommand.h"

const string& HelpCommand::getCommandString()
{
    // TODO: ���⿡ return ���� �����մϴ�.
    return command;
}

const string& HelpCommand::getUsage()
{
    // TODO: ���⿡ return ���� �����մϴ�.
    return usage;
}

bool HelpCommand::isValidArguments(const string& cmd, vector<string>& args)
{
    if (args.size() >= 0)
        return true;
    return false;
}

bool HelpCommand::Execute(const string& cmd, vector<string>& args)
{
    return true;
}
