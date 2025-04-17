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
    const string usage = command +" :\n\t200ȸ �ݺ�, LBA 0 / 99�� ���� �� Write �� ReadCompare\nusage:\n\tPrompt>" + command;
    string result = "";

    shared_ptr<ICommand> writeCommand;
    shared_ptr<ICommand> readCommand;
};

