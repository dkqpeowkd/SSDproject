#pragma once

#include "ScriptFunction.h"

class ScriptFunctionErase : public ScriptFunction {
private:
    const string command = "EraseFunc";
    shared_ptr<ICommand> eraseCommand;
    shared_ptr<ICommand> eraseRangeCommand;
    const string __EMPTY = "";
public:
    shared_ptr<ScriptFunction> clone() const override { return nullptr; }
    ScriptFunctionErase(shared_ptr<ICommand> eraseCommand, shared_ptr<ICommand> eraseRangeCommand) : eraseCommand{ eraseCommand }, eraseRangeCommand{ eraseRangeCommand } {}
    const string& getCommandString() override { return command; }
    bool isMatch(const string& command) override { return this->command == command; }
    const string& getUsage() override { return __EMPTY; }
    bool isValidArguments(const string& cmd, vector<string>& args) override {
        if (isMatch(cmd) == false)return false;
        if (args.size() != 2) return false;
        return true;
    }
    bool Execute(const string& cmd, vector<string>& args) override {
        if (isValidArguments(cmd, args) == false) return false;

        // ToDo : need to add handling options.

        /*
        int startLBA = std::stoi(args[0]);
        int numLBA = std::stoi(args[1]);

        for (int i = startLBA; i < startLBA + numLBA; ++i) {
            if (false == eraseCommand->Execute(eraseCommand->getCommandString(), args))
                return false;
        }
        */
        return true;
    }
};