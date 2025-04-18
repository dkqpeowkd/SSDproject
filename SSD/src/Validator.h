#pragma once
#include <string>

class Validator {
 public:
  bool IsNumberWithinRange(const std::string& str, const int minNum,
                           const int maxNum);
  bool IsValidDataPattern(const std::string& pattern);
  std::string GetErrorReason() { return errorReason; };
  void SetErrorReason(const std::string& errorMessage) {
    errorReason = errorMessage;
  };

  private:
  std::string errorReason;

};