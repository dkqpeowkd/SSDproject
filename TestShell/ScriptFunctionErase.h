#pragma once

#include "ScriptFunction.h"

class ScriptFunctionErase : public ScriptFunction {
private:
    const string command = "EraseFunc";
    shared_ptr<ICommand> eraseCommand;
    shared_ptr<ICommand> eraseRangeCommand;
    const string __EMPTY = "";

    bool isAutoIncrease = false;
    int LBAoffset = 0;
public:
    shared_ptr<ScriptFunction> cloneInstance() const override { return make_shared<ScriptFunctionErase>(*this); }
    ScriptFunctionErase(shared_ptr<ICommand> eraseCommand, shared_ptr<ICommand> eraseRangeCommand) : eraseCommand{ eraseCommand }, eraseRangeCommand{ eraseRangeCommand } {}
    const string& getCommandString() override { return command; }
    bool isMatch(const string& command) override { return this->command == command; }
    const string& getUsage() override { return __EMPTY; }
    bool isValidArguments(const string& cmd, vector<string>& args) override {
        if (isMatch(cmd) == false)return false;
        if (args.size() < 1 || args.size() > 2) return false;
        return true;
    }
    bool Execute(const string& cmd, vector<string>& args) override {
        if (isValidArguments(cmd, args) == false) return false;

        string& opt = args[0];
        switch (args.size()) {
        case 1:
            if (opt == "EnableAutoIncrease") {
                isAutoIncrease = true;
                return true;
            }
            if (opt == "DisableAutoIncrease") {
                isAutoIncrease = false;
                return true;
            }
            if (opt == "IncreaseLBA") {
                LBAoffset++;
                return true;
            }
            if (opt == "DecreaseLBA") {
                LBAoffset--;
                return true;
            }
            {
                string startLBA = std::to_string(LBAoffset);
                int increase = std::stoi(args[0]);
                vector<string> argsStr{ startLBA, args[0] };
                if (false == eraseCommand->Execute(eraseCommand->getCommandString(), argsStr))
                    return false;

                LBAoffset += increase;
                return true;
            }
            break;
        case 2:
        {
            if (opt == "SetLBA") {
                try {
                    int offset = std::stoi(args[1]);
                    LBAoffset = offset;
                    return true;
                }
                catch (std::exception& e) {
                    return false;
                }
            }
            /*
            string startLBA = std::to_string(LBAoffset);
            int increase = std::stoi(args[0]);
            string endLBA = std::to_string(LBAoffset + increase - 1);
            vector<string> argsStr{ startLBA, endLBA };
            if (false == eraseRangeCommand->Execute(eraseRangeCommand->getCommandString(), argsStr))
                return false;

            LBAoffset += increase;
            return true;
            */
        }
            break;
        default:
            return false;
        }

        return false;
    }
};