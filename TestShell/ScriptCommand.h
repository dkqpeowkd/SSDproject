#pragma once
#include <memory>
#include <string>
#include <vector>
#include "ICommand.h"

using std::pair;
using std::shared_ptr;
using std::make_shared;
using std::string;

class ScriptCommand : public ICommand {
private:
public:
    ScriptCommand() {}
    shared_ptr<ScriptCommand> clone() { 
        shared_ptr<ScriptCommand> cloneInstance = make_shared<ScriptCommand>(*this);
        cloneInstance->scripts = this->scripts;
        return cloneInstance;
    }
    ScriptCommand(const string& command) : scriptCommand{ command } {}
    virtual bool isMatch(const string& cmd) override;
    virtual const string& getCommandString() override;
    virtual const string& getUsage() override;
    virtual const string& getDescription() override;
    virtual bool isValidArguments(const string& cmd, vector<string>& args) override;
    virtual bool Execute(const string& cmd, vector<string>& args) override;
    void addExecution(shared_ptr<ICommand> command, vector<string> args);

    bool executeScript();
    class ScriptCommandBuilder {
    public:
        ScriptCommandBuilder(const string& command) {
            instance = make_shared<ScriptCommand>(command);
        }
        ScriptCommandBuilder& setCommand(const string& command) {
            instance->scriptCommand = command;
            return *this;
        }
        ScriptCommandBuilder& setUsage(const string& usage) {
            instance->usage = usage;
            return *this;
        }
        ScriptCommandBuilder& setDescription(const string& description) {
            instance->description = description;
            return *this;
        }
        ScriptCommandBuilder& addExecutableScript(vector<pair<shared_ptr<ICommand>, vector<string>>> executableScript) {
            for (auto e : executableScript) {
                instance->addExecution(e.first, e.second);
            }
            return *this;
        }
        shared_ptr<ScriptCommand> build() {
            return instance;
        }
    private:
        shared_ptr<ScriptCommand> instance;
    };

    //static ScriptCommandBuilder& Builder() { return *(make_shared<ScriptCommandBuilder>()); }
    static shared_ptr<ScriptCommandBuilder> Builder(const string& command) { return make_shared<ScriptCommandBuilder>(command); }
protected:
    const string NOT_PROVIDED = "Not provided.\n";
    string scriptCommand = "";
    string usage = "";
    string description = "";
    vector<pair<shared_ptr<ICommand>, vector<string>>> scripts = {};
};