#pragma once
#include <string>

class Recoder {
 public:
  void recordErrorPatternToOutputFile(const std::string& errorMessage);
  void recordZeroPatternToOutputFile();
  void recordSuccessPatternToOutputFile(const std::string& value);
  std::string getResult() { return output; };

  private:
  std::string output = "0x00000000";
};