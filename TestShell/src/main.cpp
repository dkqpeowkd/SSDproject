#pragma once
#define FIRST_RELEASE (1)

#if FIRST_RELEASE

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
//#include "../TestShell.h"

// 명령어 파서 유틸
std::vector<std::string> split(const std::string& line) {
  std::istringstream iss(line);
  std::vector<std::string> tokens;
  std::string token;
  while (iss >> token) tokens.push_back(token);
  return tokens;
}

// 명령 실행 함수들 (임시 stub)
void handleWrite(const std::vector<std::string>& args) {
  // int value = std::stoi(args[1], nullptr, 10);
  // string pattern = std::stoul(args[2], nullptr, 16);
  std::ostringstream oss;

  // oss << "SSD.exe w " << value << " " << args[2];
  oss << "ssd.exe W " << args[1] << " " << args[2];
  std::cout << oss.str() << std::endl;
  system(oss.str().c_str());
}

void handleRead(const std::vector<std::string>& args) {
  int value = std::stoi(args[1], nullptr, 10);
  std::ostringstream oss;

  oss << "ssd.exe R " << value;
  std::cout << oss.str() << std::endl;

  system(oss.str().c_str());
  // debug
  oss.str("");
  oss.clear();

  oss << "type ssd_output.txt";
  system(oss.str().c_str());
}

void handleFullWrite(const std::vector<std::string>& args) {
  int pattern = std::stoul(args[1], nullptr, 16);
  std::ostringstream oss;

  for (int LBA = 0; LBA < 100; LBA++) {
    oss.str("");
    oss.clear();
    oss << "SSD.exe W " << LBA << " " << args[1];
    std::cout << oss.str() << std::endl;

    system(oss.str().c_str());
  }
}

void handleFullRead() {
  std::ostringstream oss;

  for (int LBA = 0; LBA < 100; LBA++) {
    oss.str("");
    oss.clear();
    oss << "SSD.exe R " << LBA;
    std::cout << oss.str() << std::endl;

    system(oss.str().c_str());

    // debug
    oss.str("");
    oss.clear();

    oss << "type ssd_output.txt";
    system(oss.str().c_str());
  }
}
void handleHelp() {
  std::cout << "Usage: " << "SSD TEST SHELL" << " [OPTIONS]\n"
            << "\n"
            << "Developed by: Team BestAttitude (Changwan.Ha, Hyunkoo.kim  "
               "Hyoeun.Park  Chanwook.Lee  Ted.Kim)\n";
}

void handleScript(const std::string& cmd) {
  std::cout << "[CALL] Test Script: " << cmd << "\n";
}

bool PartialLBAWrite() {
  std::ostringstream oss;
  int pattern = 0x11111111;

  for (int nLoop = 0; nLoop < 30; nLoop++) {
    oss << "ssd.exe write " << 4 << " " << std::hex << pattern;
    oss << "ssd.exe write " << 0 << " " << std::hex << pattern;
    oss << "ssd.exe write " << 3 << " " << std::hex << pattern;
    oss << "ssd.exe write " << 1 << " " << std::hex << pattern;
    oss << "ssd.exe write " << 2 << " " << std::hex << pattern;
  }
  return 1;
}

int main() {
  std::string input;

  while (true) {
    std::cout << "SHELL> ";
    std::getline(std::cin, input);
    if (input.empty()) continue;

    auto tokens = split(input);
    std::string cmd = tokens[0];

    if (cmd == "exit") {
      break;
    } else if (cmd == "write" && tokens.size() == 3) {
      handleWrite(tokens);
    } else if (cmd == "read" && tokens.size() == 2) {
      handleRead(tokens);
    } else if (cmd == "fullwrite" && tokens.size() == 2) {
      handleFullWrite(tokens);
    } else if (cmd == "fullread" && tokens.size() == 1) {
      handleFullRead();
    } else if (cmd == "help") {
      handleHelp();
    } else if (cmd.rfind("1", 0) == 0 || cmd.rfind("2", 0) == 0 ||
               cmd.rfind("3_", 0) == 0) {
      handleScript(cmd);
    } else {
      std::cout << "INVALID COMMAND\n";
    }
  }

  std::cout << "Bye~ \n";
  return 0;
}

#else

#include <algorithm>
#include <iostream>
#include "../TestShell.h"

int main() {
  // std::cout << "this is source main" << std::endl;
  TestShell testShell;

  testShell.run();
  return 0;
}
#endif

