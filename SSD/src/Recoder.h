#pragma once
#include <string>

class Recoder {
 public:
  void RecordErrorPatternToOutputFile(const std::string& errorMessage);
  void RecordZeroPatternToOutputFile();
  void RecordSuccessPatternToOutputFile(const std::string& value);

  void ResetResult() { output = INIT_OUTPUT; };
  std::string GetResult() { return output; };

private:
  const std::string INIT_OUTPUT = "0x00000000";

  std::string output = INIT_OUTPUT;

  void writeToOutputFile(const std::string& content, const std::string& errorMessage = "");
};