#pragma once

#include "ICommand.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>

class EraseCommandRaw : public ICommand {
public:
    const std::string& getCommandString() override {
        static std::string cmd = "erase";
        return cmd;
    }

    const std::string& getUsage() override {
        static std::string usage = "erase <LBA> <SIZE> : 특정 LBA부터 SIZE만큼 삭제합니다 (최대 10칸)";
        return usage;
    }

    bool isValidArguments(const std::string& cmd, std::vector<std::string>& args) override {
        return args.size() == 2;
    }

    bool Execute(const std::string& cmd, std::vector<std::string>& args) override {
        std::ostringstream oss;
        oss << "ssd.exe E " << args[0] << " " << args[1];

        int result = system(oss.str().c_str());

        std::ifstream fin("ssd_output.txt");
        std::string output;
        std::getline(fin, output);

        return output != "ERROR";
    }
};
