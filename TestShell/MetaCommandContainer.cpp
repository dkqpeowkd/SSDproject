#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <set>

using std::string;
using std::vector;
using std::pair;
namespace fs = std::filesystem;

#include "MetaCommandContainer.h"

void MetaCommandContainer::loadMetaScript()
{
    // Read the Folders in folderPath, each folder match the script command.
    try {
        fs::directory_iterator directoryObject = fs::directory_iterator(metaScriptFolderPath);

        //load cmd help and repeats from the directory object
        for (const auto& entry : directoryObject) {
            if (entry.is_directory() == false) continue;
            //Read the files in the folder which is script command
            //It means Reading and Parsing what command do.
            string cmd = entry.path().stem().string();
            string executions = "";
            string help = "";
            string description = "";
            unsigned int repeat = 1;

            fs::directory_iterator dirIterator = fs::directory_iterator(entry.path());

            for (auto& cmdEntry : dirIterator) {
                string entryFileName = cmdEntry.path().stem().string();
                if (entryFileName == executionFile)
                    executions = loadFileContents(cmdEntry);
                if (entryFileName == helpFile)
                    help = loadFileContents(cmdEntry);
                if (entryFileName == descriptionFile)
                    description = loadFileContents(cmdEntry);
                if (entryFileName == repeatFile)
                {
                    string repeatStr = loadFileContents(cmdEntry);
                    if (repeatStr.size() == 0)
                        continue;
                    repeat = std::stoi(repeatStr);
                }
            }

            metaScriptDesc.emplace_back(MetaCommandDescription{ cmd, executions, description, help, repeat });
        }
    }
    catch (std::exception& e) {
        return;
    }
}

shared_ptr<ICommand> MetaCommandContainer::lookupCommand(const string& command, vector<shared_ptr<ICommand>>& supported)
{
    for (auto supportedCommand : supported) {
        if (supportedCommand->isMatch(command))
            return supportedCommand;
    }

    return nullptr;
}

shared_ptr<ScriptFunction> MetaCommandContainer::lookupScriptPhrase(const string& command)
{
    for (auto preScriptCommand : scriptPhrase) {
        if (preScriptCommand->isMatch(command))
            return preScriptCommand;
    }

    return nullptr;
}

shared_ptr<ScriptFunction> MetaCommandContainer::lookupScriptFunction(const string& command)
{
    for (auto scriptFunc : scriptFunctions) {
        if (scriptFunc->isMatch(command))
            return scriptFunc;
    }

    return nullptr;
}

const vector<shared_ptr<ScriptCommand>>& MetaCommandContainer::getScriptCommandList(vector<shared_ptr<ICommand>> supported)
{
    addPreDefinedScriptFunction(supported);
    for (const auto& metaScript : metaScriptDesc) {
        vector<pair<string, vector<string>>> executionCommands;
        // split command line to tokens, havin cmd and arguments.
        vector<pair<string, vector<string>>> cmds;
        if (metaScript.executions.size() == 0)
            return {};

        vector<string> lines = getLinesFromExecutions(metaScript.executions);

        for (auto line : lines) {
            std::istringstream iss(line);
            std::vector<std::string> tokens;
            std::string token;

            while (iss >> token) {
                tokens.push_back(token);
            }

            string cmd = tokens[0];
            tokens.erase(tokens.begin());

            executionCommands.emplace_back(pair{ cmd, tokens });
        }
    }
    return scriptCommandList;
}

void MetaCommandContainer::addPreDefinedScriptFunction(vector<shared_ptr<ICommand>> supported)
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

string MetaCommandContainer::loadFileContents(const fs::directory_entry& fileEntry)
{
    string __EMPTY = { "" };

    if (fileEntry.is_regular_file() == false)
        return __EMPTY;

    std::ifstream file(fileEntry.path());

    if (!file)
        return __EMPTY;

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

vector<string> MetaCommandContainer::getLinesFromExecutions(const string& executions)
{
    vector<string> lines;
    std::string line;
    std::istringstream iss(executions);

    while (std::getline(iss, line)) {
        lines.emplace_back(line);
    }
    return lines;
}