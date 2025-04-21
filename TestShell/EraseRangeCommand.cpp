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
    static std::string usage = "erase_range <START_LBA> <END_LBA> : 해당 범위 전체 삭제 (10칸 단위, 유효 영역 우선)";
    return usage;
}

bool EraseRangeCommand::isValidArguments(const std::string& cmd, std::vector<std::string>& args) {
    return args.size() == 2;
}

bool EraseRangeCommand::Execute(const std::string& cmd, std::vector<std::string>& args) {
    int start = std::stoi(args[0]);
    int end = std::stoi(args[1]);

    if (start > end) std::swap(start, end);

    int totalSize = end - start + 1;

    struct EraseCall {
        int lba;
        int size;
        bool isValid;
    };

    std::vector<EraseCall> validCalls;
    std::vector<EraseCall> invalidCalls;

    int processed = 0;
    while (processed < totalSize) {
        int chunkSize = std::min(10, totalSize - processed);
        int currentLBA = start + processed;
        int chunkStart = currentLBA;
        int chunkEnd = currentLBA + chunkSize - 1;

        // valid 0 ~ 99
        int validStart = std::max(chunkStart, 0);
        int validEnd = std::min(chunkEnd, 99);
        int validSize = validEnd - validStart + 1;

        if (validSize > 0) {
            validCalls.push_back({ validStart, validSize, true });
        }

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

    // ❌ 무효한 영역 나중 system call
    for (const auto& call : invalidCalls) {
        std::ostringstream oss;
        oss << "ssd.exe E " << call.lba << " " << call.size;
        std::cout << "[CALL] " << oss.str() << std::endl;

        if (callSystem(oss.str()) != 0) return false;
        if (readOutput() == "ERROR") return false;
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
