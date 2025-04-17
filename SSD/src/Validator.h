#pragma once
#include <string>

class Validator {
 public:
  bool isValidNumberZeroToNintyNine(const std::string& lbaStr);
  bool isValidDataPattern(const std::string& pattern) ;
  std::string getErrorReason() { return errorReason; };
  void setErrorReason(const std::string& errorMessage) {
    errorReason = errorMessage;
  };

  private:
  std::string errorReason;

};