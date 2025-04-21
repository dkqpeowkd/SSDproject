#pragma once
#include <string>
#include <vector>
#include "ICommand.h"
#include "PromptInput.h"
#include "ExitCommand.h"
#include "HelpCommand.h"
#include "ReadCommand.h"
#include "WriteCommand.h"
#include "FullReadCommand.h"
#include "FullWriteCommand.h"
#include "EraseCommand.h"
#include "EraseRangeCommand.h"
#include "FlushCommand.h"
#include "Log.h"

using std::string;
using std::vector;
using std::shared_ptr;
using std::make_shared;

class TestShell
{
public:
	TestShell();
	void run(); // main loop Ω√¿€
	void displayPrompt(); // propmt display
	PromptInput getPromptInput();
	bool ExcutePromptInput(PromptInput& promptInput);
	bool isValidPromptInput(std::shared_ptr<ICommand>& foundCommand, PromptInput& promptInput);
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
	shared_ptr<EraseRangeCommand> eraseRangeCommand;
	shared_ptr<FlushCommand> flushCommand;
    Logger log;
};

