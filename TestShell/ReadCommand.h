
#pragma once
#include <string>
#include <vector>

class ReadCommand {
public:
    virtual ~ReadCommand() = default;
    void run();
    std::string executeCommand(const std::string& input);

protected:
    virtual int callSystem(const std::string& cmd);
    virtual std::string readOutput();
};
