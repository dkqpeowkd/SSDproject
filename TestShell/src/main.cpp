#pragma once

#include <algorithm>
#include <iostream>
#include "../TestShell.h"

int main(int argc, char* argv[]) {

	if (argc == 2) {
		TestShell testShell(::Logger::DISABLE_LOG);
		std::string filename = argv[1];
		testShell.run(filename);
	} else {
		TestShell testShell(::Logger::ENABLE_LOG);
		testShell.run();
	}
	return 0;
}

