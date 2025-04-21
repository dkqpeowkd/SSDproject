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

void MetaCommandContainer::loadScript(vector<shared_ptr<ICommand>> supported)
{
	vector<string> scriptCommandList = loadScriptCommandList();

    for (auto scriptCommand : scriptCommandList) {
        string help = loadHelp(scriptCommand);
        unsigned int numloop = loadNumRepeats(scriptCommand);

        vector<string>lines = loadCommandsFromScript(scriptCommand);
        if (lines.size() == 0)
            continue;

        shared_ptr<ScriptCommand> exScript = make_shared<ScriptCommand>(scriptCommand, help);
        for (auto line : lines) {
            std::istringstream iss(line);
            std::vector<std::string> tokens;
            std::string token;

            while (iss >> token) {
                tokens.push_back(token);
            }
            
            string cmd = tokens[0];
            tokens.erase(tokens.begin());
            
            for (auto command : supported) {
                if (command->getCommandString() == cmd) {
                    exScript->addExecution(command, tokens);
                }
            }
        }
        executableScripts.emplace_back(exScript);
    }
	
}

shared_ptr<ICommand> MetaCommandContainer::lookupCommand(const string& command, vector<shared_ptr<ICommand>>& supported)
{
    for (auto supportedCommand : supported) {
        if (supportedCommand->getCommandString() == command)
            return supportedCommand;
    }
    return nullptr;
}

vector<string> MetaCommandContainer::getFileList(const string extension)
{
	const string& folderPath = scriptFolderPath;
	vector<string> fileList = {};

	for (const auto& entry : fs::directory_iterator(folderPath)) {
		if (entry.is_regular_file() && entry.path().extension() == extension) {
			//fileList.emplace_back(entry.path().string());
			fileList.emplace_back(entry.path().stem().string());
		}
	}
	return fileList;
}

vector<string> MetaCommandContainer::loadScriptCommandList()
{
	return getFileList(".cmd");
}

string MetaCommandContainer::loadHelp(const string& scriptCommand)
{
	const string fileName = scriptFolderPath + string{ "\\" } + scriptCommand + ".help";

    return loadFileContents(fileName);
}

unsigned long MetaCommandContainer::loadNumRepeats(const string& scriptCommand)
{
    const string fileName = scriptFolderPath + string{ "\\" } + scriptCommand + ".repeats";
    try {
        return std::stoul(loadFileContents(fileName));
    }
    catch (std::exception& e) {
        return 0;
    }
}

const vector<shared_ptr<ScriptCommand>>& MetaCommandContainer::getScriptCommands()
{
    // TODO: 여기에 return 문을 삽입합니다.
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
    string fileName = scriptFolderPath + string{"\\"} + scriptCommand + ".cmd";
    string fileContents = loadFileContents(fileName);

    vector<string> lines;

    std::istringstream iss(fileContents);
    std::string line;

    while (std::getline(iss, line)) {
        lines.emplace_back(line);
    }
    return lines;
}
