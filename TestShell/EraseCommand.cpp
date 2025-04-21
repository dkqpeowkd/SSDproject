#include "EraseCommand.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <algorithm>

const std::string& EraseCommand::getCommandString() {
    return cmd;
}

bool EraseCommand::isMatch(const string& command)
{
    return command == cmd;
}

const std::string& EraseCommand::getUsage() {
    static std::string usage = "erase <LBA> <SIZE> : 특정 LBA부터 SIZE만큼 삭제 (10칸씩 분할, 유효한 범위 먼저)";
    return usage;
}

bool EraseCommand::isValidArguments(const std::string& cmd, std::vector<std::string>& args) {
    return args.size() == 2;
}

bool EraseCommand::Execute(const std::string& cmd, std::vector<std::string>& args) {
    int lba = std::stoi(args[0]);
    int size = std::stoi(args[1]);

    // 음수 사이즈 처리
    if (size < 0) {
        lba = lba + size + 1;
        size = -size;
    }

    struct EraseCall {
        int lba;
        int size;
        bool isValid;
    };

    std::vector<EraseCall> validCalls;
    std::vector<EraseCall> invalidCalls;

    int processed = 0;
    while (processed < size) {
        int chunkSize = std::min(10, size - processed);
        int currentLBA = lba + processed;
        int chunkStart = currentLBA;
        int chunkEnd = currentLBA + chunkSize - 1;

        // valid 0 ~ 99
        int validStart = std::max(chunkStart, 0);
        int validEnd = std::min(chunkEnd, 99);
        int validSize = validEnd - validStart + 1;

        if (validSize > 0) {
            validCalls.push_back({ validStart, validSize, true });
        }

        // invalid 
        if (chunkStart < 0) {
            int invalidSize = std::min(chunkSize, 0 - chunkStart);
            invalidCalls.push_back({ chunkStart, invalidSize, false });
        }
        if (chunkEnd > 99) {
            int invalidStart = std::max(100, chunkStart);
            int invalidSize = chunkEnd - invalidStart + 1;
            if (invalidSize > 0) {
                invalidCalls.push_back({ invalidStart, invalidSize, false });
            }
        }

        processed += chunkSize;
    }

    // ✅ valid call first
    for (const auto& call : validCalls) {
        std::ostringstream oss;
        oss << "ssd.exe E " << call.lba << " " << call.size;
        std::cout << "[CALL] " << oss.str() << std::endl;

        if (callSystem(oss.str()) != 0) return false;
        if (readOutput() == "ERROR") return false;
    }

    // ❌ invalid call later
    for (const auto& call : invalidCalls) {
        std::ostringstream oss;
        oss << "ssd.exe E " << call.lba << " " << call.size;
        std::cout << "[CALL] " << oss.str() << std::endl;

        if (callSystem(oss.str()) != 0) return false;
        if (readOutput() == "ERROR") return false;
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
