#include "ReadCommand.h"
#include <cstdlib>

const std::string& ReadCommand::getCommandString() {
    return cmd;
}

bool ReadCommand::isMatch(const string& command)
{
    return cmd == command;
}

const std::string& ReadCommand::getUsage() {
    static std::string usage = "read <LBA>";
    return usage;
}

const std::string& ReadCommand::getDescription()
{
    static std::string description = "지정된 LBA 영역의 데이터를 읽습니다.";
    return description;
}

bool ReadCommand::isValidArguments(const std::string& cmd,
                                   std::vector<std::string>& args) {
  return args.size() == 1;
}

bool ReadCommand::Execute(const std::string& cmd,
                          std::vector<std::string>& args) {
    std::ostringstream oss;
    oss << "ssd.exe R " << args[0];
    logMessage("ReadCommand.Execute()", "[READ] %s", oss.str().c_str());

    int result = callSystem(oss.str());
    if (result == 1) {
        logMessage("ReadCommand.Execute()", "[READ] ERROR");
        return false;
    }

   std::string output = readOutput();
   logMessage("ReadCommand.Execute()", "[READ] %s", output.c_str());
   //std::cout << output << std::endl;

  if (args.size() >= 2) {
    // 사용자로부터 전달받은 패턴을 16진수로 변환
    uint32_t expected = std::stoul(args[1], nullptr, 16);
    // 출력 결과를 16진수로 변환
    uint32_t actual = std::stoul(output, nullptr, 16);

    // 비교 결과 출력
    if (expected != actual) {
      logMessage("ReadCommand.Execute()", "불일치: 기대값 0x%x 실제값 0x%x",
                 expected, actual);
      return false;
    }
  }
  return true;
}


int ReadCommand::callSystem(const std::string& cmd) {
    return system(cmd.c_str());
}

std::string ReadCommand::readOutput() {
    std::ifstream fin("ssd_output.txt");
    std::string line;
    std::getline(fin, line);
    return line;
}
