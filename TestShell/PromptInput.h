#pragma once
#include <string>
#include <vector>

using std::string;
using std::vector;

class PromptInput {
public:
	string cmd;
	vector<string> args;
};