#pragma once

#include "ICommand.h"
#include <string>
#include <vector>
#include <memory>

class Script2_PartialLBAWrite : public ICommand {
public:
    Script2_PartialLBAWrite(std::shared_ptr<ICommand> writeCommand, std::shared_ptr<ICommand> readCommand)
        : writeCommand(writeCommand), readCommand(readCommand) {
    }

    const std::string& getCommandString() override;
    const std::string& getUsage() override;
    bool isValidArguments(const std::string& cmd, std::vector<std::string>& args) override;
    bool Execute(const std::string& cmd, std::vector<std::string>& args) override;

private:
    const std::string command = "2_PartialLBAWrite";
    const std::string usage = "2_PartialLBAWrite: LBA 순서를 섞어가며 Write 후 ReadCompare 수행 (Loop 30회)";
    std::shared_ptr<ICommand> writeCommand;
    std::shared_ptr<ICommand> readCommand;
};