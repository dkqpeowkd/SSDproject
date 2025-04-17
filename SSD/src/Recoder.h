#pragma once
#include <string>

class Recoder {
 public:
  void recordErrorPatternToOutputFile(const std::string& errorMessage);
  void recordZeroPatternToOutputFile();
  void recordSuccessPatternToOutputFile(const std::string& value);

  void resetResult() { output = INIT_OUTPUT; };
  std::string getResult() { return output; };

private:
  const std::string INIT_OUTPUT = "0x00000000";

  std::string output = INIT_OUTPUT;
};