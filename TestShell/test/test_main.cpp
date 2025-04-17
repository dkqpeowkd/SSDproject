#include "gmock/gmock.h"
#include<iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;
// 명령 실행 함수들 (임시 stub)
int WriteLBA(const std::vector<std::string>& args) {
  int value = std::stoi(args[1], nullptr, 10);
  return value;
  
}

int ReadLBA(const std::vector<std::string>& args) {
  int value = std::stoi(args[1], nullptr, 10);
  return value;
}


int WritePattern(const std::vector<std::string>& args) {

  int value = std::stoul(args[2], nullptr, 16);
  return value;
}
string ReadPattern(const std::vector<std::string>& args) {

	if (args[1] == "0") return "0x11111111";
	if (args[1] == "50") return  "0x99999999";
    if (args[1] == "99") return "0xFFFFFFFF";

	return "0x00000000";
}

void WriteFunction(void) {
  
}
void ReadFunction(void) {}


int FullWrite() {
  int i;
  for (i = 0; i < 99; i++) WriteFunction();
 
  return i;
}


int FullRead() {
  int i;
  for (i = 0; i < 99; i++) ReadFunction();

  return i;
}


TEST(WriteTest, WriteLBATEST1) {

	vector<string> split_command;
  
	split_command.push_back("Write");
	split_command.push_back("0");
    split_command.push_back("ABABABAB");

	EXPECT_EQ(0, WriteLBA(split_command));
}


TEST(WriteTest, WriteLBATEST2) {
  vector<string> split_command;

  split_command.push_back("Write");
  split_command.push_back("50");
  split_command.push_back("ABABABAB");

  EXPECT_EQ(50, WriteLBA(split_command));
}


TEST(WriteTest, WriteLBATEST3) {
  vector<string> split_command;

  split_command.push_back("Write");
  split_command.push_back("99");
  split_command.push_back("ABABABAB");

  EXPECT_EQ(99, WriteLBA(split_command));
}

TEST(WriteTest, WritePatternTEST1) {
  vector<string> split_command;

  split_command.push_back("Write");
  split_command.push_back("50");
  split_command.push_back("0x12345678");

  EXPECT_EQ(0x12345678, WritePattern(split_command));
}

TEST(WriteTest, WritePatternTEST2) {
  vector<string> split_command;

  split_command.push_back("Write");
  split_command.push_back("50");
  split_command.push_back("0x9ABCDEF0");

  EXPECT_EQ(0x9ABCDEF0, WritePattern(split_command));
}

TEST(WriteTest, WritePatternTEST3) {
  vector<string> split_command;

  split_command.push_back("Write");
  split_command.push_back("50");
  split_command.push_back("0x00000000");

  EXPECT_EQ(0x00000000, WritePattern(split_command));
}

TEST(WriteTest, WritePatternTEST4) {
  vector<string> split_command;

  split_command.push_back("Write");
  split_command.push_back("50");
  split_command.push_back("0x99999999");

  EXPECT_EQ(0x99999999, WritePattern(split_command));
}

TEST(WriteTest, WritePatternTEST5) {
  vector<string> split_command;

  split_command.push_back("Write");
  split_command.push_back("50");
  split_command.push_back("0xAAAAAAAA");

  EXPECT_EQ(0xAAAAAAAA, WritePattern(split_command));
}

TEST(WriteTest, WritePatternTEST6) {
  vector<string> split_command;

  split_command.push_back("Write");
  split_command.push_back("50");
  split_command.push_back("0xFFFFFFFF");

  EXPECT_EQ(0xFFFFFFFF, WritePattern(split_command));
}



TEST(ReadTest, ReadLBATEST1) {
  vector<string> split_command;

  split_command.push_back("Write");
  split_command.push_back("0");
  split_command.push_back("ABABABAB");

  EXPECT_EQ(0, ReadLBA(split_command));
}

TEST(ReadTest, ReadLBATEST2) {
  vector<string> split_command;

  split_command.push_back("Write");
  split_command.push_back("50");
  split_command.push_back("ABABABAB");

  EXPECT_EQ(50, ReadLBA(split_command));
}

TEST(ReadTest, ReadLBATEST3) {
  vector<string> split_command;

  split_command.push_back("Write");
  split_command.push_back("99");
  split_command.push_back("ABABABAB");

  EXPECT_EQ(99, ReadLBA(split_command));
}

TEST(ReadTest, ReadPatternTEST1) {
  vector<string> split_command;

  split_command.push_back("Read");
  split_command.push_back("0");

  EXPECT_EQ("0x11111111", ReadPattern(split_command));
}


TEST(ReadTest, ReadPatternTEST2) {
  vector<string> split_command;

  split_command.push_back("Read");
  split_command.push_back("50");

  EXPECT_EQ("0x99999999", ReadPattern(split_command));
}


TEST(ReadTest, ReadPatternTEST3) {
  vector<string> split_command;

  split_command.push_back("Read");
  split_command.push_back("99");

  EXPECT_EQ("0xFFFFFFFF", ReadPattern(split_command));
}


TEST(ReadTest, ReadPatternTEST4) {
  vector<string> split_command;

  split_command.push_back("Read");
  split_command.push_back("5");

  EXPECT_EQ("0x00000000", ReadPattern(split_command));
}


TEST(ReadTest, ReadPatternTEST5) {
  vector<string> split_command;

  split_command.push_back("Read");
  split_command.push_back("60");

  EXPECT_EQ("0x00000000", ReadPattern(split_command));
}


TEST(ReadTest, ReadPatternTEST6) {
  vector<string> split_command;

  split_command.push_back("Read");
  split_command.push_back("95");

  EXPECT_EQ("0x00000000", ReadPattern(split_command));
}

TEST(FullWriteTest, FullWriteTest) {

  EXPECT_EQ(99, FullWrite());
}

TEST(FullReadTest, FullReadTest) 
{ 
	EXPECT_EQ(99, FullRead());
}

int main() { 
	testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
