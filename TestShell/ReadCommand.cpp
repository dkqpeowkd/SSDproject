
#include "ReadCommand.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <cstdlib>

// 문자열 분할 함수
std::vector<std::string> split(const std::string& line) {
    std::istringstream iss(line);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) tokens.push_back(token);
    return tokens;
}

// 실제 실행 함수
std::string ReadCommand::executeCommand(const std::string& input) {
    auto tokens = split(input);
    if (tokens.empty()) return "";

    const std::string& cmd = tokens[0];

    if (cmd == "read" && tokens.size() == 2) {
        int lba;
        try {
            lba = std::stoi(tokens[1]);
        }
        catch (...) {
            std::ofstream fout("ssd_output.txt");
            fout << "ERROR";
            fout.close();
            std::cerr << "ERROR" << std::endl;
            return "ERROR";
        }

        if (lba < 0 || lba >= 100) {
            std::ofstream fout("ssd_output.txt");
            fout << "ERROR";
            fout.close();
            std::cerr << "ERROR" << std::endl;
            return "ERROR";
        }

        std::string command = "ssd R " + std::to_string(lba);
        callSystem(command);

        std::string result = readOutput();
        std::cout << result << std::endl;
        return result;
    }

    return "INVALID COMMAND";
}

// 기본 system 호출
int ReadCommand::callSystem(const std::string& cmd) {
    return system(cmd.c_str());
}

// 기본 파일 읽기
std::string ReadCommand::readOutput() {
    std::ifstream fin("ssd_output.txt");
    std::string line;
    std::getline(fin, line);
    return line;
}

// 인터랙티브 입력 함수
void ReadCommand::run() {
    std::string input;
    while (true) {
        std::cout << "SSD> ";
        std::getline(std::cin, input);
        std::string result = executeCommand(input);
        if (result == "EXIT") break;
        std::cout << result << std::endl;
    }
}
