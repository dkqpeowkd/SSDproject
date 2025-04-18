#include "FullWriteCommand.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>

const std::string& FullWriteCommand::getCommandString() {
    static std::string cmd = "fullwrite";
    return cmd;
}

const std::string& FullWriteCommand::getUsage() {
    static std::string usage = "fullwrite <PATTERN> : 전체 LBA 영역에 주어진 패턴을 저장합니다.";
    return usage;
}

bool FullWriteCommand::isValidArguments(const std::string& cmd, std::vector<std::string>& args) {
    return args.size() == 1;
}

bool FullWriteCommand::Execute(const std::string& cmd, std::vector<std::string>& args) {
    for (int i = 0; i < 100; ++i) {
        std::ostringstream oss;
        oss << "ssd.exe W " << i << " " << args[0];
        int result = callSystem(oss.str());

        std::string output = readOutput();
        if (output == "ERROR") {
            return false;
        }
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
