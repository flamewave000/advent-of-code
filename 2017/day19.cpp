#include "util.h"
#define TEST false

using namespace std;
using namespace strx;
using namespace linq;

const char *test_data =
	"     |          \n"
	"     |  +--+    \n"
	"     A  |  C    \n"
	" F---|----E|--+ \n"
	"     |  |  |  D \n"
	"     +B-+  +--+ ";

#pragma region Data Structures
enum class path_type : char { vertical = '|',
							  horizontal = '-',
							  intersect = '+',
							  none = ' ' };
struct svec2 {
	short x, y;
};
struct llvec2 {
	int64_t x, y;
	inline llvec2 & operator+=(const svec2 &rhs) {
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	}
};
#pragma endregion

#pragma region Operator Overloads
inline bool operator==(const char &c, const path_type &p) { return c == (char)p; }
inline bool operator==(const path_type &p, const char &c) { return (char)p == c; }
inline bool operator!=(const char &c, const path_type &p) { return c != (char)p; }
inline bool operator!=(const path_type &p, const char &c) { return (char)p != c; }
#pragma endregion


int main(int argc, const char *argv[]) {
	config c = proc(argc, argv, __FILE__);

#if TEST
	cout << test_data << endl;
	vector<string> lines = split(test_data, '\n');
#else
	vector<string> lines = split(c.input, '\n');
#endif

	string letters;
	int64_t steps = 0;

	size_t xsize = lines[0].size(), ysize = lines.size();
	svec2 dir = {0, 1};
	llvec2 pos = {0, 0};

	for (int c = 0; c < xsize; c++) {
		if (lines[0][c] == path_type::vertical) {
			pos.x = c;
			pos.y = 1;
			break;
		}
	}

#define FETCH(xoffset, yoffset) lines[pos.y + yoffset][pos.x + xoffset]

	printf("START: %lld, %lld\n", pos.x, pos.y);

	while (true) {
		// if our position has gone outside the bounds of the array
		if (pos.x < 0 || pos.x >= xsize || pos.y < 0 || pos.y >= ysize) {
			printf("END: OUT OF RANGE\n");
			steps++;
			break;
		}
		const char current = FETCH(0, 0);
		// if we hit whitespace, we're done
		if (current == path_type::none) {
			printf("END: WHITESPACE\n");
			steps++;
			break;
		}
		// if we are at an intersection
		if (current == path_type::intersect) {
			// if we are moving horizontally
			if (dir.x) {
				// if there is a vertical path above us
				if (pos.y > 0 && FETCH(0, -1) != path_type::none) {
					dir = {0, -1};
					printf("UP\n");
				}
				// if there is a vertical path below us
				else if (pos.y < (ysize - 1) && FETCH(0, 1) != path_type::none) {
					dir = {0, 1};
					printf("DOWN\n");
				}
			}
			// if we are moving vertically
			else {
				// if there is a horizontal path left of us
				if (pos.x > 0 && FETCH(-1, 0) != path_type::none) {
					dir = {-1, 0};
					printf("LEFT\n");
				}
				// if there is a horizontal path right of us
				else if (pos.x < (xsize - 1) && FETCH(1, 0) != path_type::none) {
					dir = {1, 0};
					printf("RIGHT\n");
				}
			}
		}
		// if we are sitting on a letter
		if (current >= 'A' && current <= 'Z') {
			letters += current;
			printf("FOUND %c\n", current);
		}
		pos += dir;
		steps++;
	}
#undef FETCH
	printf("Final Position: %lld, %lld\n\nLetters: %s\nSteps: %lld\n", pos.x, pos.y, letters.c_str(), steps);
	return 0;
}

/*
http://adventofcode.com/2017/day/19

--- Day 19: A Series of Tubes ---

Somehow, a network packet got lost and ended up here. It's trying to follow a routing diagram (your puzzle input), but it's confused about where to go.

Its starting point is just off the top of the diagram. Lines (drawn with |, -, and +) show the path it needs to take, starting by going down onto the only line connected to the top of the diagram. It needs to follow this path until it reaches the end (located somewhere within the diagram) and stop there.

Sometimes, the lines cross over each other; in these cases, it needs to continue going the same direction, and only turn left or right when there's no other option. In addition, someone has left letters on the line; these also don't change its direction, but it can use them to keep track of where it's been. For example:
┌────────────────┐
│     |          │
│     |  +--+    │
│     A  |  C    │
│ F---|----E|--+ │
│     |  |  |  D │
│     +B-+  +--+ │
└────────────────┘

Given this diagram, the packet needs to take the following path:

    Starting at the only line touching the top of the diagram, it must go down, pass through A, and continue onward to the first +.
    Travel right, up, and right, passing through B in the process.
    Continue down (collecting C), right, and up (collecting D).
    Finally, go all the way left through E and stopping at F.

Following the path to the end, the letters it sees on its path are ABCDEF.

The little packet looks up at you, hoping you can help it find the way. What letters will it see (in the order it would see them) if it follows the path? (The routing diagram is very wide; make sure you view it without line wrapping.)

Your puzzle answer was HATBMQJYZ.

--- Part Two ---

The packet is curious how many steps it needs to go.

For example, using the same routing diagram from the example above...

┌────────────────┐
│     |          │
│     |  +--+    │
│     A  |  C    │
│ F---|--|-E---+ │
│     |  |  |  D │
│     +B-+  +--+ │
└────────────────┘

...the packet would go:

    6 steps down (including the first line at the top of the diagram).
    3 steps right.
    4 steps up.
    3 steps right.
    4 steps down.
    3 steps right.
    2 steps up.
    13 steps left (including the F it stops on).

This would result in a total of 38 steps.

How many steps does the packet need to go?

Your puzzle answer was 16332.

*/