#pragma once

class SsdComponentFactory {
 public:
  virtual ~SsdComponentFactory() = default;
  virtual std::unique_ptr<NandStorage> createNandStorage() = 0;
  virtual std::unique_ptr<Recoder> createRecoder() = 0;
  virtual std::unique_ptr<Validator> createValidator() = 0;
};