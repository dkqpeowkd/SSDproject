#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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
  int value = std::stoi(args[1], nullptr, 10);
  int pattern = std::stoul(args[2], nullptr, 16);
  std::ostringstream oss;

  oss << "ssd.exe write " << value << " " << std::hex << pattern;

  system(oss.str().c_str());
 
}

void handleRead(const std::vector<std::string>& args) {
  int value = std::stoi(args[1], nullptr, 10);
  std::ostringstream oss;

  oss << "ssd.exe read " << value;

  system(oss.str().c_str());
}

void handleFullWrite(const std::vector<std::string>& args) {
    int pattern = std::stoul(args[1], nullptr, 16);
    std::ostringstream oss;

    for (int LBA = 0; LBA < 99; LBA++) {
      oss << "ssd.exe write " << LBA << " " << std::hex << pattern;

      system(oss.str().c_str());
    }

  }

void handleFullRead() {
    std::ostringstream oss;

    for (int LBA = 0; LBA < 99; LBA++) {
        oss << "ssd.exe read " << LBA;
        system(oss.str().c_str());
    }
  }
  void handleHelp() {

   std::cout << "Usage: " << "SSD TEST SHELL" << " [OPTIONS]\n"
            << "\n"
            << "Developed by: Team BestAttitude (Changwan.Ha, Hyunkoo.kim  Hyoeun.Park  Chanwook.Lee  Ted.Kim)\n";

  }

void handleScript(const std::string& cmd) {
  std::cout << "[CALL] Test Script: " << cmd << "\n";
}

int main() {
  std::string input;

  while (true) {
    std::cout << "SSD TEST SHELL> ";
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