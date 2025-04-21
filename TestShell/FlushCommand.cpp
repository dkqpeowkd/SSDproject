#include "FlushCommand.h"
#include <cstdlib>

const std::string& FlushCommand::getCommandString() {
    return cmd;
}

bool FlushCommand::isMatch(const std::string& command) {
    return cmd == command;
}

const std::string& FlushCommand::getUsage() {
    static std::string usage = "flush";
    return usage;
}

const std::string& FlushCommand::getDescription()
{
    static std::string description = "Command Buffer에 있는 모든 명령어를 수행하여 비웁니다.";
    return description;
}

bool FlushCommand::isValidArguments(const std::string& cmd, std::vector<std::string>& args) {
    return args.empty();
}

bool FlushCommand::Execute(const std::string& cmd, std::vector<std::string>& args) {
    std::string command = "ssd.exe F";
    std::cout << "[FLUSH] " << command << std::endl;

    int result = callSystem(command);

    if (result == 1) {
        std::cout << "ERROR" << std::endl;
        return false;
    }

    std::cout << "FLUSH COMPLETED!" << std::endl;
    return true;
}

int FlushCommand::callSystem(const std::string& cmd) {
    return system(cmd.c_str());
}
