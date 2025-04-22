#pragma once

#include <algorithm>
#include <iostream>
#include "../TestShell.h"

int main(int argc, char* argv[]) {

	if (argc == 2) {
		TestShell testShell(false);

		std::string filename = argv[1];
		testShell.run(filename);
	} else {
		TestShell testShell(true);
		testShell.run();
	}
	return 0;
}

