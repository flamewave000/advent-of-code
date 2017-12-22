#include "util.h"

using namespace std;

int puzzle_a(const string& input);
int puzzle_b(const string& input);

int main(int argc, const char *argv[])
{
	config c = proc(argc, argv, __FILE__);

	cout << "\n\nResult:" << (c.puzzle == 1 ? puzzle_a(c.input) : puzzle_b(c.input)) << endl;
	return 0;
}

int puzzle_a(const string& input) {
	istringstream iss(input);
	string buff;
	unsigned int sum = 0, hi = 0, lo = ~0, val = 0;
	while(getline(iss, buff)) {
		istringstream tokens(buff);
		while(!tokens.eof()) {
			tokens >> val;
			if(val < lo) lo = val;
			if(val > hi) hi = val;
		}
		printf("\nRow[hi:%4d, lo:%4d, %4d] %s", hi, lo, hi-lo, buff.c_str());
		sum += hi - lo;
		hi = val = 0;
		lo = ~0;
	}
	return sum;
}

int puzzle_b(const string& input) {
	istringstream iss(input);
	string buff;
	unsigned int sum = 0, val = 0;
	int c = 0;
	vector<int> values;
	while(getline(iss, buff)) {
		istringstream tokens(buff);
		while(!tokens.eof()) {
			tokens >> val;
			c = values.size() - 1;
			for(; c >= 0; c--) {
				if(values[c] % val == 0 || val % values[c] == 0)
					break;
			}
			// if we found a pair
			if(c >= 0) {
				int div = val > values[c] ? val / values[c] : values[c] / val;
				printf("\nRow[a:%4d, b:%4d, v:%4d] %s", val, values[c], div, buff.c_str());
				sum += div;
				break;
			}
			values.push_back(val);
		}
		values.clear();
	}
	return sum;
}

/*
http://adventofcode.com/2017/day/2

--- Day 2: Corruption Checksum ---
As you walk through the door, a glowing humanoid shape yells in your direction. "You there! Your state appears to be idle. Come help us repair the corruption in this spreadsheet - if we take another millisecond, we'll have to display an hourglass cursor!"
The spreadsheet consists of rows of apparently-random numbers. To make sure the recovery process is on the right track, they need you to calculate the spreadsheet's checksum. For each row, determine the difference between the largest value and the smallest value; the checksum is the sum of all of these differences.
For example, given the following spreadsheet:
5 1 9 5
7 5 3
2 4 6 8
The first row's largest and smallest values are 9 and 1, and their difference is 8.
The second row's largest and smallest values are 7 and 3, and their difference is 4.
The third row's difference is 6.
In this example, the spreadsheet's checksum would be 8 + 4 + 6 = 18.

--- Part Two ---
"Great work; looks like we're on the right track after all. Here's a star for your effort." However, the program seems a little worried. Can programs be worried?
"Based on what we're seeing, it looks like all the User wanted is some information about the evenly divisible values in the spreadsheet. Unfortunately, none of us are equipped for that kind of calculation - most of us specialize in bitwise operations."
It sounds like the goal is to find the only two numbers in each row where one evenly divides the other - that is, where the result of the division operation is a whole number. They would like you to find those numbers on each line, divide them, and add up each line's result.
For example, given the following spreadsheet:
5 9 2 8
9 4 7 3
3 8 6 5
In the first row, the only two numbers that evenly divide are 8 and 2; the result of this division is 4.
In the second row, the two numbers are 9 and 3; the result is 3.
In the third row, the result is 2.
In this example, the sum of the results would be 4 + 3 + 2 = 9.

*/