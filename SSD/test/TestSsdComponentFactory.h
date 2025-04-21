#pragma once
#include "../src/SsdComponentFactory.h"

class TestSsdComponentFactory : public SsdComponentFactory {
 public:
  std::unique_ptr<NandStorage> createNandStorage() override {
    return std::make_unique<NandStorage>();
  }
  std::unique_ptr<Recoder> createRecoder() override {
    return std::make_unique<Recoder>();
  }
  std::unique_ptr<Validator> createValidator() override {
    return std::make_unique<Validator>();
  }
};