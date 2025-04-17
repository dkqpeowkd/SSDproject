#include "gmock/gmock.h"
#include "ICommand.h"
#include "TestShell.h"
using namespace ::testing;

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

	shared_ptr<ICommand> command = testShell.findCommand("exit");

	EXPECT_THAT(command, NotNull());
}

TEST_F(TestShellFixture, CommandValidationPositiveTC) {
	TestShell::PropmtInput promptInput{ "exit", {} };

	bool status = testShell.ExcutePromptInput(promptInput);
	bool expected = true;
	EXPECT_EQ(expected, status);
}

TEST_F(TestShellFixture, CommandArgValidationNegativeTC) {
	TestShell::PropmtInput promptInput{ "exit", {"dummyArg"}};

	bool status = testShell.ExcutePromptInput(promptInput);
	bool expected = false;
	EXPECT_EQ(expected, status);
}



