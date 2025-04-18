#pragma once
#include <string>
#include <vector>
#include "ICommand.h"
#include "ExitCommand.h"
#include "HelpCommand.h"
#include "ReadCommand.h"
#include "WriteCommand.h"
#include "FullReadCommand.h"
#include "FullWriteCommand.h"
#include "EraseCommand.h"
#include "ErageRangeCommand.h"
#include "Script1.h"
#include "Script2.h"
#include "Script3.h"

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
	void addCommand(shared_ptr<ICommand> newCommand);

private:
	vector<shared_ptr<ICommand>> commandList;
	shared_ptr<ExitCommand> exitCommand;
	shared_ptr<HelpCommand> helpCommand;
	shared_ptr<ReadCommand> readCommand;
	shared_ptr<WriteCommand> writeCommand;
	shared_ptr<FullReadCommand> fullReadCommand;
	shared_ptr<FullWriteCommand> fullWriteCommand;
	shared_ptr<EraseCommand> eraseCommand;
	shared_ptr<ErageRangeCommand> erageRangeCommand;
	shared_ptr<Script1_FullWriteAndReadCompare> scriptCommand1;
	shared_ptr<Script2_PartialLBAWrite> scriptCommand2;
	shared_ptr<Script3_WriteReadAging> scriptCommand3;
};

