#include "util.h"

using namespace std;
using namespace strx;
using namespace linq;

const string ab = "ab", cd = "cd", pq = "pq", xy = "xy";

int puzzle_a(const string &input) {
	istringstream ss(input);
	string line;

	int count = 0;
	while (!ss.eof()) {
		ss >> line;
		bool dbl_letter = false;
		bool valid = true;
		int vowel_count = 0;
		char buff[3] = {0, 0, 0};
		for (int c = 0, size = line.size(); c < size; c++) {
			if (c < size - 1) {
				if (!dbl_letter && line[c] == line[c + 1]) {
					dbl_letter = true;
				}
				buff[0] = line[c];
				buff[1] = line[c + 1];
				if (buff == ab || buff == cd || buff == pq || buff == xy) {
					valid = false;
					break;
				}
			}
			if (contains("aeiou", line[c])) {
				vowel_count++;
			}
		}
		if(valid && dbl_letter && vowel_count >= 3) {
			count ++;
		}
	}
	return count;
}

int puzzle_b(const string &input) {
	return 0;
}

int main(int argc, const char *argv[]) {
	config c = proc(argc, argv, __FILE__);

	// cout << "Data: " << c.input << endl;
	auto result = c.puzzle == 1 ? puzzle_a(c.input) : puzzle_b(c.input);
	cout << "\nResult: " << result << endl;
	return 0;
}

/*
http://adventofcode.com/201/day/

--- Day 5: Doesn't He Have Intern-Elves For This? ---

Santa needs help figuring out which strings in his text file are naughty or nice.

A nice string is one with all of the following properties:

 - It contains at least three vowels (aeiou only), like aei, xazegov, or aeiouaeiouaeiou.
 - It contains at least one letter that appears twice in a row, like xx, abcdde (dd), or aabbccdd (aa, bb, cc, or dd).
 - It does not contain the strings ab, cd, pq, or xy, even if they are part of one of the other requirements.

For example:

 - ugknbfddgicrmopn is nice because it has at least three vowels (u...i...o...), a double letter (...dd...), and none of the disallowed substrings.
 - aaa is nice because it has at least three vowels and a double letter, even though the letters used by different rules overlap.
 - jchzalrnumimnmhp is naughty because it has no double letter.
 - haegwjzuvuyypxyu is naughty because it contains the string xy.
 - dvszwmarrgswjxmb is naughty because it contains only one vowel.

How many strings are nice?

Your puzzle answer was 258.

*/