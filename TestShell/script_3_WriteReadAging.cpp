#include "script_3_WriteReadAging.h"

const string& script_3_WriteReadAging::getCommandString()
{
    // TODO: ���⿡ return ���� �����մϴ�.
    return command;
}

const string& script_3_WriteReadAging::getUsage()
{
    // TODO: ���⿡ return ���� �����մϴ�.
    return usage;
}

bool script_3_WriteReadAging::isValidArguments(const string& cmd, vector<string>& args)
{
    if (args.size() == 0)
        return true;
    return false;
}

bool script_3_WriteReadAging::Execute(const string& cmd, vector<string>& args)
{
    //200ȸ �ݺ�, LBA 0 / 99�� ���� �� Write �� ReadCompare
    int testCount = 200;

    while (testCount--) {
        //ReadCompare
    }
    return true;
}
