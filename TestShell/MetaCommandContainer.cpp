#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <sstream>
#include "ScriptCommand.h"
#include "MetaCommandContainer.h"

using std::string;
using std::vector;
using std::pair;
namespace fs = std::filesystem;

void MetaCommandContainer::loadMetaScript()
{
	loadMetaScriptDescription(metaScriptFolderPath);

    //load meta description
	
}

void MetaCommandContainer::loadMetaCommand(vector<shared_ptr<ICommand>> supported)
{
    //load meta command
}

shared_ptr<ICommand> MetaCommandContainer::lookupCommand(const string& command, vector<shared_ptr<ICommand>>& supported)
{
    for (auto supportedCommand : supported) {
        if (supportedCommand->getCommandString() == command)
            return supportedCommand;
    }
    return nullptr;
}

void MetaCommandContainer::addScriptFunctions(vector<shared_ptr<ICommand>>& supported)
{
    shared_ptr<ICommand> readCommand = lookupCommand("read", supported);
    shared_ptr<ICommand> writeCommand = lookupCommand("write", supported);
    if (readCommand && writeCommand) {
        shared_ptr<ScriptFunctionWrite> writeFunc = make_shared<ScriptFunctionWrite>(writeCommand, readCommand);
        scriptFunctions.emplace_back(writeFunc);
    }
    
    shared_ptr<ICommand> eraseCommand = lookupCommand("erase", supported);
    shared_ptr<ICommand> eraseRangeCommand = lookupCommand("erase_range", supported);
    if (eraseCommand && eraseRangeCommand) {
        shared_ptr<ScriptFunctionErase> eraseFunc = make_shared<ScriptFunctionErase>(eraseCommand, eraseRangeCommand);
        scriptFunctions.emplace_back(eraseFunc);
    }

    shared_ptr<ScriptFunctionLoop> functionLoop = make_shared<ScriptFunctionLoop>();
    scriptPhrase.emplace_back(functionLoop);
}

vector<string> MetaCommandContainer::getFileList(const string extension)
{
	const string& folderPath = metaScriptFolderPath;
	vector<string> fileList = {};

	for (const auto& entry : fs::directory_iterator(folderPath)) {
		if (entry.is_regular_file() && entry.path().extension() == extension) {
			//fileList.emplace_back(entry.path().string());
			fileList.emplace_back(entry.path().stem().string());
		}
	}
	return fileList;
}

void MetaCommandContainer::loadMetaScriptDescription(const string& folderPath)
{
    // Read the Folders in folderPath, each folder match the script command.
    try {
        fs::directory_iterator directoryObject = fs::directory_iterator(folderPath);

        //load cmd help and repeats from the directory object
        for (const auto& entry : directoryObject) {
            if (entry.is_directory() == false) continue;
            metaScriptDesc.emplace_back(getMetaDescription(entry));
        }
    }
    catch (std::exception& e) {
        return;
    }
}

MetaCommandDescription MetaCommandContainer::getMetaDescription(const fs::directory_entry& entry)
{
    //Read the files in the folder which is script command
    //It means Reading and Parsing what command do.
    string cmd = entry.path().stem().string();
    string executions = "";
    string help = "";
    string description = "";
    unsigned int repeat = 1;

    fs::directory_iterator dirIterator = fs::directory_iterator(entry.path());
    //vector<fs::directory_entry> sortedFiles = getSortedEntries(dirIterator, [](const fs::directory_entry a) {return a.is_regular_file(); });

    for(auto& cmdEntry : dirIterator) {
        string entryFileName = cmdEntry.path().stem().string();
        if (entryFileName == executionFile)
            executions = "";
        if (entryFileName == helpFile)
            help = "";
        if (entryFileName == descriptionFile)
            description = "";
    }

    return MetaCommandDescription{ cmd, executions, description, help, repeat };
}

string MetaCommandContainer::loadHelp(const string& scriptCommand)
{
	const string fileName = metaScriptFolderPath + string{ "\\" } + scriptCommand + ".help";

    return loadFileContents(fileName);
}

unsigned long MetaCommandContainer::loadNumRepeats(const string& scriptCommand)
{
    const string fileName = metaScriptFolderPath + string{ "\\" } + scriptCommand + ".repeats";
    try {
        return std::stoul(loadFileContents(fileName));
    }
    catch (std::exception& e) {
        return 0;
    }
}

const vector<shared_ptr<ScriptCommand>>& MetaCommandContainer::getScriptCommands(vector<shared_ptr<ICommand>>& supportedCommand)
{
    addScriptFunctions(supportedCommand);
    return executableScripts;
}

string MetaCommandContainer::loadFileContents(string fileName)
{
    string __EMPTY = { "" };
    // read file and save.
    fs::path filepath = fileName; // 읽을 파일 경로

    // 파일 존재 여부와 regular file인지 확인
    if (!fs::exists(filepath) || !fs::is_regular_file(filepath)) {
        //std::cerr << "파일이 존재하지 않거나 일반 파일이 아닙니다." << std::endl;
        return __EMPTY;
    }

    // 파일 열기
    std::ifstream file(filepath, std::ios::in);
    if (!file) {
        //std::cerr << "파일을 열 수 없습니다." << std::endl;
        return __EMPTY;
    }

    // 파일 전체를 문자열로 읽기
    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

vector<string> MetaCommandContainer::loadCommandsFromScript(const string& scriptCommand)
{
    string fileName = metaScriptFolderPath + string{"\\"} + scriptCommand + ".cmd";
    string fileContents = loadFileContents(fileName);

    vector<string> lines;

    std::istringstream iss(fileContents);
    std::string line;

    while (std::getline(iss, line)) {
        lines.emplace_back(line);
    }
    return lines;
}
