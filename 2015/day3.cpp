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

void process(const char &c, int &x, int &y, int &house_count, unordered_map<int, unordered_set<int>> &house_map) {
	if (c == NORTH) y++;
	else if (c == EAST) x++;
	else if (c == SOUTH) y--;
	else if (c == WEST) x--;
	else {
		cerr << "INVALID COMMAND: " << c << endl;
		exit(EXIT_FAILURE);
	}
	auto it = house_map.find(x);
	if (it == house_map.end() || it->second.find(y) == it->second.end()) {
		house_map[x].insert(y);
		house_count++;
	}
}

int main(int argc, const char *argv[]) {
	config cfg = proc(argc, argv, __FILE__);

	unordered_map<int, unordered_set<int>> house_map;
	house_map[0].insert(0);
	int house_count = 1;

	if (cfg.puzzle == 1) {
		int x = 0, y = 0;
		for (size_t c = 0, size = cfg.input.size(); c < size; c++) {
			process(cfg.input[c], x, y, house_count, house_map);
		}
	} else {
		int sx = 0, sy = 0, rx = 0, ry = 0;
		for (size_t c = 0, size = cfg.input.size(); c < size; c++) {
			process(cfg.input[c], c % 2 ? sx : rx, c % 2 ? sy : ry, house_count, house_map);
		}
	}
	cout << "\nResult: " << house_count << endl;
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

--- Part Two ---

The next year, to speed up the process, Santa creates a robot version of himself, Robo-Santa, to deliver presents with him.

Santa and Robo-Santa start at the same location (delivering two presents to the same starting house), then take turns moving based on instructions from the elf, who is eggnoggedly reading from the same script as the previous year.

This year, how many houses receive at least one present?

For example:

 - ^v delivers presents to 3 houses, because Santa goes north, and then Robo-Santa goes south.
 - ^>v< now delivers presents to 3 houses, and Santa and Robo-Santa end up back where they started.
 - ^v^v^v^v^v now delivers presents to 11 houses, with Santa going one direction and Robo-Santa going the other.

Your puzzle answer was 2639.

*/