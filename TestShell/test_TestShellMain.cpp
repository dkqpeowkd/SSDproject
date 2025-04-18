#include "gmock/gmock.h"
#include "ICommand.h"
#include "TestShell.h"
#include "script_3_WriteReadAging.h"

using namespace ::testing;

class MockCommandTestDouble : public ICommand {
public:
	MockCommandTestDouble(string cmd, string usage) : cmd{ cmd }, usage{ usage } {}
	//MOCK_METHOD(const string&, getCommandString, (), (override));
	//MOCK_METHOD(const string&, getUsage, (), (override));

	const string& getCommandString() { return cmd; }
	const string& getUsage() { return usage; }
	MOCK_METHOD(bool, isValidArguments, (const string& cmd, vector<string>& args), (override));
	MOCK_METHOD(bool, Execute, (const string& cmd, vector<string>& args), (override));
private:
	string cmd;
	string usage;
};

class TestShellFixture : public Test {
protected:
	void SetUp() override {
		prevcoutbuf = std::cout.rdbuf(replacedOutputBuffer.rdbuf()); // 1,2
	}

	void TearDown() override {
		std::cout.rdbuf(prevcoutbuf);
	}
public:
	TestShell testShell;
	std::ostringstream replacedOutputBuffer;
	std::streambuf* prevcoutbuf;

	string getConsoleOutput() {
		replacedOutputBuffer.str();
	}

	shared_ptr<MockCommandTestDouble> getCommandTestDouble(string cmd, string usage) {
		return make_shared<MockCommandTestDouble>(cmd, usage );
		//MockCommandTestDouble command{ cmd, usage };

		//EXPECT_CALL(command, isValidArguments).WillRepeatedly(Return(true));
		//EXPECT_CALL(command, Execute).WillRepeatedly(Return(true));

		//return command;
	}
};

TEST_F(TestShellFixture, displayPromptTC) {
	testShell.displayPrompt();
	string console_output = replacedOutputBuffer.str();

	string expected = "SSDTestShell:>";
	EXPECT_EQ(expected, console_output);
}

TEST_F(TestShellFixture, FindCommandNegativeTC) {

	shared_ptr<ICommand> command = testShell.findCommand("test");

	EXPECT_THAT(command, IsNull());
}

TEST_F(TestShellFixture, FindCommandPositiveTC) {

	shared_ptr<ICommand> exitCommand = testShell.findCommand("exit");

	EXPECT_THAT(exitCommand, NotNull());

	shared_ptr<ICommand> helpCommand = testShell.findCommand("help");

	EXPECT_THAT(helpCommand, NotNull());
}

TEST_F(TestShellFixture, CommandValidationPositiveTC) {
	PromptInput promptInput{ "exit", {} };

	bool status = testShell.ExcutePromptInput(promptInput);
	bool expected = true;
	EXPECT_EQ(expected, status);
}

TEST_F(TestShellFixture, CommandArgValidationNegativeTC) {
	PromptInput promptInput{ "exit", {"dummyArg"}};

	shared_ptr<ICommand> foundCommand = testShell.findCommand("exit");
	bool status = testShell.isValidPromptInput(foundCommand, promptInput);
	bool expected = false;
	EXPECT_EQ(expected, status);
}

TEST_F(TestShellFixture, CommandArgValidationPositiveTC) {
	PromptInput promptInput{ "exit", {} };

	shared_ptr<ICommand> foundCommand = testShell.findCommand("exit");
	bool status = testShell.isValidPromptInput(foundCommand, promptInput);
	bool expected = true;
	EXPECT_EQ(expected, status);
}

TEST_F(TestShellFixture, AddCommandTC) {
	shared_ptr<MockCommandTestDouble> writeCommand = getCommandTestDouble("write", "writeUsage");

	testShell.addCommand(writeCommand);

	shared_ptr<ICommand> command = testShell.findCommand("write");

	EXPECT_THAT(command, NotNull());
}

TEST_F(TestShellFixture, AddScriptCommandTC) {
	shared_ptr<MockCommandTestDouble> newCommand = getCommandTestDouble("3_WriteReadAging", "3_WriteReadAging Usage");

	testShell.addCommand(newCommand);
	shared_ptr<ICommand> command = testShell.findCommand("3_WriteReadAging");

	EXPECT_THAT(command, NotNull());
}

TEST_F(TestShellFixture, AddScripCommandInvalidArgTC) {

	string cmd = "3_WriteReadAging";
	vector<string> args = {"dummyArgs"};
	shared_ptr<MockCommandTestDouble> newCommand = getCommandTestDouble(cmd, "3_WriteReadAging Usage");

	EXPECT_CALL(*newCommand, Execute(cmd, args)).WillRepeatedly(Return(false));

	bool expected = false;

	EXPECT_EQ(expected, newCommand->Execute(cmd, args));
}

TEST_F(TestShellFixture, AddScripCommandValidArgTC) {

	string cmd = "3_WriteReadAging";
	vector<string> args = {};
	shared_ptr<MockCommandTestDouble> newCommand = getCommandTestDouble(cmd, "3_WriteReadAging Usage");

	EXPECT_CALL(*newCommand, Execute(cmd, args)).WillRepeatedly(Return(true));

	bool expected = true;

	EXPECT_EQ(expected, newCommand->Execute(cmd, args));
}
/*
TEST_F(TestShellFixture, PromptInputTC) {
	std::streambuf* cinbuf = std::cin.rdbuf();
	string input = "exit\n";

	std::istringstream iss(input);
	std::cin.rdbuf(iss.rdbuf());

	std::cin.rdbuf(cinbuf);	

	EXPECT_EQ(input, testShell.getPromptInput().cmd);
}
*/

