#include "SsdInterface.h"

bool SsdInterface::Write(std::string lba, std::string dataPattern) {
  return true;
}

void SsdInterface::Read(std::string lba) { 
  if (isValidNumberZeroToNintyNine(lba) == false) return recordErrorPatternToOutputFile();
  return  recordZeroPatternToOutputFile(); 
  
        
  
}

std::string SsdInterface::GetResult() { return output; }

void SsdInterface::recordZeroPatternToOutputFile() {
  output = ZERO_PATTERN;

  std::ofstream file(OUTPUT_FILE_NAME, std::ios::trunc); 
  if (file.is_open()) {
    file << output << "\n";
  }
}

void SsdInterface::recordErrorPatternToOutputFile() {
  output = ERROR_PATTERN;

  std::ofstream file(OUTPUT_FILE_NAME, std::ios::trunc);
  if (file.is_open()) {
    file << output << "\n";
  }
}

void SsdInterface::recordSuccessPatternToOutputFile(const std::string & value) {
  output = value;

  std::ofstream file(OUTPUT_FILE_NAME, std::ios::trunc);
  if (file.is_open()) {
    file << output << "\n";
  }
}

bool SsdInterface::isValidNumberZeroToNintyNine(const std::string& str) {
  // ��� ���ڰ� �������� Ȯ��
  for (char ch : str) {
    if (!std::isdigit(ch)) return false;
  }
  int num = std::stoi(str);
  // ���� ������ 0~99���� Ȯ��
  try {
    int num = std::stoi(str);
    return num >= 0 && num <= 99;
  } catch (...) {
    return false;
  }
}