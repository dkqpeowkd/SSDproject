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
#include "ScriptFunctionWrite.h"
#include "ScriptFunctionErase.h"
#include "ScriptFunctionLoop.h"

void MetaCommandContainer::loadMetaScript()
{
    // Read the Folders in folderPath, each folder match the script command.
    try {
        fs::directory_iterator directoryObject = fs::directory_iterator(metaScriptFolderPath);

        //load cmd help and repeats from the directory object
        for (const auto& entry : directoryObject) {
            if (entry.is_directory() == false) continue;
            metaScriptDesc.emplace_back(getMetaCommandDescriptionFromFile(entry));
        }
    }
    catch (std::exception& e) {
        return;
    }
}

MetaCommandDescription MetaCommandContainer::getMetaCommandDescriptionFromFile(const fs::directory_entry& entry)
{
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
    return MetaCommandDescription{ cmd, executions, description, help, repeat };
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
        try {
            vector<pair<string, vector<string>>> executionCommands = getCommandsAndArgumentsFromExecutions(metaScript.executions);
            vector<pair<shared_ptr<ICommand>, vector<string>>> executableScripts = getExecutableScripts(executionCommands, supported);

            shared_ptr<ScriptCommand> newScriptCommand = ScriptCommand::Builder(metaScript.cmd)
                ->setUsage(metaScript.usage)
                .setDescription(metaScript.description)
                .addExecutableScript(executableScripts)
                .build();
            scriptCommandList.emplace_back(newScriptCommand);
        }
        catch (std::exception& e) {
            continue;
        }
    }
    return scriptCommandList;
}

shared_ptr<ScriptCommand> MetaCommandContainer::createNewScriptCommandInstance(MetaCommandDescription& metaScript, vector<shared_ptr<ICommand>> supported)
{
    try {
        vector<pair<string, vector<string>>> executionCommands = getCommandsAndArgumentsFromExecutions(metaScript.executions);
        vector<pair<shared_ptr<ICommand>, vector<string>>> executableScripts = getExecutableScripts(executionCommands, supported);

        shared_ptr<ScriptCommand> newScriptCommand = ScriptCommand::Builder(metaScript.cmd)
            ->setUsage(metaScript.usage)
            .setDescription(metaScript.description)
            .addExecutableScript(executableScripts)
            .build();
        return newScriptCommand;
    }
    catch (std::exception& e) {
        return nullptr;
    }
}

shared_ptr<ScriptCommand> MetaCommandContainer::getScriptCommand(const string& cmd, vector<shared_ptr<ICommand>> supported)
{
    for (auto& metaScript : metaScriptDesc) {
        if (metaScript.cmd == cmd) {
            return createNewScriptCommandInstance(metaScript, supported);
        }
    }

    return nullptr;
}

vector<pair<shared_ptr<ICommand>, vector<string>>> MetaCommandContainer::getExecutableScripts(vector<pair<string, vector<string>>>& executionCommands, vector<shared_ptr<ICommand>>& supported)
{
    shared_ptr<ScriptCommand> parent = nullptr;
    vector<pair<shared_ptr<ICommand>, vector<string>>> execScripts;

    for (auto iter = executionCommands.begin(); iter != executionCommands.end(); ++iter)
    {

        auto execCmd = *iter;
        string cmd = execCmd.first;
        vector<string> args = execCmd.second;

        shared_ptr<ICommand> foundCommand = lookupCommand(cmd, supported);
        shared_ptr<ScriptFunction> foundFunc = lookupScriptFunction(cmd);
        shared_ptr<ScriptFunction> foundScript = lookupScriptPhrase(cmd);
        if (foundCommand == nullptr && foundFunc == nullptr && foundScript == nullptr)
            continue;
        if (foundCommand)
            execScripts.emplace_back(pair{ foundCommand, args });
        if (foundFunc)
            execScripts.emplace_back(pair{ foundFunc, args });
        if (foundScript) {
            iter++;
            if (parseScriptToExcutable(foundScript, iter, supported))
                execScripts.emplace_back(pair{ foundScript, args });
        }
    }
    return execScripts;
}

bool MetaCommandContainer::parseScriptToExcutable(
    shared_ptr<ScriptFunction> parent, 
    vector<pair<string, vector<string>>>::iterator& startIter,
    vector<shared_ptr<ICommand>>& supported)
{
    try {
        string finishKey = parent->getFinishKeyword();
        vector<pair<string, vector<string>>>::iterator& iter = startIter;
        for (iter = startIter; iter->first != finishKey; iter++) {
            if (parsePhraseInScript(parent, startIter, supported) == false) continue;
        }
        return true;
    }
    catch (std::exception& e) {
        return false;
    }
}

bool MetaCommandContainer::parsePhraseInScript(
    shared_ptr<ScriptFunction> parent,
    vector<pair<string, vector<string>>>::iterator& startIter,
    vector<shared_ptr<ICommand>>& supported)
{
    auto execCmd = *startIter;
    string cmd = execCmd.first;
    vector<string> args = execCmd.second;

    shared_ptr<ICommand> foundCommand = lookupCommand(cmd, supported);
    shared_ptr<ScriptFunction> foundFunc = lookupScriptFunction(cmd);
    shared_ptr<ScriptFunction> foundScript = lookupScriptPhrase(cmd);
    if (foundCommand == nullptr && foundFunc == nullptr && foundScript == nullptr)
        return false;
    if (foundCommand) {
        parent->addExecution(foundCommand, args);
    }
    if (foundFunc)
        parent->addExecution(foundFunc, args);
    if (foundScript) {
        startIter++;
        if (parseScriptToExcutable(foundScript, startIter, supported))
            parent->addExecution(foundScript, args);
        else
            return false;
    }

    return true;
}

vector<pair<string, vector<string>>> MetaCommandContainer::getCommandsAndArgumentsFromExecutions(const string& executions)
{ 
    if (executions.size() == 0)
        return {};

    vector<string> lines = getLinesFromExecutions(executions);
    return extractMetaCommands(lines);
}

vector<pair<string, vector<string>>> MetaCommandContainer::extractMetaCommands(vector<string>& commandLines)
{
    vector<pair<string, vector<string>>> metaCommands;
    for (auto line : commandLines) {
        std::istringstream iss(line);
        std::vector<std::string> tokens;
        std::string token;

        while (iss >> token) {
            tokens.push_back(token);
        }

        string cmd = tokens[0];
        tokens.erase(tokens.begin());

        metaCommands.emplace_back(pair{ cmd, tokens });
    }

    return metaCommands;
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