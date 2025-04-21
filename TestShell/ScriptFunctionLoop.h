#pragma once

#include "ScriptFunction.h"

class ScriptFunctionLoop : public ScriptFunction {
private:
    const string command = "Loop";
    const string __EMPTY = "";
public:
    shared_ptr<ScriptFunction> clone() const override { return nullptr; }
    ScriptFunctionLoop() {}
    const string& getCommandString() override { return command; }
    bool isMatch(const string& command) override { return this->command == command; }
    const string& getUsage() override { return __EMPTY; }
    bool isValidArguments(const string& cmd, vector<string>& args) override {
        if (isMatch(cmd) == false)return false;
        if (args.size() != 1) return false;
        return true;
    }
    bool Execute(const string& cmd, vector<string>& args) override {
        if (isMatch(cmd) == false)
            return false;
        if (isValidArguments(cmd, args) == false)
            return false;

        return executeScriptLoop(args);
    }

    bool executeScriptLoop(vector<string>& args) {
        int loopCnt = std::stoi(args[0]);

        for (int i = 0; i < loopCnt; ++i) {
            if (false == executeScript())
                return false;
        }

        return true;
    }

};