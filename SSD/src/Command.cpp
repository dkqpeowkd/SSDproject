#include <utility>

#include "Command.h"

#include <algorithm>

#include <sstream>
#include "SsdController.h"


WriteCommand::WriteCommand(int lba_, std::string value_)
    : lba(lba_), value(std::move(value_)) {}

void WriteCommand::ApplyCommandBuffer(std::map<int, std::string>& lbaMap) const {
  lbaMap[lba] = value;
}

std::string WriteCommand::ToString() const {
  return "W " + std::to_string(lba) + " " + value;
}

void WriteCommand::ExecuteNandStorage(NandStorage& nandStorage, Recoder& recoder,
                           Validator& validator) const {
  if (!nandStorage.Write(std::to_string(lba), value)) {
    validator.SetErrorReason(" ### Write Fail (about File) ### ");
    recoder.RecordErrorPatternToOutputFile(validator.GetErrorReason());
  }
}

EraseCommand::EraseCommand(int lba_, int size_) : lba(lba_), size(size_) {}

void EraseCommand::ApplyCommandBuffer(std::map<int, std::string>& lbaMap) const {
  int start = std::min(lba, lba + size - 1);
  int end = std::max(lba, lba + size - 1);
  for (int i = start; i <= end; ++i) {
    lbaMap[i] = "0x00000000";
  }
}

std::string EraseCommand::ToString() const {
  return "E " + std::to_string(lba) + " " + std::to_string(size);
}

void EraseCommand::ExecuteNandStorage(NandStorage& nandStorage, Recoder& recoder,
                           Validator& validator) const {
  int start = std::min(lba, lba + size - 1);
  int end = std::max(lba, lba + size - 1);

  if (end > MAX_LBA) {
    validator.SetErrorReason("### Last LBA is over Max LBA ###");
    recoder.RecordErrorPatternToOutputFile(validator.GetErrorReason());
    return;
  }

  for (int i = start; i <= end; ++i) {
    if (!nandStorage.Write(std::to_string(i), "0x00000000")) {
      validator.SetErrorReason(" ### Write Fail (about File) ### ");
      recoder.RecordErrorPatternToOutputFile(validator.GetErrorReason());
    }
  }
}