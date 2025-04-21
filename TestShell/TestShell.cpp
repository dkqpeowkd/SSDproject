#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <vector>
#include "TestShell.h"
#include "ICommand.h"
#include "ReadCommand.h"
#include "WriteCommand.h"
#include "FullReadCommand.h"
#include "FullWriteCommand.h"
#include "EraseCommand.h"
#include "EraseRangeCommand.h"
#include "FlushCommand.h"
#include "MetaCommandContainer.h"
#include <Windows.h>

using std::cout;

// 명령어 파서 유틸
std::vector<std::string> split(const std::string& line) {
	std::istringstream iss(line);
	std::vector<std::string> tokens;
	std::string token;
	while (iss >> token) tokens.push_back(token);
	return tokens;
}
TestShell::TestShell(bool EnableLog) {
  if (EnableLog == true) {
    exitCommand = make_shared<ExitCommand>(&log);
    helpCommand = make_shared<HelpCommand>(&log);
    readCommand = make_shared<ReadCommand>(&log);
    writeCommand = make_shared<WriteCommand>(&log);
    fullReadCommand = make_shared<FullReadCommand>(&log);
    fullWriteCommand = make_shared<FullWriteCommand>(&log);
    eraseCommand = make_shared<EraseCommand>(&log);
    eraseRangeCommand = make_shared<EraseRangeCommand>(&log);
    flushCommand = make_shared<FlushCommand>(&log);
  }

  addCommand(exitCommand);
  addCommand(helpCommand);
  addCommand(readCommand);
  addCommand(writeCommand);
  addCommand(fullReadCommand);
  addCommand(fullWriteCommand);
  addCommand(eraseCommand);
  addCommand(eraseRangeCommand);
  addCommand(flushCommand);

  

  helpCommand->addSupportedCommand(commandList);
}

TestShell::TestShell()
{
	exitCommand = make_shared<ExitCommand>();
	helpCommand = make_shared<HelpCommand>();
	readCommand = make_shared<ReadCommand>();
	writeCommand = make_shared<WriteCommand>();
	fullReadCommand = make_shared<FullReadCommand>();
	fullWriteCommand = make_shared<FullWriteCommand>();
	eraseCommand = make_shared<EraseCommand>();
	eraseRangeCommand = make_shared<EraseRangeCommand>();
	flushCommand = make_shared<FlushCommand>();


	addCommand(exitCommand);
	addCommand(helpCommand);
	addCommand(readCommand);
	addCommand(writeCommand);
	addCommand(fullReadCommand);
	addCommand(fullWriteCommand);
	addCommand(eraseCommand);
	addCommand(eraseRangeCommand);
	addCommand(flushCommand);

	helpCommand->addSupportedCommand(commandList);
}
void TestShell::run()
{
	while (exitCommand->isSystemActive()) {
		displayPrompt();
		PromptInput promptInput = getPromptInput();
		ExcutePromptInput(promptInput);
	}
}
std::filesystem::path getExecutableDir() {
  char path[MAX_PATH];
  GetModuleFileNameA(NULL, path, MAX_PATH);
  return std::filesystem::path(path).parent_path();
}

void TestShell::run(const std::string filename) {
  std::filesystem::path exeDir = getExecutableDir();
  std::filesystem::path filePath = exeDir / filename;
  std::ifstream infile(filePath);

  std::string line;
  if (std::getline(infile, line)) {
    // BOM 제거
    if (line.size() >= 3 && static_cast<unsigned char>(line[0]) == 0xEF &&
        static_cast<unsigned char>(line[1]) == 0xBB &&
        static_cast<unsigned char>(line[2]) == 0xBF) {
      line = line.substr(3);
    }
    std::cout << line << "      ___   Run...";
  }

  shared_ptr<ICommand> foundCommand = findCommand(line);
  PromptInput promptInput;
  promptInput.cmd = line;

  if (foundCommand == nullptr) {
    MetaCommandContainer scriptContainer;
    scriptContainer.loadMetaScript(commandList);
    shared_ptr<ICommand> scriptCommand =
        scriptContainer.getScriptCommand(promptInput.cmd, commandList);

    if (scriptCommand == nullptr) {
      std::cout << "Fail" << std::endl;
      return;
	}


    if (scriptCommand->Execute(promptInput.cmd, promptInput.args) == false) {
          std::cout << "Fail" << std::endl;
		return;

        }
  }

    std::cout << "Pass" << std::endl;


  while (std::getline(infile, line)) {
    std::cout << line << "      ___   Run...";
    foundCommand = findCommand(line);
    promptInput.cmd = line;
    if (foundCommand == nullptr) {
      MetaCommandContainer scriptContainer;
      scriptContainer.loadMetaScript(commandList);
      shared_ptr<ICommand> scriptCommand =
          scriptContainer.getScriptCommand(promptInput.cmd, commandList);

      if (scriptCommand == nullptr) {
        std::cout << "Fail" << std::endl;
        return;
      }

      if (scriptCommand->Execute(promptInput.cmd, promptInput.args) == false) {
        std::cout << "Fail" << std::endl;
        return;
      }
    }
    std::cout << "Pass" << std::endl;
  }

  infile.close();  // 파일 닫기
}
void TestShell::displayPrompt()
{
	cout << "SSDTestShell:>";
}

PromptInput TestShell::getPromptInput()
{
	PromptInput promptInput;
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

bool TestShell::ExcutePromptInput(PromptInput& promptInput)
{
	shared_ptr<ICommand> foundCommand = findCommand(promptInput.cmd);

	if (foundCommand == nullptr) {
		MetaCommandContainer scriptContainer;
		scriptContainer.loadMetaScript(commandList);
		shared_ptr<ICommand> scriptCommand = scriptContainer.getScriptCommand(promptInput.cmd, commandList);

		if (scriptCommand == nullptr)
			return false;

		if (scriptCommand->Execute(promptInput.cmd, promptInput.args) == false)
			return false;
		return true;
	}

	if (false == isValidPromptInput(foundCommand, promptInput)) {
		std::cout << "INVALID COMMAND" << std::endl;
		return false;
	}

	bool executed = foundCommand->Execute(promptInput.cmd, promptInput.args);
	if (!executed) {
		return false;
	}

	return true;
}

bool TestShell::isValidPromptInput(std::shared_ptr<ICommand>& foundCommand, PromptInput& promptInput)
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
		if (supported->isMatch(command))
			return supported;
	}
	return nullptr;
}

void TestShell::addCommand(shared_ptr<ICommand> newCommand)
{
	commandList.emplace_back(newCommand);
	//helpCommand->addHelp(newCommand->getUsage());

}
