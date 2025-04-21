#pragma once
#include "ICommand.h"
#include <string>
#include <vector>

class EraseCommand : public ICommand {
public:
    const std::string& getCommandString() override;
    const std::string& getUsage() override;
    bool isMatch(const std::string& command) override;
    bool isValidArguments(const std::string& cmd, std::vector<std::string>& args) override;
    bool Execute(const std::string& cmd, std::vector<std::string>& args) override;

protected:
    virtual int callSystem(const std::string& cmd);
    virtual std::string readOutput();

private:
    const std::string cmd = "erase";

    struct EraseCall {
        int lba;
        int size;
    };

    std::vector<EraseCall> groupAndChunk(const std::vector<EraseCall>& calls);
    void performEraseCalls(const std::vector<EraseCall>& calls);
};
