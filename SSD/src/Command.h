#pragma once
#include <map>
#include <string>
class NandStorage;
class Validator;
class Recoder;

class ICommand {
 public:
  virtual ~ICommand() = default;
  virtual void ApplyCommandBuffer(std::map<int, std::string>& lbaMap) const = 0;
  virtual std::string ToString() const = 0;
  virtual void ExecuteNandStorage(NandStorage& nandStorage, Recoder& recoder,
                       Validator& validator) const = 0;
};


class WriteCommand : public ICommand {
  int lba;
  std::string value;

 public:
  WriteCommand(int lba_, std::string value_);
  void ApplyCommandBuffer(std::map<int, std::string>& lbaMap) const override;
  std::string ToString() const override;
  virtual void ExecuteNandStorage(NandStorage& nandStorage, Recoder& recoder,
                       Validator& validator) const override;
};


class EraseCommand : public ICommand {
  int lba;
  int size;

 public:
  EraseCommand(int lba_, int size_);
  void ApplyCommandBuffer(std::map<int, std::string>& lbaMap) const override;
  std::string ToString() const override;
  virtual void ExecuteNandStorage(NandStorage& nandStorage, Recoder& recoder,
                       Validator& validator) const override;
};