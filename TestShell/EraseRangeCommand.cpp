#include "EraseRangeCommand.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

const std::string& EraseRangeCommand::getCommandString() {
    static std::string cmd = "erase_range";
    return cmd;
}

const std::string& EraseRangeCommand::getUsage() {
    static std::string usage = "erase_range <START_LBA> <END_LBA> : START부터 END까지 범위를 삭제합니다 (최대 10칸)";
    return usage;
}

bool EraseRangeCommand::isValidArguments(const std::string& cmd, std::vector<std::string>& args) {
    return args.size() == 2;
}

bool EraseRangeCommand::Execute(const std::string& cmd, std::vector<std::string>& args) {
    int start = std::stoi(args[0]);
    int end = std::stoi(args[1]);
    int size = end - start + 1;

    std::ostringstream oss;
    oss << "ssd.exe E " << start << " " << size;

    int result = callSystem(oss.str());

    std::string output = readOutput();
    return output != "ERROR";
}

int EraseRangeCommand::callSystem(const std::string& cmd) {
    return system(cmd.c_str());
}

std::string EraseRangeCommand::readOutput() {
    std::ifstream fin("ssd_output.txt");
    std::string line;
    std::getline(fin, line);
    return line;
}
