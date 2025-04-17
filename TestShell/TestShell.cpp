#include <iostream>
#include "TestShell.h"
#include "ICommand.h"

using std::cout;
void TestShell::displayPrompt()
{
	cout << "SSDTestShell:>";
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
