#pragma once
#include <vector>
#include <string>

using std::string;
using std::vector;
#define interface struct

interface ICommand {
public:
    virtual const string& getCommandString() = 0;
    virtual const string& getUsage() = 0;
    virtual bool isValidArguments(const string& cmd, vector<string>& args) = 0;
    virtual bool Execute(const string& cmd, vector<string>& args) = 0;
};