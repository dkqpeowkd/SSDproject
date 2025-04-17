#include "script_3_WriteReadAging.h"

const string& script_3_WriteReadAging::getCommandString()
{
    // TODO: 여기에 return 문을 삽입합니다.
    return command;
}

const string& script_3_WriteReadAging::getUsage()
{
    // TODO: 여기에 return 문을 삽입합니다.
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
    //200회 반복, LBA 0 / 99에 랜덤 값 Write 후 ReadCompare
    int testCount = 200;

    while (testCount--) {
        //ReadCompare
    }
    return true;
}
