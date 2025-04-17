#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <string>
#include <vector>
#include "TestShell.h"
#include "ICommand.h"
#include "Script1.h"

using std::cout;

// 명령어 파서 유틸
std::vector<std::string> split(const std::string& line) {
	std::istringstream iss(line);
	std::vector<std::string> tokens;
	std::string token;
	while (iss >> token) tokens.push_back(token);
	return tokens;
}

TestShell::TestShell()
{
	exitCommand = make_shared<ExitCommand>();
	commandList.emplace_back(exitCommand);
	shared_ptr<HelpCommand> helpCommand = make_shared<HelpCommand>();
	commandList.emplace_back(helpCommand);
	commandList.emplace_back(make_shared<Script1_FullWriteAndReadCompare>());
}
void TestShell::run()
{
	while (exitCommand->isSystemActive()) {
		string input;
		displayPrompt();
		std::getline(std::cin, input);
		if (input.empty()) continue;

		auto tokens = split(input);
		std::string cmd = tokens[0];
		tokens.erase(tokens.begin());

		TestShell::PropmtInput promptInput{ cmd, tokens };

		ExcutePromptInput(promptInput);
	}
}
void TestShell::displayPrompt()
{
	cout << "SSDTestShell:>";
}

::TestShell::PropmtInput TestShell::getPromptInput()
{
	string lineInput = "";

	//std::cin.ignore();
	std::getline(std::cin, lineInput);

	return ::TestShell::PropmtInput();
}

bool TestShell::ExcutePromptInput(::TestShell::PropmtInput& promptInput)
{
	shared_ptr<ICommand> foundCommand = findCommand(promptInput.cmd);

	if (false == isValidPromptInput(foundCommand, promptInput))
		return false;

	foundCommand->Execute(promptInput.cmd, promptInput.args);

	return true;
}

bool TestShell::isValidPromptInput(std::shared_ptr<ICommand>& foundCommand, TestShell::PropmtInput& promptInput)
{
	if (foundCommand == nullptr)
		return false;
	if (false == foundCommand->isValidArguments(promptInput.cmd, promptInput.args))
		return false;
	return true;
}

shared_ptr<ICommand> TestShell::findCommand(const string& command)
{
	if (commandList.size() == 0)
		return nullptr;

	for (auto supported : commandList)
		if (command == supported->getCommandString())
			return supported;
	return nullptr;
}
