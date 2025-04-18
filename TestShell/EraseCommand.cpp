#include "EraseCommand.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

const std::string& EraseCommand::getCommandString() {
    static std::string cmd = "erase";
    return cmd;
}

const std::string& EraseCommand::getUsage() {
    static std::string usage = "erase <LBA> <SIZE> : 특정 LBA부터 SIZE만큼 삭제합니다 (최대 10칸)";
    return usage;
}

bool EraseCommand::isValidArguments(const std::string& cmd, std::vector<std::string>& args) {
    return args.size() == 2;
}

bool EraseCommand::Execute(const std::string& cmd, std::vector<std::string>& args) {
    std::ostringstream oss;
    oss << "ssd.exe E " << args[0] << " " << args[1];

    int result = callSystem(oss.str());

    std::string output = readOutput();
    return output != "ERROR";
}

int EraseCommand::callSystem(const std::string& cmd) {
    return system(cmd.c_str());
}

std::string EraseCommand::readOutput() {
    std::ifstream fin("ssd_output.txt");
    std::string line;
    std::getline(fin, line);
    return line;
}
