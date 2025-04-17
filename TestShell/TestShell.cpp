#pragma once
#if 1
#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <vector>
#include "TestShell.h"
#include "ICommand.h"
#include "Script1.h"
#include "Script2.h"

using std::cout;

TestShell::TestShell()
{
	exitCommand = make_shared<ExitCommand>();
	commandList.emplace_back(exitCommand);
	shared_ptr<HelpCommand> helpCommand = make_shared<HelpCommand>();
	commandList.emplace_back(helpCommand);
	commandList.emplace_back(make_shared<Script1_FullWriteAndReadCompare>());
	commandList.emplace_back(make_shared<Script2_PartialLBAWrite>());

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

	if (false == isValidPromptInput(foundCommand, promptInput)) {
		std::cout << "INVALID COMMAND" << std::endl;
		return false;
	}

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
	if (commandList.empty()) return nullptr;

	for (auto& supported : commandList) {
		const std::string& registered = supported->getCommandString();
		// ���� ��ġ or ��ϵ� ��ɾ "1_FullWriteAndReadCompare"�ε� "1_"ó�� �ٿ��� ��� ���
		if (command == registered || registered.rfind(command, 0) == 0) {
			return supported;
		}
	}
	return nullptr;
}
#endif