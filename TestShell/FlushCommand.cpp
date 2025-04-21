#include "FlushCommand.h"
#include <cstdlib>

const std::string& FlushCommand::getCommandString() {
    return cmd;
}

bool FlushCommand::isMatch(const std::string& command) {
    return cmd == command;
}

const std::string& FlushCommand::getUsage() {
    static std::string usage = "flush : Command Buffer에 있는 모든 명령어를 수행하여 비웁니다.";
    return usage;
}

bool FlushCommand::isValidArguments(const std::string& cmd, std::vector<std::string>& args) {
    return args.empty();
}

bool FlushCommand::Execute(const std::string& cmd, std::vector<std::string>& args) {

    std::string command = "ssd.exe F";
    logMessage("FlushCommand.Execute()", "[FLUSH] %s", command.c_str());

    int result = callSystem(command);
    if (result == 1) {
        logMessage("FlushCommand.Execute()", "[FLUSH] ERROR");
        return false;
    }

    logMessage("FlushCommand.Execute()", "[FLUSH] COMPLETED!");
    return true;

}

int FlushCommand::callSystem(const std::string& cmd) {
    return system(cmd.c_str());
}
