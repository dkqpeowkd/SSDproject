#include <iostream>
#include <string>
#include <regex>
#include <algorithm>

#include "SsdType.h"
#include "SsdInterface.h"

int main(int argc, char* argv[]) {
  SsdInterface ssdInterface;

  ssdInterface.ResetResult();

  try {
    // 명령어 개수 검사
    if (argc < 3) {
      throw std::invalid_argument("Insufficient arguments.");
    }

    std::string command = argv[1];
    std::string lbaStr = argv[2];

    std::transform(command.begin(), command.end(), command.begin(), ::toupper);

    if (command == "R") {
      if (argc != 3) {
        throw std::invalid_argument("Insufficient read arguments.");
      }

      ssdInterface.Read(lbaStr);
    } else if (command == "W") {
      if (argc != 4) {
        throw std::invalid_argument("Insufficient write arguments.");
      }

      std::string value = argv[3];

      ssdInterface.Write(lbaStr, value);
    } else {
      throw std::invalid_argument("Insufficient comment.");
    }
  } catch (const std::exception& errorMessage) {
    ssdInterface.InvalidCommand(errorMessage.what());
  }

  return (ssdInterface.GetResult() == ERROR_PATTERN);
}