#include <iostream>
#include "TestShell.h"
#include "ICommand.h"

using std::cout;
TestShell::TestShell()
{
	commandList.emplace_back(make_shared<ExitCommand>());
	commandList.emplace_back(make_shared<HelpCommand>());
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
