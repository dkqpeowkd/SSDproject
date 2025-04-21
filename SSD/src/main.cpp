#include <algorithm>
#include <iostream>
#include <memory>
#include <string>

#include "RealSsdComponentFactory.h"
#include "SsdController.h"
#include "SsdType.h"

// 상수 정의
const std::string CMD_READ = "R";
const std::string CMD_WRITE = "W";
const std::string CMD_ERASE = "E";
const std::string CMD_FLUSH = "F";
const std::string ERR_INSUFFICIENT_ARGS = "Insufficient arguments.";
const std::string ERR_INSUFFICIENT_READ_ARGS = "Insufficient read arguments.";
const std::string ERR_INSUFFICIENT_WRITE_ARGS = "Insufficient write arguments.";
const std::string ERR_INSUFFICIENT_ERASE_ARGS = "Insufficient erase arguments.";
const std::string ERR_INSUFFICIENT_FLUSH_ARGS = "Insufficient flush arguments.";
const std::string ERR_INVALID_COMMAND = "Invalid command.";

void processReadCommand(SsdController& controller, int argc, char* argv[]) {
  if (argc != 3) {
    throw std::invalid_argument(ERR_INSUFFICIENT_READ_ARGS);
  }
  controller.Read(argv[2]);
}

void processWriteCommand(SsdController& controller, int argc, char* argv[]) {
  if (argc != 4) {
    throw std::invalid_argument(ERR_INSUFFICIENT_WRITE_ARGS);
  }
  controller.Write(argv[2], argv[3]);
}

void processEraseCommand(SsdController& controller, int argc, char* argv[]) {
  if (argc != 4) {
    throw std::invalid_argument(ERR_INSUFFICIENT_ERASE_ARGS);
  }
  controller.Erase(argv[2], argv[3]);
}

void processFlushCommand(SsdController& controller, int argc, char* argv[]) {
  if (argc != 2) {
    throw std::invalid_argument(ERR_INSUFFICIENT_FLUSH_ARGS);
  }
  controller.Flush();
}

int main(int argc, char* argv[]) {
  std::unique_ptr<SsdComponentFactory> realFactory =
      std::make_unique<RealSsdComponentFactory>();
  SsdController ssdControllerWithReal(std::move(realFactory));

  ssdControllerWithReal.ResetResult();

  try {
    if (argc < 2) {
      throw std::invalid_argument(ERR_INSUFFICIENT_ARGS);
    }

    std::string command = argv[1];
    std::transform(command.begin(), command.end(), command.begin(), ::toupper);

    if (command == CMD_READ) {
      processReadCommand(ssdControllerWithReal, argc, argv);
    } else if (command == CMD_WRITE) {
      processWriteCommand(ssdControllerWithReal, argc, argv);
    } else if (command == CMD_ERASE) {
      processEraseCommand(ssdControllerWithReal, argc, argv);
    } else if (command == CMD_FLUSH) {
      processFlushCommand(ssdControllerWithReal, argc, argv);
    } else {
      throw std::invalid_argument(ERR_INVALID_COMMAND);
    }
  } catch (const std::exception& errorMessage) {
    ssdControllerWithReal.InvalidCommand(errorMessage.what());
  }

  bool isError = (ssdControllerWithReal.GetResult() == ERROR_PATTERN);
  return isError;
}