
#pragma once
#include "ICommand.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class ReadCommand : public ICommand {
public:
    virtual ~ReadCommand() = default;

    const std::string& getCommandString() override {
        static std::string cmd = "read";
        return cmd;
    }

    const std::string& getUsage() override {
        static std::string usage = "read <LBA>";
        return usage;
    }

    bool isValidArguments(const std::string& cmd, std::vector<std::string>& args) override {
        if (args.size() != 2) return false;
        try {
            int lba = std::stoi(args[1]);
            return lba >= 0 && lba < 100;
        }
        catch (...) {
            return false;
        }
    }

    bool Execute(const std::string& cmd, std::vector<std::string>& args) override;

protected:
    virtual int callSystem(const std::string& cmd);
    virtual std::string readOutput();
};
