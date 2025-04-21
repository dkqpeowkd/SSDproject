#include "Recoder.h"
#include "SsdType.h"

#include <fstream>
#include <string>

void Recoder::writeToOutputFile(const std::string& content, const std::string& errorMessage) {
  output = content;

  std::ofstream outputFile(OUTPUT_FILE_NAME, std::ios::trunc);
  if (outputFile.is_open()) {
    outputFile << content << "\n";
#ifdef _DEBUG
    if (!errorMessage.empty()) {
      outputFile << errorMessage << "\n";
    }
#endif
  }
}

void Recoder::RecordZeroPatternToOutputFile() {
  writeToOutputFile(ZERO_PATTERN);
}

void Recoder::RecordErrorPatternToOutputFile(const std::string& errorMessage) {
  writeToOutputFile(ERROR_PATTERN, errorMessage);
}

void Recoder::RecordSuccessPatternToOutputFile(const std::string& value) {
  writeToOutputFile(value);
}