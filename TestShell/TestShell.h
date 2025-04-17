
#pragma once
#include <string>
#include <vector>

class TestShell {
public:
    virtual ~TestShell() = default;
    void run();
    std::string executeCommand(const std::string& input);

protected:
    virtual int callSystem(const std::string& cmd);
    virtual std::string readOutput();
};
