#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <string>
#include <vector>
#include "TestShell.h"
#include "ICommand.h"

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
	helpCommand = make_shared<HelpCommand>();

	addCommand(exitCommand);
	addCommand(helpCommand);
}
void TestShell::run()
{
	while (exitCommand->isSystemActive()) {
		displayPrompt();
		TestShell::PropmtInput promptInput = getPromptInput();
		ExcutePromptInput(promptInput);
	}
}
void TestShell::displayPrompt()
{
	cout << "SSDTestShell:>";
}

::TestShell::PropmtInput TestShell::getPromptInput()
{
	::TestShell::PropmtInput promptInput;
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

	for (auto supported : commandList) {
		if (command == supported->getCommandString())
			return supported;
	}
	return nullptr;
}

void TestShell::addCommand(shared_ptr<ICommand> newCommand)
{
	commandList.emplace_back(newCommand);
	helpCommand->addHelp(newCommand->getUsage());
}
