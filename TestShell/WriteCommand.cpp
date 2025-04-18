#include "WriteCommand.h"
#include <iostream>
#include <sstream>
#include <regex>
#include <cstdlib>

const std::string& WriteCommand::getCommandString() {
    return command;
}

const std::string& WriteCommand::getUsage() {
    return usage;
}

bool WriteCommand::isValidArguments(const std::string& cmd, std::vector<std::string>& args) {
    if (args.size() != 2) return false;

    try {
        int lba = std::stoi(args[0]);
        if (lba < 0 || lba >= 100) return false;

        std::regex patternRegex("0x[0-9A-Fa-f]{8}");
        return std::regex_match(args[1], patternRegex);
    }
    catch (...) {
        return false;
    }
}

bool WriteCommand::Execute(const std::string& cmd, std::vector<std::string>& args) {
    std::ostringstream oss;
    oss << "ssd.exe write " << args[0] << " " << args[1];

    int result = system(oss.str().c_str());
    if (result != 0) {
        std::cout << "[ERROR] Failed to execute: " << oss.str() << std::endl;
        return false;
    }

    return true;
}
