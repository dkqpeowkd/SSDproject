#pragma once
#include <algorithm>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <cstdarg>
#include <cstdio>

namespace fs = std::filesystem;

class Logger {
 public:
	static const bool ENABLE_LOG = true;
	static const bool DISABLE_LOG = true;

	Logger() {}
	Logger(bool isLogEnable) : isLogEnable{ isLogEnable } {}
	std::string currentTimestamp();
	std::string Logger::formatLogMessage(const std::string& classname);
	void archiveOldestIfNecessary();
	void log(const std::string& prefix, const char* format, ...);
private:
	bool isLogEnable = false;
};
