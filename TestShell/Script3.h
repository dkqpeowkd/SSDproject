#pragma once

#include "ICommand.h"
#include <string>
#include <vector>
#include <memory>

class Script3_WriteReadAging : public ICommand {
public:
    Script3_WriteReadAging(std::shared_ptr<ICommand> writeCommand, std::shared_ptr<ICommand> readCommand)
        : writeCommand(writeCommand), readCommand(readCommand) {
    }

    const std::string& getCommandString() override;
    const std::string& getUsage() override;
    bool isValidArguments(const std::string& cmd, std::vector<std::string>& args) override;
    bool Execute(const std::string& cmd, std::vector<std::string>& args) override;

private:
    const std::string command = "3_WriteReadAging";
    const std::string usage = "3_WriteReadAging: 200회 반복, LBA 0 / 99에 랜덤 값 Write 후 ReadCompare 수행";
    std::shared_ptr<ICommand> writeCommand;
    std::shared_ptr<ICommand> readCommand;
};
