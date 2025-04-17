#include "gmock/gmock.h"
#include "ICommand.h"
#include "TestShell.h"
using namespace ::testing;

TEST(TestShellMainLoopTS, displayPromptTC) {
	TestShell testShell;

	std::ostringstream replacedOutputBuffer;
	std::streambuf* prevcoutbuf;
	prevcoutbuf = std::cout.rdbuf(replacedOutputBuffer.rdbuf()); // 1,2

	testShell.displayPrompt();
	string console_output = replacedOutputBuffer.str();

	std::cout.rdbuf(prevcoutbuf);

	string expected = "SSDTestShell:>";

	EXPECT_EQ(expected, console_output);
}

TEST(TestShellMainLoopTs, FindCommandNegativeTC) {
	TestShell testShell;

	shared_ptr<ICommand> command = testShell.findCommand("test");

	EXPECT_THAT(command, IsNull());
}

TEST(TestShellMainLoopTs, FindCommandPositiveTC) {
	TestShell testShell;

	shared_ptr<ICommand> command = testShell.findCommand("exit");

	EXPECT_THAT(command, NotNull());
}



