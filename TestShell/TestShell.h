#pragma once
#include <string>
#include <vector>
#include "ICommand.h"
#include "ExitCommand.h"
#include "HelpCommand.h"

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
	void run(); // main loop 시작
	void displayPrompt(); // propmt display
	::TestShell::PropmtInput getPromptInput();
	bool ExcutePromptInput(::TestShell::PropmtInput& promptInput);
	bool isValidPromptInput(std::shared_ptr<ICommand>& foundCommand, TestShell::PropmtInput& promptInput);
	shared_ptr<ICommand> findCommand(const string& command);
    // 명령어 파서 유틸
    std::vector<std::string> split(const std::string& line) {
        std::istringstream iss(line);
        std::vector<std::string> tokens;
        std::string token;
        while (iss >> token) tokens.push_back(token);
        return tokens;
    }

       private:
	vector<shared_ptr<ICommand>> commandList;
	shared_ptr<ExitCommand> exitCommand;
};

