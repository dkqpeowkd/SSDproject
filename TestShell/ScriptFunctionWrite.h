#pragma once
#include <random>
#include <iomanip>
#include "ScriptFunction.h"

class ScriptFunctionWrite : public ScriptFunction {
private:
    const string command = "WriteFunc";
    shared_ptr<ICommand> writeCommand;
    shared_ptr<ICommand> readCommand;
    const string __EMPTY = "";

    bool isAutoIncrease = false;
    int LBAoffset = 0;
    vector<pair<string, string>> writeHistory = {};
public:
    shared_ptr<ScriptFunction> clone() const override { return nullptr; }
    ScriptFunctionWrite(shared_ptr<ICommand> writeCommand, shared_ptr<ICommand> readCommand) : writeCommand{ writeCommand }, readCommand{ readCommand } {}
    const string& getCommandString() override { return command; }
    bool isMatch(const string& command) override { return this->command == command; }
    const string& getUsage() override { return __EMPTY; }
    bool isValidArguments(const string& cmd, vector<string>& args) override {
        if (isMatch(cmd) == false)return false;
        if (args.size() == 0 || args.size() > 3) return false;
        return true;
    }
    bool Execute(const string& cmd, vector<string>& args) override {
        if (isValidArguments(cmd, args) == false) return false;

        string& opt = args[0];
        switch (args.size()) {
        case 1:
            if (opt == "EnableAutoIncrease") {
                isAutoIncrease = true;
                return true;
            }
            if (opt == "DisableAutoIncrease") {
                isAutoIncrease = false;
                return true;
            }
            if (opt == "IncreaseLBA") {
                LBAoffset++;
                return true;
            }
            if (opt == "DecreaseLBA") {
                LBAoffset--;
                return true;
            }
            if (opt == "ReadCompare") {
                try { return ReadCompare(cmd, args); }
                catch (std::exception& e) { return false; }
            }
            break;
        case 2:

            if (opt == "SetLBA") {
                try {
                    int offset = std::stoi(args[1]);
                    LBAoffset = offset;
                    return true;
                }
                catch (std::exception& e) {
                    return false;
                }
            }
            if (opt == "Random") {
                try {
                    int targetLBA = std::stoi(args[1]);
                    return WriteRandom(targetLBA);
                }
                catch (std::exception& e) {
                    return false;
                }
            }
            break;
        default:
            return false;
        }

        return false;
    }

    bool ReadCompare(const string& cmd, vector<string>& args) {
        bool result = true;
        if (writeHistory.size() == 0)
            return false;
        if (args.size() == 0) {
            for (const auto& writeEntry : writeHistory) {
                vector<string> args{ writeEntry.first, writeEntry.second };
                if (false == readCommand->Execute(readCommand->getCommandString(), args)) {
                    result = false;
                    break;
                }
            }
            writeHistory.clear();
            return result;
        }
        
        if (args.size() == 2) {
            if (false == readCommand->Execute(readCommand->getCommandString(), args))
                return false;
            return true;
        }

        return false;
    }

    bool WriteRandom(int numLBA) {
        int lbaNum;
        for (lbaNum = LBAoffset; lbaNum < LBAoffset + numLBA; ++lbaNum) {
            string lbaStr = std::to_string(lbaNum);
            string randomValue = getRandomHexValue();
            //std::cout << "write with random value : " << lbaStr << ", " << randomValue << std::endl;
            vector<string> args{ lbaStr, randomValue };
            if (writeCommand->Execute(writeCommand->getCommandString(), args) == false)
                return false;
            else
                writeHistory.emplace_back(pair{ lbaStr, randomValue });
        }
        LBAoffset = lbaNum;
        return true;
    }

    string getRandomHexValue() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<uint32_t> dis(0, 0xFFFFFFFF);

        uint32_t random_hex = dis(gen);

        std::stringstream ss;
        ss << "0x" << std::uppercase << std::hex << std::setw(8) << std::setfill('0') << random_hex;
        return ss.str();
    }
};