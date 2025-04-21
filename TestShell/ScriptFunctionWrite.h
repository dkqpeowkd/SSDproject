#pragma once

#include "ScriptFunction.h"

class ScriptFunctionWrite : public ScriptFunction {
private:
    const string command = "WriteFunc";
    shared_ptr<ICommand> writeCommand;
    shared_ptr<ICommand> readCommand;
    const string __EMPTY = "";
public:
    shared_ptr<ScriptFunction> clone() const override { return nullptr; }
    ScriptFunctionWrite(shared_ptr<ICommand> writeCommand, shared_ptr<ICommand> readCommand) : writeCommand{ writeCommand }, readCommand{ readCommand } {}
    const string& getCommandString() override { return command; }
    bool isMatch(const string& command) override { return this->command == command; }
    const string& getUsage() override { return __EMPTY; }
    bool isValidArguments(const string& cmd, vector<string>& args) override {
        if (isMatch(cmd) == false)return false;
        if (args.size() == 0 || args.size() > 3) return false;
        return true;
    }
    bool Execute(const string& cmd, vector<string>& args) override {
        if (isValidArguments(cmd, args) == false) return false;

        string& opt = args[0];
        switch (args.size()) {
        case 1:
            if (opt == "EnableAutoIncrease") {

                return true;
            }
            if (opt == "DisableAutoIncrease") {

                return true;
            }
            if (opt == "IncreaseLBA") {

                return true;
            }
            if (opt == "ReadCompare") {

            }
            break;
        case 2:

            if (opt == "SetLBA") {

            }
            if (opt == "Random") {

            }
            break;
        default:
            return false;
        }

        return false;
    }
};