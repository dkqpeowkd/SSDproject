#pragma once
#include <memory>
#include "ICommand.h"

using std::shared_ptr;

class script_3_WriteReadAging : public ICommand {
public:
    script_3_WriteReadAging(shared_ptr<ICommand> writeCommand, shared_ptr<ICommand> readCommand) 
        : writeCommand{ writeCommand }, readCommand{ readCommand } {}
    const string& getCommandString() override;
    const string& getUsage() override;
    bool isValidArguments(const string& cmd, vector<string>& args) override;
    bool Execute(const string& cmd, vector<string>& args) override;
private:
    const string command = "3_WriteReadAging";
    const string usage = command +" :\n\t200회 반복, LBA 0 / 99에 랜덤 값 Write 후 ReadCompare\nusage:\n\tPrompt>" + command;
    string result = "";

    shared_ptr<ICommand> writeCommand;
    shared_ptr<ICommand> readCommand;
};

