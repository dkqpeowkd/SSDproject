#include "FullWriteCommand.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>

const std::string& FullWriteCommand::getCommandString() {
    return cmd;
}

bool FullWriteCommand::isMatch(const string& command)
{
    return cmd == command;
}

const std::string& FullWriteCommand::getUsage() {
    static std::string usage = "fullwrite <PATTERN>";
    return usage;
}

const std::string& FullWriteCommand::getDescription()
{
    static std::string description = "전체 LBA 영역에 주어진 패턴을 저장합니다.";
    return description;
}

bool FullWriteCommand::isValidArguments(const std::string& cmd, std::vector<std::string>& args) {
    return args.size() == 1;
}

bool FullWriteCommand::Execute(const std::string& cmd, std::vector<std::string>& args) {
    for (int i = 0; i < 100; ++i) {
        std::ostringstream oss;
        oss << "ssd.exe W " << i << " " << args[0];
        logMessage("FullWriteCommand.Execute()", "[FULLWRITE] %s", oss.str().c_str());

        int result = callSystem(oss.str());
        if (result == 1) {
            logMessage("FullWriteCommand.Execute()", "[FULLWRITE] ERROR");
            continue;
        }
        std::string output = readOutput();
        logMessage("WriteCommand.Execute()", "[WRITE] COMPLETED!");
        return true;
    }
    return true;
}

int FullWriteCommand::callSystem(const std::string& cmd) {
    return system(cmd.c_str());
}

std::string FullWriteCommand::readOutput() {
    std::ifstream fin("ssd_output.txt");
    std::string line;
    std::getline(fin, line);
    return line;
}
