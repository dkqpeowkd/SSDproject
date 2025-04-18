#include "Script1.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <cstdlib>

const std::string& Script1_FullWriteAndReadCompare::getCommandString() {
    static std::string command = "1_FullWriteAndReadCompare";
    return command;
}

const std::string& Script1_FullWriteAndReadCompare::getUsage() {
    static std::string usage = "1_FullWriteAndReadCompare: 전체 LBA 영역을 5개씩 나눠 Write/ReadCompare 수행";
    return usage;
}

bool Script1_FullWriteAndReadCompare::isValidArguments(const std::string& cmd, std::vector<std::string>& args) {
    return args.empty();
}

bool Script1_FullWriteAndReadCompare::Execute(const std::string& cmd, std::vector<std::string>& args) {
    for (int group = 0; group < 100; group += 5) {
        // 패턴 생성
        std::stringstream ss;
        ss << "0x" << std::hex << std::setw(8) << std::setfill('0') << (0x11111111 + group);
        std::string pattern = ss.str();

        // Write
        for (int lba = group; lba < group + 5 && lba < 100; ++lba) {
            std::ostringstream writeCmd;
            writeCmd << "ssd.exe W " << lba << " " << pattern;
            system(writeCmd.str().c_str());
        }

        // Read + Compare
        for (int lba = group; lba < group + 5 && lba < 100; ++lba) {
            std::ostringstream readCmd;
            readCmd << "ssd.exe R " << lba;
            system(readCmd.str().c_str());

            std::ifstream fin("ssd_output.txt");
            std::string result;
            std::getline(fin, result);
            fin.close();

            if (result != pattern) {
                std::cout << "[FAIL] LBA " << lba << " - Expected: " << pattern << ", Got: " << result << std::endl;
                return true;
            }
        }
    }
    std::cout << "PASS" << std::endl;
    return true;
}
