#pragma once
#include <string>
#include <vector>
#include <memory>
#include <set>
#include <filesystem>
#include "ICommand.h"
#include "ScriptCommand.h"
#include "ScriptFunction.h"
#include "ScriptFunctionWrite.h"
#include "ScriptFunctionErase.h"
#include "ScriptFunctionLoop.h"

using std::string;
using std::vector;
using std::set;
using std::shared_ptr;
using std::make_shared;

struct MetaCommandDescription {
	string cmd;
	string executions;
	string description;
	string usage;
	unsigned int repeats;
};

class MetaCommandContainer {
public:
	MetaCommandContainer() {}
	MetaCommandContainer(const string scriptPath) : metaScriptFolderPath{ scriptPath } {}
	void loadMetaScript();
	MetaCommandDescription getMetaCommandDescriptionFromFile(const fs::directory_entry& entry);
	const vector<shared_ptr<ScriptCommand>>& getScriptCommandList(vector<shared_ptr<ICommand>> supported);
private:
	const string executionFile = "execution";
	const string helpFile = "help";
	const string descriptionFile = "description";
	const string repeatFile = "repeat";
	string metaScriptFolderPath = "scripts";

	void addPreDefinedScriptFunction(vector<shared_ptr<ICommand>> supported);
	string loadFileContents(const fs::directory_entry& fileEntry);
	shared_ptr<ICommand> lookupCommand(const string& command, vector<shared_ptr<ICommand>>& supported);
	shared_ptr<ScriptFunction> lookupScriptPhrase(const string& command);
	shared_ptr<ScriptFunction> lookupScriptFunction(const string& command);
	vector<string> getLinesFromExecutions(const string&);
	vector<pair<string, vector<string>>> getCommandsAndArgumentsFromExecutions(const string& executions);
	vector<pair<string, vector<string>>> extractMetaCommands(vector<string>& commandLines);
	vector<pair<shared_ptr<ICommand>, vector<string>>> getExecutableScripts(vector<pair<string, vector<string>>>& executionCommands, vector<shared_ptr<ICommand>>& supported);
	bool parseScriptToExcutable(
		shared_ptr<ScriptFunction> parent,
		vector<pair<string, vector<string>>>::iterator& startIter,
		vector<shared_ptr<ICommand>>& supported);
	bool parsePhraseInScript(shared_ptr<ScriptFunction> parent, vector<pair<string, vector<string>>>::iterator& startIter, vector<shared_ptr<ICommand>>& supported);
private:
	vector<shared_ptr<ScriptFunction>> scriptPhrase;
	vector<shared_ptr<ScriptFunction>> scriptFunctions;
	vector<shared_ptr<ScriptCommand>> scriptCommandList;
	vector<MetaCommandDescription> metaScriptDesc;
	vector<MetaCommandDescription> metaCommandDesc;
};