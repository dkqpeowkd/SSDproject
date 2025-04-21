#pragma once

#include "ScriptCommand.h"

class ScriptFunction : public ScriptCommand {
public:
    virtual ~ScriptFunction() = default;
    virtual shared_ptr<ScriptFunction> clone() const = 0;
    const string& getStartKeyword() { return getCommandString(); }
    const string getFinishKeyword() { return "End" + getStartKeyword(); }
};