#pragma once
#include <memory>
#include "ICommand.h"

using std::pair;
//class ScriptCommand;

class ScriptCommand : public ICommand {
private:
public:
    ScriptCommand() {}
    ScriptCommand(const string& command) : scriptCommand{ command } {}
    virtual bool isMatch(const string& cmd) override;
    virtual const string& getCommandString() override;
    virtual const string& getUsage() override;
    virtual const string& getDescription() override;
    virtual bool isValidArguments(const string& cmd, vector<string>& args) override;
    virtual bool Execute(const string& cmd, vector<string>& args) override;
    void addExecution(std::shared_ptr<ICommand> command, vector<string> args);

    bool executeScript();
    class ScriptCommandBuilder {
    public:
        ScriptCommandBuilder(const string& command) {
            instance = std::make_shared<ScriptCommand>(command);
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
        ScriptCommandBuilder& addExecutableScript(vector<pair<std::shared_ptr<ICommand>, vector<string>>> executableScript) {
            for (auto e : executableScript) {
                instance->addExecution(e.first, e.second);
            }
            return *this;
        }
        std::shared_ptr<ScriptCommand> build() {
            return instance;
        }
    private:
        std::shared_ptr<ScriptCommand> instance;
    };

    //static ScriptCommandBuilder& Builder() { return *(make_shared<ScriptCommandBuilder>()); }
    static std::shared_ptr<ScriptCommandBuilder> Builder(
        const string& command) {
      return std::make_shared<ScriptCommandBuilder>(command);
    }

   protected:
    const string NOT_PROVIDED = "Not provided.\n";
    string scriptCommand = "";
    string usage = "";
    string description = "";
    vector<string> commandLines;
    vector<pair<std::shared_ptr<ICommand>, vector<string>>> scripts = {};
};