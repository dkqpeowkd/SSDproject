#include "Recoder.h"
#include "SsdType.h"
#include <string>
#include <fstream>
#include <cctype>
#include <sstream>
#include <iomanip>

void Recoder::recordZeroPatternToOutputFile() {
  output = ZERO_PATTERN;

  std::ofstream file(OUTPUT_FILE_NAME, std::ios::trunc);
  if (file.is_open()) {
    file << output << "\n";
  }
}

void Recoder::recordErrorPatternToOutputFile(const std::string& errorMessage) {
  output = ERROR_PATTERN;

  std::ofstream file(OUTPUT_FILE_NAME, std::ios::trunc);
  if (file.is_open()) {
    file << output << "\n";
#ifdef _DEBUG
    file << errorMessage << "\n";
#endif
  }
}

void Recoder::recordSuccessPatternToOutputFile(const std::string& value) {
  output = value;

  std::ofstream file(OUTPUT_FILE_NAME, std::ios::trunc);
  if (file.is_open()) {
    file << output << "\n";
  }
}