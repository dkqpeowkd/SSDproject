#pragma once
#include <string>
#include <vector>
#include <memory>
#include "ICommand.h"
#include "ScriptCommand.h"
#include "ScriptFunction.h"

using std::string;
using std::vector;
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
	MetaCommandContainer(const string scriptPath) : scriptFolderPath{ scriptPath } {}
	void loadMetaScript();
	void loadMetaCommand(vector<shared_ptr<ICommand>> supported);
	vector<string> getFileList(const string extension);
	void loadMetaScriptDescription(const string& folderPath);
	MetaCommandDescription getMetaDescription(const fs::directory_entry& entry);
	string loadHelp(const string& scriptCommand);
	unsigned long loadNumRepeats(const string& scriptCommand);
	const vector<shared_ptr<ScriptCommand>>& getScriptCommands(vector<shared_ptr<ICommand>>& supportedCommand);
private:
	const string scriptFolderPath = "scripts";
	const string executionFile = "execution";
	const string descriptionFile = "description";
	const string helpFile = "help";

	string loadFileContents(string fileName);
	vector<string> loadCommandsFromScript(const string& scriptCommand);
	shared_ptr<ICommand> lookupCommand(const string& command, vector<shared_ptr<ICommand>>& supported);
	void addScriptFunctions(vector<shared_ptr<ICommand>>& supported);

	vector<shared_ptr<ScriptCommand>> executableScripts;
	vector<MetaCommandDescription> metaScriptDesc;

	//vector<ScriptFunction> scriptFunctions;
	//vector<ScriptFunction> scriptPhrase;
};

