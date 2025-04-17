#pragma once

#include "ICommand.h"

interface ExitCommand : public ICommand {
    const string& getCommandString() { return command;}
    const string& getUsage() { return usage; }
    bool isValidArguments(const string& cmd, vector<string>& args) { return args.size() == 0; }
    bool Execute(const string& cmd, vector<string>& args) {
        if (false == isValidArguments(cmd, args))
            return false;
        isActive = false;
        return true;
    }
    bool isSystemActive() { return isActive; }
private:
    const string command = "exit";
    const string usage = "";
    bool isActive = true;
};
