#pragma once
#include <string>
#include <vector>
#include <memory>
#include "ICommand.h"
#include "ScriptCommand.h"

using std::string;
using std::vector;
using std::shared_ptr;
using std::make_shared;

class MetaCommandContainer {
public:
	MetaCommandContainer() {}
	MetaCommandContainer(const string scriptPath) : scriptFolderPath{ scriptPath } {}
	void loadScript(vector<shared_ptr<ICommand>> supported);
	vector<string> getFileList(const string extension);
	vector<string> loadScriptCommandList(); // list up the XXXXXX.cmd in scripts folder
	string loadHelp(const string& scriptCommand);
	unsigned long loadNumRepeats(const string& scriptCommand);
	const vector<shared_ptr<ScriptCommand>>& getScriptCommands();
private:
	string scriptFolderPath = "scripts";
	string loadFileContents(string fileName);
	vector<string> loadCommandsFromScript(const string& scriptCommand);
	shared_ptr<ICommand> lookupCommand(const string& command, vector<shared_ptr<ICommand>>& supported);
	vector<shared_ptr<ScriptCommand>> executableScripts;
};

