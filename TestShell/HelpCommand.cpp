#include <iostream>
#include "HelpCommand.h"

using std::shared_ptr;

const string& HelpCommand::getCommandString()
{
    // TODO: 여기에 return 문을 삽입합니다.
    return command;
}

bool HelpCommand::isMatch(const string& command)
{
    return this->command == command;
}

const string& HelpCommand::getUsage()
{
    return usage;
}

const string& HelpCommand::getDescription()
{
    return description;
}

bool HelpCommand::isValidArguments(const string& cmd, vector<string>& args)
{
    if (args.size() >= 0)
        return true;
    return false;
}

bool HelpCommand::Execute(const string& cmd, vector<string>& args)
{
    std::cout << "Developed by: Team BestAttitude (Changwan.Ha, Hyunkoo.kim  Hyoeun.Park  Chanwook.Lee  Ted.Kim)\n\n";
    std::cout << "-- Command Help ---------------------------------------\n";
    for (auto& command : supportedCommand) {
        std::cout << "[" << command->getCommandString() << "]\n";
        std::cout << "Description:\n";
        //std::cout << "\t" << command->getDescription() << "\n";
        printLines(command->getDescription());
        std::cout << "Usage:\n";
        //std::cout << "\t" << command->getUsage() << "\n";
        printLines(command->getUsage());
        std::cout << "\n\n";
    }
    return true;
}

void HelpCommand::addSupportedCommand(vector<shared_ptr<ICommand>> supported)
{
    supportedCommand = supported;
}

void HelpCommand::printLines(const string& lines)
{
    std::string line;
    std::istringstream iss(lines);

    while (std::getline(iss, line)) {
        std::cout << '\t' << line << "\n";
    }
}