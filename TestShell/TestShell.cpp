#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <vector>
#include "TestShell.h"
#include "ICommand.h"
#include "ReadCommand.h"
#include "WriteCommand.h"
#include "FullReadCommand.h"
#include "FullWriteCommand.h"
#include "EraseCommand.h"
#include "EraseRangeCommand.h"
#include "FlushCommand.h"
#include "MetaCommandContainer.h"
#include <Windows.h>

using std::cout;

// 명령어 파서 유틸
std::vector<std::string> split(const std::string& line) {
	std::istringstream iss(line);
	std::vector<std::string> tokens;
	std::string token;
	while (iss >> token) tokens.push_back(token);
	return tokens;
}
TestShell::TestShell(bool EnableLog) {
	Logger* logger = (EnableLog) ? &loggerEnable : &loggerDisable;
	exitCommand = make_shared<ExitCommand>(logger);
	helpCommand = make_shared<HelpCommand>(logger);
	readCommand = make_shared<ReadCommand>(logger);
	writeCommand = make_shared<WriteCommand>(logger);
	fullReadCommand = make_shared<FullReadCommand>(logger);
	fullWriteCommand = make_shared<FullWriteCommand>(logger);
	eraseCommand = make_shared<EraseCommand>(logger);
	eraseRangeCommand = make_shared<EraseRangeCommand>(logger);
	flushCommand = make_shared<FlushCommand>(logger);

	addCommand(exitCommand);
	addCommand(helpCommand);
	addCommand(readCommand);
	addCommand(writeCommand);
	addCommand(fullReadCommand);
	addCommand(fullWriteCommand);
	addCommand(eraseCommand);
	addCommand(eraseRangeCommand);
	addCommand(flushCommand);

	helpCommand->addSupportedCommand(commandList);
}

TestShell::TestShell()
{
	Logger logger;
	exitCommand = make_shared<ExitCommand>();
	helpCommand = make_shared<HelpCommand>();
	readCommand = make_shared<ReadCommand>(&logger);
	writeCommand = make_shared<WriteCommand>(&logger);
	fullReadCommand = make_shared<FullReadCommand>(&logger);
	fullWriteCommand = make_shared<FullWriteCommand>(&logger);
	eraseCommand = make_shared<EraseCommand>(&logger);
	eraseRangeCommand = make_shared<EraseRangeCommand>(&logger);
	flushCommand = make_shared<FlushCommand>(&logger);


	addCommand(exitCommand);
	addCommand(helpCommand);
	addCommand(readCommand);
	addCommand(writeCommand);
	addCommand(fullReadCommand);
	addCommand(fullWriteCommand);
	addCommand(eraseCommand);
	addCommand(eraseRangeCommand);
	addCommand(flushCommand);

	helpCommand->addSupportedCommand(commandList);
}
void TestShell::run()
{
	while (exitCommand->isSystemActive()) {
		displayPrompt();
		PromptInput promptInput = getPromptInput();
		ExcutePromptInput(promptInput);
	}
}
std::filesystem::path getExecutableDir() {
  char path[MAX_PATH];
  GetModuleFileNameA(NULL, path, MAX_PATH);
  return std::filesystem::path(path).parent_path();
}

void TestShell::run(const std::string filename) {
	//std::filesystem::path exeDir = getExecutableDir();
	//std::filesystem::path filePath = exeDir / filename;
	//std::ifstream infile(filePath);
	std::ifstream infile(filename);

	std::string line;
	if (std::getline(infile, line)) {
		// BOM 제거
		if (line.size() >= 3 && static_cast<unsigned char>(line[0]) == 0xEF &&
			static_cast<unsigned char>(line[1]) == 0xBB &&
			static_cast<unsigned char>(line[2]) == 0xBF) {
			line = line.substr(3);
		}
	}

	if(false == ExecuteScriptFromFileLine(line))
		return;

	while (std::getline(infile, line)) {
		if(false == ExecuteScriptFromFileLine(line))
			return;
	}

  infile.close();  // 파일 닫기
}

bool TestShell::ExecuteScriptFromFileLine(string& line)
{
	PromptInput promptInput;
	promptInput.cmd = line;

	MetaCommandContainer scriptContainer;
	scriptContainer.loadMetaScript(commandList);
	shared_ptr<ICommand> scriptCommand = scriptContainer.getScriptCommand(promptInput.cmd, commandList);

	if (scriptCommand == nullptr) {
		std::cout << "Fail" << std::endl;
		return false;
	}

	std::cout << line << "      ___   Run...";
	if (scriptCommand->Execute(promptInput.cmd, promptInput.args) == false) {
		std::cout << "Fail" << std::endl;
		return false;
	}
	std::cout << "Pass" << std::endl;
	return true;
}

void TestShell::displayPrompt()
{
	cout << "Shell> ";
}

PromptInput TestShell::getPromptInput()
{
	try {
		PromptInput promptInput;
		string input;
		std::getline(std::cin, input);
		if (input.empty())
			return promptInput;

		auto tokens = split(input);
		std::string cmd = tokens[0];
		tokens.erase(tokens.begin());

		promptInput.cmd = cmd;
		promptInput.args = tokens;

		return promptInput;
	}
	catch (std::exception& e) {
		return PromptInput{};
	}
}

bool TestShell::ExcutePromptInput(PromptInput& promptInput)
{
	if (TryInternalCommand(promptInput) == false) {
		if (TryScriptCommand(promptInput.cmd, promptInput.args) == false) {
			std::cout << "INVALID COMMAND" << std::endl;
			return false;
		}
	}
	return true;
}

bool TestShell::TryInternalCommand(PromptInput& promptInput)
{
	shared_ptr<ICommand> foundCommand = findCommand(promptInput.cmd);

	if (foundCommand == nullptr)
		return false;

	if (false == isValidPromptInput(foundCommand, promptInput))
		return false;

	if (false ==foundCommand->Execute(promptInput.cmd, promptInput.args))
		return false;

	return true;
}

bool TestShell::TryScriptCommand(const string& cmd, vector<string>& args)
{
	MetaCommandContainer scriptContainer;
	scriptContainer.loadMetaScript(commandList);
	shared_ptr<ICommand> scriptCommand = scriptContainer.getScriptCommand(cmd, commandList);

	if (scriptCommand == nullptr)
		return false;

	if (scriptCommand->Execute(cmd, args) == false)
		return false;
	return true;
}

bool TestShell::isValidPromptInput(std::shared_ptr<ICommand>& foundCommand, PromptInput& promptInput)
{
	if (foundCommand == nullptr)
		return false;
	if (false == foundCommand->isValidArguments(promptInput.cmd, promptInput.args))
		return false;
	return true;
}

shared_ptr<ICommand> TestShell::findCommand(const string& command)
{
	if (commandList.empty()) return nullptr;

	for (auto& supported : commandList) {
		if (supported->isMatch(command))
			return supported;
	}
	return nullptr;
}

void TestShell::addCommand(shared_ptr<ICommand> newCommand)
{
	commandList.emplace_back(newCommand);
	//helpCommand->addHelp(newCommand->getUsage());
}
