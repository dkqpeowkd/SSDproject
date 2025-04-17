#include "Script2.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>

const std::string& Script2_PartialLBAWrite::getCommandString() {
    static std::string command = "2_PartialLBAWrite";
    return command;
}

const std::string& Script2_PartialLBAWrite::getUsage() {
    static std::string usage = "2_PartialLBAWrite: LBA 순서를 섞어가며 Write 후 ReadCompare 수행 (Loop 30회)";
    return usage;
}

bool Script2_PartialLBAWrite::isValidArguments(const std::string& cmd, std::vector<std::string>& args) {
    return (cmd == "2_" || cmd == "2_PartialLBAWrite") && args.empty();
}

bool Script2_PartialLBAWrite::Execute(const std::string& cmd, std::vector<std::string>& args) {
    std::string pattern = "0xABCD1234";
    std::vector<int> lbas = { 4, 0, 3, 1, 2 };

    for (int loop = 0; loop < 30; ++loop) {
        // Write to shuffled LBAs
        for (int lba : lbas) {
            std::ostringstream writeCmd;
            writeCmd << "ssd.exe write " << lba << " " << pattern;
            system(writeCmd.str().c_str());
        }

        // ReadCompare
        for (int lba : lbas) {
            std::ostringstream readCmd;
            readCmd << "ssd.exe read " << lba;
            system(readCmd.str().c_str());

            std::ifstream fin("ssd_output.txt");
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
