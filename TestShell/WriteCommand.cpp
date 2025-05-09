#include "WriteCommand.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>

const std::string& WriteCommand::getCommandString() {
    return cmd;
}

bool WriteCommand::isMatch(const string& command)
{
    return cmd == command;
}

const std::string& WriteCommand::getUsage() {
    static std::string usage = "write <LBA> <PATTERN> : 해당 LBA에 패턴을 저장합니다.";
    return usage;
}

bool WriteCommand::isValidArguments(const std::string& cmd, std::vector<std::string>& args) {
    return args.size() == 2;
}

bool WriteCommand::Execute(const std::string& cmd, std::vector<std::string>& args) {
    std::ostringstream oss;
    oss << "ssd.exe W " << args[0] << " " << args[1];
    logMessage("WriteCommand.Execute()", "[WRITE] %s", oss.str().c_str());

    int result = callSystem(oss.str());

    if (result == 1) {
        logMessage("WriteCommand.Execute()", "[WRITE] ERROR");
        return false;
    }
    std::string output = readOutput();
    logMessage("WriteCommand.Execute()", "[WRITE] COMPLETED!");
    return true;
}

int WriteCommand::callSystem(const std::string& cmd) {
    return system(cmd.c_str());
}

std::string WriteCommand::readOutput() {
    std::ifstream fin("ssd_output.txt");
    std::string line;
    std::getline(fin, line);
    return line;
}
