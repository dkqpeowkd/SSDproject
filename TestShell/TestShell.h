#pragma once
#include <string>
#include <vector>
#include "ICommand.h"

using std::string;
using std::vector;
using std::shared_ptr;

class TestShell
{
public:
	struct PropmtInput {
		string cmd;
		vector<string> args;
	};
public:
	void run(); // main loop Ω√¿€
	void displayPrompt(); // propmt display
	::TestShell::PropmtInput getPromptInput();
	bool ExcutePromptInput(::TestShell::PropmtInput& promptInput);
	shared_ptr<ICommand> findCommand(const string& command);

private:
	vector<shared_ptr<ICommand>> commandList;
};

