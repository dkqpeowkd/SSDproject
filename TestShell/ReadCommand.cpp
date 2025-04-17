
#include "ReadCommand.h"

int ReadCommand::callSystem(const std::string& cmd) {
    return system(cmd.c_str());
}

std::string ReadCommand::readOutput() {
    std::ifstream fin("ssd_output.txt");
    std::string line;
    std::getline(fin, line);
    return line;
}

bool ReadCommand::Execute(const std::string& cmd, std::vector<std::string>& args) {
    int lba = std::stoi(args[1]);
    std::string command = "ssd R " + std::to_string(lba);
    callSystem(command);

    std::string result = readOutput();
    std::cout << result << std::endl;
    return true;
}
