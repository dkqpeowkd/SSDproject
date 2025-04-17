#pragma once
#include <string>
#include <vector>
#include "ICommand.h"
#include "ExitCommand.h"

using std::string;
using std::vector;
using std::shared_ptr;
using std::make_shared;

class TestShell
{
public:
	struct PropmtInput {
		string cmd;
		vector<string> args;
	};
public:
	TestShell();
	void run(); // main loop Ω√¿€
	void displayPrompt(); // propmt display
	::TestShell::PropmtInput getPromptInput();
	bool ExcutePromptInput(::TestShell::PropmtInput& promptInput);
	bool isValidPromptInput(std::shared_ptr<ICommand>& foundCommand, TestShell::PropmtInput& promptInput);
	shared_ptr<ICommand> findCommand(const string& command);

private:
	vector<shared_ptr<ICommand>> commandList;
	ExitCommand *exitCommand;
};

