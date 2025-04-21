#include "EraseCommand.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <cmath>

const std::string& EraseCommand::getCommandString() {
    return cmd;
}

bool EraseCommand::isMatch(const std::string& command) {
    return cmd == command;
}

const std::string& EraseCommand::getUsage() {
    static std::string usage = "erase <LBA> <SIZE> : 지정된 영역을 삭제합니다 (최대 10칸 단위로 분할 전송)";
    return usage;
}

bool EraseCommand::isValidArguments(const std::string& cmd, std::vector<std::string>& args) {
    return args.size() == 2;
}

bool EraseCommand::Execute(const std::string& cmd, std::vector<std::string>& args) {
    int lba = std::stoi(args[0]);
    int size = std::stoi(args[1]);

    if (size == 0) return true;

    // 음수 size 처리 (역방향 삭제)
    if (size < 0) {
        lba = lba + size + 1;
        size = std::abs(size);
    }

    // 10칸 단위로 나눠서 모든 LBA 수집
    std::vector<EraseCall> calls;
    for (int i = 0; i < size; ++i) {
        calls.push_back({ lba + i, 1 });
    }

    // 유효 / 무효 LBA 구분
    std::vector<EraseCall> valid, invalid;
    for (const auto& call : calls) {
        if (call.lba >= 0 && call.lba < 100)
            valid.push_back(call);
        else
            invalid.push_back(call);
    }

    // 그룹핑 + 10단위 쪼개기
    auto chunkedValid = groupAndChunk(valid);
    auto chunkedInvalid = groupAndChunk(invalid);

    // system call
    performEraseCalls(chunkedValid);
    performEraseCalls(chunkedInvalid);

    return true;
}

std::vector<EraseCommand::EraseCall> EraseCommand::groupAndChunk(const std::vector<EraseCall>& calls) {
    std::vector<EraseCall> result;
    if (calls.empty()) return result;

    // 정렬
    std::vector<EraseCall> sorted = calls;
    std::sort(sorted.begin(), sorted.end(), [](const EraseCall& a, const EraseCall& b) {
        return a.lba < b.lba;
        });

    // 연속된 LBA 그룹핑
    std::vector<EraseCall> grouped;
    grouped.push_back(sorted[0]);
    for (size_t i = 1; i < sorted.size(); ++i) {
        EraseCall& last = grouped.back();
        if (last.lba + last.size == sorted[i].lba) {
            last.size++;
        }
        else {
            grouped.push_back(sorted[i]);
        }
    }

    // 그룹을 10칸 단위로 나눔
    for (const auto& group : grouped) {
        int start = group.lba;
        int remain = group.size;
        while (remain > 0) {
            int chunkSize = std::min(10, remain);
            result.push_back({ start, chunkSize });
            start += chunkSize;
            remain -= chunkSize;
        }
    }

    return result;
}

void EraseCommand::performEraseCalls(const std::vector<EraseCall>& calls) {
    for (const auto& call : calls) {
        std::ostringstream oss;
        oss << "ssd.exe E " << call.lba << " " << call.size;
        std::cout << "[ERASE] " << oss.str() << std::endl;

        int result = callSystem(oss.str());
        if (result == 1) {
            std::cout << "ERROR" << std::endl;
        }
        else {
            std::cout << "DELETED" << std::endl;
        }
    }
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
