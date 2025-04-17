#include "HelpCommand.h"

const string& HelpCommand::getCommandString()
{
    // TODO: 여기에 return 문을 삽입합니다.
    return command;
}

const string& HelpCommand::getUsage()
{
    // TODO: 여기에 return 문을 삽입합니다.
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
