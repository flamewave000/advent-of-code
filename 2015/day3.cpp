#include "util.h"

using namespace std;
using namespace strx;
using namespace linq;

enum direction {
	NORTH = '^',
	EAST = '>',
	SOUTH = 'v',
	WEST = '<'
};

int puzzle_a(const string &input) {
	unordered_map<int, unordered_map<int, int>> house_map;
	house_map[0][0] = 1;

	int house_count = 1;
	int x = 0, y = 0;
	for (size_t c = 0, size = input.size(); c < size; c++) {
		switch (input[c]) {
		case NORTH:
			y++;
			break;
		case EAST:
			x++;
			break;
		case SOUTH:
			y--;
			break;
		case WEST:
			x--;
			break;
		default:
			cerr << "INVALID COMMAND" << endl;
			exit(EXIT_FAILURE);
		}
		auto col = house_map.find(x);
		if (col == house_map.end() || col->second.find(y) == col->second.end()) {
			house_map[x][y] = 1;
			house_count++;
		} else
			house_map[x][y]++;
	}
	return house_count;
}

int puzzle_b(const string &input) {
	return 0;
}

int main(int argc, const char *argv[]) {
	config c = proc(argc, argv, __FILE__);

	auto result = c.puzzle == 1 ? puzzle_a(c.input) : puzzle_b(c.input);
	cout << "\nResult: " << result << endl;
	return EXIT_SUCCESS;
}

/*
http://adventofcode.com/2015/day/3

--- Day 3: Perfectly Spherical Houses in a Vacuum ---

Santa is delivering presents to an infinite two-dimensional grid of houses.

He begins by delivering a present to the house at his starting location, and then an elf at the North Pole calls him via radio and tells him where to move next. Moves are always exactly one house to the north (^), south (v), east (>), or west (<). After each move, he delivers another present to the house at his new location.

However, the elf back at the north pole has had a little too much eggnog, and so his directions are a little off, and Santa ends up visiting some houses more than once. How many houses receive at least one present?

For example:

 - `>`          delivers presents to 2 houses: one at the starting location, and one to the east.
 - `^>v<`       delivers presents to 4 houses in a square, including twice to the house at his starting/ending location.
 - `^v^v^v^v^v` delivers a bunch of presents to some very lucky children at only 2 houses.

Your puzzle answer was 2565.

*/