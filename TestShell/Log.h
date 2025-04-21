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
  std::string currentTimestamp();
  std::string Logger::formatLogMessage(const std::string& classname);
  void archiveOldestIfNecessary();
  void log(const std::string& prefix, const char* format, ...);
};
