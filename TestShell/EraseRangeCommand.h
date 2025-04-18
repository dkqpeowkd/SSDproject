#pragma once

#include "ICommand.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>

class EraseRangeCommand : public ICommand {
public:
    EraseRangeCommand() = default;

    const std::string& getCommandString() override {
        static std::string cmd = "erase_range";
        return cmd;
    }

    const std::string& getUsage() override {
        static std::string usage = "erase_range <START_LBA> <END_LBA> : START부터 END까지 범위를 삭제합니다 (최대 10칸)";
        return usage;
    }

    bool isValidArguments(const std::string& cmd, std::vector<std::string>& args) override {
        return args.size() == 2;
    }

    bool Execute(const std::string& cmd, std::vector<std::string>& args) override {
        int start = std::stoi(args[0]);
        int end = std::stoi(args[1]);
        int size = end - start + 1;

        std::ostringstream oss;
        oss << "ssd.exe E " << start << " " << size;

        int result = callSystem(oss.str());
        std::string output = readOutput();
        return output != "ERROR";
    }

protected:
    virtual int callSystem(const std::string& cmd) {
        return system(cmd.c_str());
    }

    virtual std::string readOutput() {
        std::ifstream fin("ssd_output.txt");
        std::string line;
        std::getline(fin, line);
        return line;
    }
};
