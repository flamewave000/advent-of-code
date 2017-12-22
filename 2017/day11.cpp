#define LINQ_USE_MACROS
#include "util.h"

using namespace std;
using namespace strx;
using namespace linq;

inline int puzzle_a(const string &input);
int puzzle_b(const string &input);

int main(int argc, const char *argv[])
{
	config c = proc(argc, argv, __FILE__);

	cout << "\nResult:" << (c.puzzle == 1 ? puzzle_a(c.input) : puzzle_b(c.input)) << endl;
	return 0;
}

inline int distance_to_centre(int x, int y, int z) {
	return (abs(x) + abs(y) + abs(z)) >> 1;
}

int walk(const string &input, function<void(int,int,int)> *step_handler = nullptr) {
	auto steps = split(input, ',');
	int x = 0, y = 0, z = 0;
	cout << "Step Count: " << steps.size() << endl;
	for (auto step : steps)
	{
		if (step[1] == 'e')
		{
			x++;
			if (step[0] == 'n') z--;
			else y--;
		}
		else if (step[1] == 'w')
		{
			x--;
			if (step[0] == 'n') y++;
			else z++;
		}
		else if (step[0] == 'n')
		{
			y++;
			z--;
		}
		else
		{
			z++;
			y--;
		}
		if (step_handler != nullptr)
			step_handler->operator()(x, y, z);
	}
	return distance_to_centre(x, y, z);
}

inline int puzzle_a(const string &input)
{
	return walk(input);
}

int puzzle_b(const string &input)
{
	int max_distance = 0;
	function<void(int, int, int)> step([&max_distance](int x, int y, int z) -> void {
		int distance = distance_to_centre(x, y, z);
		if (max_distance < distance)
			max_distance = distance;
	});
	walk(input, &step);
	return max_distance;
}

/*
http://adventofcode.com/2017/day/11

--- Day 11: Hex Ed ---
Crossing the bridge, you've barely reached the other side of the stream when a program comes up to you, clearly in distress. "It's my child process," she says, "he's gotten lost in an infinite grid!"
Fortunately for her, you have plenty of experience with infinite grids.
Unfortunately for you, it's a hex grid.
The hexagons ("hexes") in this grid are aligned such that adjacent hexes can be found to the north, northeast, southeast, south, southwest, and northwest:
    \ n  /
  nw +--+ ne
    /    \
  -+      +-
    \    /
  sw +--+ se
    / s  \
You have the path the child process took. Starting where he started, you need to determine the fewest number of steps required to reach him. (A "step" means to move from the hex you are in to any adjacent hex.)
For example:
- ne,ne,ne is 3 steps away.
- ne,ne,sw,sw is 0 steps away (back where you started).
- ne,ne,s,s is 2 steps away (se,se).
- se,sw,se,sw,sw is 3 steps away (s,s,sw).

--- Part Two ---
How many steps away is the furthest he ever got from his starting position?

*/