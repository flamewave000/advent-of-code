#include "util.h"

using namespace std;
using namespace strx;

int puzzle_a(const string &input);
int puzzle_b(const string &input);

int main(int argc, const char *argv[])
{
	config c = proc(argc, argv, 8);

	cout << "\nResult:" << (c.puzzle == 1 ? puzzle_a(c.input) : puzzle_b(c.input)) << endl;
	return 0;
}

inline int &get_value(unordered_map<string, int> &registers, const string &key)
{
	return registers.find(key) != registers.end() ? registers[key] : (registers[key] = 0);
}

enum class comparison
{
	eq,
	ne,
	lt,
	gt,
	le,
	ge
};
struct condition
{
	string lvalue;
	comparison operation;
	int rvalue;
	bool compare(int lvalue) const
	{
		switch (operation)
		{
		case comparison::eq:
			return lvalue == rvalue;
		case comparison::ne:
			return lvalue != rvalue;
		case comparison::lt:
			return lvalue < rvalue;
		case comparison::gt:
			return lvalue > rvalue;
		case comparison::le:
			return lvalue <= rvalue;
		case comparison::ge:
			return lvalue >= rvalue;
		default:
			return false;
		}
	}
};
struct instruction
{
	string reg_name;
	bool increment;
	int amount;
	condition cond;
};

comparison str2comp(const string &str)
{
	if (str == "==")
		return comparison::eq;
	if (str == "!=")
		return comparison::ne;
	if (str == "<")
		return comparison::lt;
	if (str == ">")
		return comparison::gt;
	if (str == "<=")
		return comparison::le;
	if (str == ">=")
		return comparison::ge;
}

inline instruction process_line(const string &line)
{
	vector<string> tokens = split(line, ' ');
	return {
		tokens[0],
		tokens[1] == "inc",
		atoi(tokens[2].c_str()),
		// skip tokens[3], it is simply the word `if`
		{tokens[4],
		 str2comp(tokens[5]),
		 atoi(tokens[6].c_str())}};
}
void process(const string &input, function<void(const instruction &)> &&execute)
{
	vector<string> lines = split(input, '\n');
	printf("instruction count: %lu\n", lines.size());
	instruction inst;
	for (auto line : lines)
	{
		execute(process_line(line));
	}
}

inline unordered_map<string, int>::iterator get_register(unordered_map<string, int> &registers, const string &name)
{
	auto it = registers.find(name);
	return it != registers.end() ? it : registers.insert(make_pair(name, 0)).first;
}

void print_registers(const unordered_map<string, int> &registers) {
	for (auto reg : registers)
	{
		cout << "register: {0} \tvalue: {1}"_f % reg.first % reg.second << endl;
	}
}

int puzzle_a(const string &input)
{
	unordered_map<string, int> registers;
	process(input, [&registers](const instruction &inst) {
		auto lvalue = get_register(registers, inst.cond.lvalue);
		if (inst.cond.compare(lvalue->second))
		{
			auto reg = get_register(registers, inst.reg_name);
			if (inst.increment)
				reg->second += inst.amount;
			else
				reg->second -= inst.amount;
		}
	});

	int highest = INT32_MIN;
	for (auto reg : registers)
	{
		if (reg.second > highest)
			highest = reg.second;
	}
	print_registers(registers);
	return highest;
}

int puzzle_b(const string &input)
{
	unordered_map<string, int> registers;
	int highest = INT32_MIN;
	process(input, [&registers, &highest](const instruction &inst) {
		auto lvalue = get_register(registers, inst.cond.lvalue);
		if (inst.cond.compare(lvalue->second))
		{
			auto reg = get_register(registers, inst.reg_name);
			if (inst.increment)
				reg->second += inst.amount;
			else
				reg->second -= inst.amount;
			if(reg->second > highest) {
				highest = reg->second;
			}
		}
	});
	print_registers(registers);
	return highest;
}

/*
http://adventofcode.com/2017/day/8

--- Day 8: I Heard You Like Registers ---
You receive a signal directly from the CPU. Because of your recent assistance with jump instructions, it would like you to compute the result of a series of unusual register instructions.
Each instruction consists of several parts: the register to modify, whether to increase or decrease that register's value, the amount by which to increase or decrease it, and a condition. If the condition fails, skip the instruction without modifying the register. The registers all start at 0. The instructions look like this:
b inc 5 if a > 1
a inc 1 if b < 5
c dec -10 if a >= 1
c inc -20 if c == 10
These instructions would be processed as follows:
Because a starts at 0, it is not greater than 1, and so b is not modified.
a is increased by 1 (to 1) because b is less than 5 (it is 0).
c is decreased by -10 (to 10) because a is now greater than or equal to 1 (it is 1).
c is increased by -20 (to -10) because c is equal to 10.
After this process, the largest value in any register is 1.
You might also encounter <= (less than or equal to) or != (not equal to). However, the CPU doesn't have the bandwidth to tell you what all the registers are named, and leaves that to you to determine.
What is the largest value in any register after completing the instructions in your puzzle input?

--- Part Two ---
To be safe, the CPU also needs to know the highest value held in any register during this process so that it can decide how much memory to allocate to these operations. For example, in the above instructions, the highest value ever held was 10 (in register c after the third instruction was evaluated).

*/