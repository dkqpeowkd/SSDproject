#include "FullReadCommand.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

const std::string& FullReadCommand::getCommandString() {
    return cmd;
}

bool FullReadCommand::isMatch(const string& command)
{
    return cmd == command;
}

const std::string& FullReadCommand::getUsage() {
    static std::string usage = "fullread";
    return usage;
}

const std::string& FullReadCommand::getDescription()
{
    static std::string description = "전체 LBA 영역을 읽고 출력합니다.";
    return description;
}

bool FullReadCommand::isValidArguments(const std::string& cmd, std::vector<std::string>& args) {
    return args.empty();
}

bool FullReadCommand::Execute(const std::string& cmd, std::vector<std::string>& args) {
    for (int i = 0; i < 100; ++i) {
        std::ostringstream oss;
        oss << "ssd.exe R " << i;
        logMessage("FullReadCommand.Execute()", "[FULLREAD] %s", oss.str().c_str());

        int result = callSystem(oss.str());

        if (result == 1) {
            logMessage("FullReadCommand.Execute()", "[FULLREAD] ERROR");
            continue;
        }

        std::string output = readOutput();
        logMessage("FullReadCommand.Execute()", "[FULLREAD] %s", output.c_str());
    }

    return true;
}

int FullReadCommand::callSystem(const std::string& cmd) {
    return system(cmd.c_str());
}

std::string FullReadCommand::readOutput() {
    std::ifstream fin("ssd_output.txt");
    std::string line;
    std::getline(fin, line);
    return line;
}
