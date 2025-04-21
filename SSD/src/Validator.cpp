#include "Validator.h"
#include <algorithm>
#include <cctype>

bool Validator::IsNumberWithinRange(const std::string& str, const int minNum,
    const int maxNum, bool isScope) {
  try {
    int num = std::stoi(str);

    if (isScope == true) num = std::abs(num);

    if (num >= minNum && num <= maxNum) {
      return true;
    } else {
      errorReason = "### Out of range ###";
      return false;
    }
  } catch (...) {
    errorReason = "### Not decimal ###";
    return false;
  }
}
  
bool Validator::IsValidDataPattern(const std::string& dataPattern) {
  if (dataPattern.length() != 10) {
    errorReason = "### DataPattern Length is not 10 ###";
    return false;
  }

  if (dataPattern[0] != '0' || dataPattern[1] != 'x') {
    errorReason = "### DataPattern Is not started with '0x' ###";
    return false;
  }

  auto isHexDigit = [](char c) {
    c = std::toupper(c);
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F');
  };

  if (std::all_of(dataPattern.begin() + 2, dataPattern.end(), isHexDigit) == false) {
    errorReason = "### DataPattern Is not Hex number ###";
    return false;
  }

  return true;
}
