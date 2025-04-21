#pragma once

#include "ScriptCommand.h"

class ScriptFunction : public ScriptCommand {
public:
    virtual ~ScriptFunction() = default;
    virtual shared_ptr<ScriptFunction> clone() const = 0;
    const string& getStartKeyword() { return getCommandString(); }
    const string getFinishKeyword() { return "End" + getStartKeyword(); }
};

class ScriptFunctionWrite : public ScriptFunction {
private:
    const string command = "WriteFunc";
    shared_ptr<ICommand> writeCommand;
    shared_ptr<ICommand> readCommand;
    const string __EMPTY = "";
public:
    shared_ptr<ScriptFunction> clone() const override { return nullptr; }
    ScriptFunctionWrite(shared_ptr<ICommand> writeCommand, shared_ptr<ICommand> readCommand) : writeCommand{ writeCommand }, readCommand{ readCommand } {}
    const string& getCommandString() override {return command;}
    bool isMatch(const string& command) override { return this->command == command; }
    const string& getUsage() override { return __EMPTY; }
    bool isValidArguments(const string& cmd, vector<string>& args) override {
        if (isMatch(cmd) == false)return false;
        if (args.size() == 0 || args.size() > 3) return false;
        return true;
    }
    bool Execute(const string& cmd, vector<string>& args) override {
        if (isValidArguments(cmd, args) == false) return false;

        return false;
    }
};

class ScriptFunctionErase : public ScriptFunction {
private:
    const string command = "EraseFunc";
    shared_ptr<ICommand> eraseCommand;
    const string __EMPTY = "";
public:
    shared_ptr<ScriptFunction> clone() const override { return nullptr; }
    ScriptFunctionErase(shared_ptr<ICommand> eraseCommand) : eraseCommand{ eraseCommand } {}
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

        return false;
    }
};

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