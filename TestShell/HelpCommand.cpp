#include <iostream>
#include "HelpCommand.h"

using std::shared_ptr;

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
    std::cout << "Developed by: Team BestAttitude (Changwan.Ha, Hyunkoo.kim  Hyoeun.Park  Chanwook.Lee  Ted.Kim)\n";
    for (auto help : cmdHelp)
        std::cout << help << "\n";
    return true;
}

void HelpCommand::addHelp(string help)
{
    cmdHelp.emplace_back(help);
}
