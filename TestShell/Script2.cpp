#include "Script2.h"
#include <iostream>
#include <sstream>
#include <fstream>

const std::string& Script2_PartialLBAWrite::getCommandString() {
    return command;
}

const std::string& Script2_PartialLBAWrite::getUsage() {
    return usage;
}

bool Script2_PartialLBAWrite::isValidArguments(const std::string& cmd, std::vector<std::string>& args) {
    return (cmd == "2_" || cmd == command) && args.empty();
}

bool Script2_PartialLBAWrite::Execute(const std::string& cmd, std::vector<std::string>& args) {
    std::string pattern = "0xABCD1234";
    std::vector<int> lbas = { 4, 0, 3, 1, 2 };

    for (int loop = 0; loop < 30; ++loop) {
        for (int lba : lbas) {
            std::vector<std::string> writeArgs = { std::to_string(lba), pattern };
            if (!writeCommand->Execute("write", writeArgs)) {
                std::cout << "[ERROR] Write failed at loop " << loop << ", LBA " << lba << std::endl;
                return false;
            }
        }

        for (int lba : lbas) {
            std::vector<std::string> readArgs = { std::to_string(lba) };
            if (!readCommand->Execute("read", readArgs)) {
                std::cout << "[ERROR] Read failed at loop " << loop << ", LBA " << lba << std::endl;
                return false;
            }

            std::ifstream fin("ssd_output.txt");
            if (!fin.is_open()) {
                std::cout << "[ERROR] Failed to open ssd_output.txt" << std::endl;
                return false;
            }

            std::string result;
            std::getline(fin, result);
            fin.close();

            if (result != pattern) {
                std::cout << "[FAIL] Loop " << loop << ", LBA " << lba << " - Expected: " << pattern << ", Got: " << result << std::endl;
                std::cout << "FAIL" << std::endl;
                return false;
            }
        }
    }

    std::cout << "PASS" << std::endl;
    return true;
}
