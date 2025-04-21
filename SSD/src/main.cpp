#include <iostream>
#include <string>
#include <regex>
#include <algorithm>

#include "SsdType.h"
#include "SsdController.h"
#include "RealSsdComponentFactory.h"

int main(int argc, char* argv[]) {
  std::unique_ptr<SsdComponentFactory> realFactory =
      std::make_unique<RealSsdComponentFactory>();
  SsdController ssdControllerWithReal(std::move(realFactory));

  ssdControllerWithReal.ResetResult();

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

      ssdControllerWithReal.Read(lbaStr);
    } else if (command == "W") {
      if (argc != 4) {
        throw std::invalid_argument("Insufficient write arguments.");
      }

      std::string value = argv[3];

      ssdControllerWithReal.Write(lbaStr, value);
    } else if (command == "E") {
      if (argc != 4) {
        throw std::invalid_argument("Insufficient erase arguments.");
      }

      std::string scope = argv[3];

      ssdControllerWithReal.Erase(lbaStr, scope);
    } else if (command == "F") {
      if (argc != 2) {
        throw std::invalid_argument("Insufficient flush arguments.");
      }

      ssdControllerWithReal.Flush();
    } else {
      throw std::invalid_argument("Insufficient comment.");
    }
  } catch (const std::exception& errorMessage) {
    ssdControllerWithReal.InvalidCommand(errorMessage.what());
  }

  return (ssdControllerWithReal.GetResult() == ERROR_PATTERN);
}