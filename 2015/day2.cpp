#define LINQ_USE_MACROS
#include "util.h"

using namespace std;
using namespace strx;
using namespace linq;

struct box {
	uint8_t lo, mi, hi, _;
	box(uint8_t x, uint8_t y, uint8_t z) : lo(x), mi(y), hi(z) {
		sort(&lo, &_);
	}
};

vector<box> process(const string &input) {
	auto tokens = FROM(split(replace(replace(input, '\n', ' '), 'x', ' '), ' '))
		SELECT(uint8_t) { return (uint8_t)atoi(item.data()); }
		END;
	vector<box> boxes;
	cout << "tokens size: " << tokens.size() << endl;
	for (size_t c = 0, size = tokens.size(); c < size; c += 3) {
		boxes.push_back(box(tokens[c], tokens[c + 1], tokens[c + 2]));
	}
	return move(boxes);
}

constexpr int calc_area(const int &lo, const int &mi, const int &hi) {
	return (3 * lo * mi) + (2 * hi * (mi + lo));
}

int puzzle_a(const string &input) {
	auto boxes = process(input);

	size_t total_area = 0, area = 0;
	for (auto box : boxes) {
		total_area += calc_area(box.lo, box.mi, box.hi);
	}
	return total_area;
}

int puzzle_b(const string &input) {
	return 0;
}

int main(int argc, const char *argv[]) {
	config c = proc(argc, argv, __FILE__);

	auto result = c.puzzle == 1 ? puzzle_a(c.input) : puzzle_b(c.input);
	cout << "\nResult: " << result << endl;
	return 0;
}

/*
http://adventofcode.com/2015/day/2

--- Day 2: I Was Told There Would Be No Math ---

The elves are running low on wrapping paper, and so they need to submit an order for more. They have a list of the dimensions (length l, width w, and height h) of each present, and only want to order exactly as much as they need.

Fortunately, every present is a box (a perfect right rectangular prism), which makes calculating the required wrapping paper for each gift a little easier: find the surface area of the box, which is 2*l*w + 2*w*h + 2*h*l. The elves also need a little extra paper for each present: the area of the smallest side.

For example:

    A present with dimensions 2x3x4 requires 2*6 + 2*12 + 2*8 = 52 square feet of wrapping paper plus 6 square feet of slack, for a total of 58 square feet.
    A present with dimensions 1x1x10 requires 2*1 + 2*10 + 2*10 = 42 square feet of wrapping paper plus 1 square foot of slack, for a total of 43 square feet.

All numbers in the elves' list are in feet. How many total square feet of wrapping paper should they order?

Your puzzle answer was 1588178.

*/