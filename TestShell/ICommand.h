#pragma once
#include <vector>
#include <string>

using std::string;
using std::vector;
#define interface struct

interface ICommand {
public:
    virtual const string& getCommandString() = 0;
    virtual bool isMatch(const string& command) = 0;
    virtual const string& getUsage() = 0;
    virtual const string& getDescription() { return ""; }
    virtual bool isValidArguments(const string& cmd, vector<string>& args) = 0;
    virtual bool Execute(const string& cmd, vector<string>& args) = 0;
    virtual string getResultValue() { return ""; }
};