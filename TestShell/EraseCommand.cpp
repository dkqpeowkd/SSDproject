#include "EraseCommand.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cmath>

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
    int lba = std::stoi(args[0]);
    int size = std::stoi(args[1]);

    // SIZE가 음수면: 역방향 보정
    if (size < 0) {
        lba = lba + size + 1; // 시작 위치 보정
        size = -size;         // 양수로 변환
    }

    int processed = 0;
    while (processed < size) {
        int chunkSize = std::min(10, size - processed);
        int currentLBA = lba + processed;

        std::ostringstream oss;
        oss << "ssd.exe E " << currentLBA << " " << chunkSize;
        int result = callSystem(oss.str());
        std::string output = readOutput();

        if (output == "ERROR") return false;

        processed += chunkSize;
    }

    return true;
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
