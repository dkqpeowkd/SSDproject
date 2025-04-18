#include "Script3.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <random>
#include <iomanip>

const std::string& Script3_WriteReadAging::getCommandString() {
    return command;
}

const std::string& Script3_WriteReadAging::getUsage() {
    return usage;
}

bool Script3_WriteReadAging::isValidArguments(const std::string& cmd, std::vector<std::string>& args) {
    return args.empty();
}

bool Script3_WriteReadAging::Execute(const string& cmd, vector<string>& args)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 0xFFFFFFFF);
    //200회 반복, LBA 0 / 99에 랜덤 값 Write 후 ReadCompare
    for (int loop = 0; loop < 200; ++loop) {
        std::ostringstream pattern1, pattern2;
        pattern1 << "0x" << std::uppercase << std::hex << std::setw(8) << std::setfill('0') << dist(gen);
        pattern2 << "0x" << std::uppercase << std::hex << std::setw(8) << std::setfill('0') << dist(gen);

        for (int lba : {0, 99}) {
            std::string pattern = (lba == 0 ? pattern1.str() : pattern2.str());

            std::vector<std::string> writeArgs = { std::to_string(lba), pattern };
            if (!writeCommand->Execute("write", writeArgs)) {
                std::cout << "[ERROR] Write failed at loop " << loop << ", LBA " << lba << std::endl;
                return false;
            }

            std::vector<std::string> readArgs = { std::to_string(lba) };
            if (!readCommand->Execute("read", readArgs)) {
                std::cout << "[ERROR] Read failed at loop " << loop << ", LBA " << lba << std::endl;
                return false;
            }

            std::ifstream fin("ssd_output.txt");
            std::string result;
            std::getline(fin, result);
            fin.close();

            if (result != pattern) {
                std::cout << "[FAIL] Loop " << loop << ", LBA " << lba
                    << " - Expected: " << pattern << ", Got: " << result << std::endl;
                return true;
            }
        }
    }

    std::cout << "PASS" << std::endl;
    return true;
}
