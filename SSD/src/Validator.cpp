#include "Validator.h"

bool Validator::isValidNumberZeroToNintyNine(const std::string& str) {
  try {
    int num = std::stoi(str);

    if (num >= 0 && num <= 99) {
      return true;
    } else {
      errorReason = "### LBA is out of range (0~99) ###";
      return false;
    }
  } catch (...) {
    errorReason = "### LBA is not decimal ###";
    return false;
  }
}


  
bool Validator::isValidDataPattern(const std::string& dataPattern) {
  if (dataPattern.length() != 10) {
    errorReason = "### DataPattern Length is not 10 ###";
    return false;
  }

  if (dataPattern[0] != '0' || dataPattern[1] != 'x') {
    errorReason = "### DataPattern Is not started with '0x' ###";
    return false;
  }

  for (size_t i = 2; i < dataPattern.length(); ++i) {
    char c = dataPattern[i];

    if (c >= '0' && c <= '9') {
      continue;
    }

    if (c >= 'a' && c <= 'f') {
      continue;
    }

    if (c >= 'A' && c <= 'F') {
      continue;
    }

    errorReason = "### DataPattern Is not Hex number ###";
    return false;
  }

  return true;
}
