
#include "TestShell.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <cstdlib>

// ���ڿ� ���� �Լ�
std::vector<std::string> split(const std::string& line) {
    std::istringstream iss(line);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) tokens.push_back(token);
    return tokens;
}

// ���� ���� �Լ�
std::string TestShell::executeCommand(const std::string& input) {
    auto tokens = split(input);
    if (tokens.empty()) return "";

    const std::string& cmd = tokens[0];

    if (cmd == "exit") return "EXIT";
    if (cmd == "help") return "HELP";

    if (cmd == "write" && tokens.size() == 3) {
        std::string command = "ssd W " + tokens[1] + " " + tokens[2];
        callSystem(command);
        return "WRITE " + tokens[1] + " " + tokens[2];
    }

    if (cmd == "read" && tokens.size() == 2) {
        int lba = std::stoi(tokens[1]);
        if (lba < 0 || lba >= 100) {
            std::ofstream fout("ssd_output.txt");
            fout << "ERROR";
            fout.close();
            std::cerr << "ERROR" << std::endl;
            return "ERROR";
        }

        std::string command = "ssd R " + tokens[1];
        callSystem(command);

        std::string result = readOutput();
        std::cout << result << std::endl;
        return result;
    }

    if (cmd == "fullwrite" && tokens.size() == 2) {
        for (int i = 0; i < 100; ++i) {
            std::string command = "ssd W " + std::to_string(i) + " " + tokens[1];
            callSystem(command);
        }
        return "FULLWRITE";
    }

    if (cmd == "fullread" && tokens.size() == 1) {
        for (int i = 0; i < 100; ++i) {
            std::string command = "ssd R " + std::to_string(i);
            callSystem(command);

            std::string result = readOutput();
            std::cout << "LBA " << i << ": " << result << std::endl;
        }
        return "FULLREAD";
    }

    if (cmd.rfind("1_", 0) == 0 || cmd.rfind("2_", 0) == 0 || cmd.rfind("3_", 0) == 0) {
        return "SCRIPT " + cmd;
    }

    return "INVALID COMMAND";
}

// �⺻ system ȣ��
int TestShell::callSystem(const std::string& cmd) {
    return system(cmd.c_str());
}

// �⺻ ���� �б�
std::string TestShell::readOutput() {
    std::ifstream fin("ssd_output.txt");
    std::string line;
    std::getline(fin, line);
    return line;
}

// ���ͷ�Ƽ�� �Է� �Լ�
void TestShell::run() {
    std::string input;
    while (true) {
        std::cout << "SSD> ";
        std::getline(std::cin, input);
        std::string result = executeCommand(input);
        if (result == "EXIT") break;
        std::cout << result << std::endl;
    }
}
