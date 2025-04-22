#include "EraseRangeCommand.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <algorithm>

const std::string& EraseRangeCommand::getCommandString() {
    return cmd;
}

bool EraseRangeCommand::isMatch(const string& command)
{
    return cmd == command;
}

const std::string& EraseRangeCommand::getUsage() {
    static std::string usage = "erase_range <START_LBA> <END_LBA>";
    return usage;
}

const std::string& EraseRangeCommand::getDescription()
{
    static std::string description = "주어진 범위를 삭제";
    return description;
}

bool EraseRangeCommand::isValidArguments(const std::string& cmd, std::vector<std::string>& args) {
    return args.size() == 2;
}

bool EraseRangeCommand::Execute(const std::string& cmd, std::vector<std::string>& args) {
    int startLBA = std::stoi(args[0]);
    int endLBA = std::stoi(args[1]);

    if (startLBA < 0 || endLBA < 0 || startLBA >= 100 || endLBA >= 100) {
        std::cout << "ERROR" << std::endl;
        return true;
    }

    if (startLBA > endLBA)
        std::swap(startLBA, endLBA);

    int totalSize = endLBA - startLBA + 1;

    for (int i = 0; i < totalSize; i += 10) {
        int chunkSize = std::min(10, totalSize - i);
        int lba = startLBA + i;

        std::ostringstream oss;
        oss << "ssd.exe E " << lba << " " << chunkSize;
        logMessage("EraseRangeCommand.Execute()", "[ERASE] %s", oss.str().c_str());

        int result = callSystem(oss.str());

        if (result == 1) {
            logMessage("EraseRangeCommand.Execute()", "[ERASE] ERROR");
        }
        else {
            logMessage("EraseRangeCommand.Execute()", "[ERASE] COMPLETED!");
        }
    }

    return true;
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
