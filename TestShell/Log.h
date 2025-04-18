#pragma once
#include <algorithm>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

class Logger {
public:
	std::string currentTimestamp();
	std::string formatLogMessage(const std::string& classname,const std::string& message);
	void archiveOldestIfNecessary();
	void log(const std::string& classname, const std::string& message);
  };


class LogBase  {
    public:
        LogBase() : log(nullptr) {}
        LogBase(Logger* logger) : log(logger) {}

    protected:
        Logger* log;

    void logMessage(const std::string& msg,const std::string& msg2) const {
      if (log)
        log->log(msg,msg2); 
    }
  };