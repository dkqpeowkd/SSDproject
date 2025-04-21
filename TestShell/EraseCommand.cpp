#include "EraseCommand.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>

// 구조체: Erase 호출 단위를 나타냄
struct EraseCall {
    int lba;
    int size;
};

const std::string& EraseCommand::getCommandString() {
    return cmd;
}

bool EraseCommand::isMatch(const string& command)
{
    return cmd == command;
}

const std::string& EraseCommand::getUsage() {
    static std::string usage = "erase <LBA> <SIZE> : 지정된 영역을 삭제합니다 (최대 10칸 단위로 분할 전송)";
    return usage;
}

// 명령어 인자 수 검증 (입력값 유효성은 SSD가 판단)
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

    // 10칸 단위로 분할
    std::vector<EraseCall> chunks;
    for (int i = 0; i < size; i += 10) {
        int chunkSize = std::min(10, size - i);
        chunks.push_back({ lba + i, chunkSize });
    }

    std::vector<EraseCall> validCalls;
    std::vector<EraseCall> invalidCalls;

    // 각 청크 내에서 유효 / 무효 LBA를 1칸씩 분할
    for (const auto& chunk : chunks) {
        for (int i = 0; i < chunk.size; ++i) {
            int curr = chunk.lba + i;
            if (curr >= 0 && curr < 100) {
                validCalls.push_back({ curr, 1 });
            }
            else {
                invalidCalls.push_back({ curr, 1 });
            }
        }
    }

    // 유효 LBA 정렬 및 연속된 LBA 그룹핑
    std::sort(validCalls.begin(), validCalls.end(), [](const EraseCall& a, const EraseCall& b) {
        return a.lba < b.lba;
        });

    std::vector<EraseCall> grouped;
    for (const auto& call : validCalls) {
        if (grouped.empty()) {
            grouped.push_back(call);
        }
        else {
            EraseCall& last = grouped.back();
            if (last.lba + last.size == call.lba) {
                last.size++;
            }
            else {
                grouped.push_back(call);
            }
        }
    }

    // 그룹핑된 유효 LBA를 다시 10칸 단위로 쪼갬
    std::vector<EraseCall> chunkedValidCalls;
    for (const auto& group : grouped) {
        int start = group.lba;
        int remaining = group.size;
        while (remaining > 0) {
            int chunkSize = std::min(10, remaining);
            chunkedValidCalls.push_back({ start, chunkSize });
            start += chunkSize;
            remaining -= chunkSize;
        }
    }

    // 무효 LBA도 정렬 + 그룹핑
    std::sort(invalidCalls.begin(), invalidCalls.end(), [](const EraseCall& a, const EraseCall& b) {
        return a.lba < b.lba;
        });

    std::vector<EraseCall> groupedInvalid;
    for (const auto& call : invalidCalls) {
        if (groupedInvalid.empty()) {
            groupedInvalid.push_back(call);
        }
        else {
            EraseCall& last = groupedInvalid.back();
            if (last.lba + last.size == call.lba) {
                last.size++;
            }
            else {
                groupedInvalid.push_back(call);
            }
        }
    }

    std::vector<EraseCall> chunkedInvalidCalls;
    for (const auto& group : groupedInvalid) {
        int start = group.lba;
        int remaining = group.size;
        while (remaining > 0) {
            int chunkSize = std::min(10, remaining);
            chunkedInvalidCalls.push_back({ start, chunkSize });
            start += chunkSize;
            remaining -= chunkSize;
        }
    }

    // 유효 LBA system call 수행
    for (const auto& call : chunkedValidCalls) {
        std::ostringstream oss;
        oss << "ssd.exe E " << call.lba << " " << call.size;
        std::cout << "[ERASE] " << oss.str() << std::endl;
        int result = callSystem(oss.str());

        if (result == 1) {
            std::cout << "DELETED" << std::endl;
        }
        else {
            std::cout << "ERROR" << std::endl;
        }
    }

    // 무효 LBA도 그대로 system call
    for (const auto& call : chunkedInvalidCalls) {
        std::ostringstream oss;
        oss << "ssd.exe E " << call.lba << " " << call.size;
        std::cout << "[ERASE] " << oss.str() << std::endl;
        int result = callSystem(oss.str());
        if (result == 1) {
            std::cout << "DELETED" << std::endl;
        }
        else {
            std::cout << "ERROR" << std::endl;
        }
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
