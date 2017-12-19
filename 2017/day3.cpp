#include "util.h"

using namespace std;

int puzzle_a(const string &input);
int puzzle_b(const string &input);
enum class direction : unsigned char
{
	none = 0,
	left = 1,
	right = 2,
	up = 3,
	down = 4
};
struct cell
{
	cell *l, *r, *t, *b, *lt, *rt, *lb, *rb;
	int value;
	direction dir;
	cell()
	{
		l = r = t = b = lt = rt = lb = rb = nullptr;
		value = 0;
		dir = direction::none;
	}
	cell *make_next()
	{
		cell *next = new cell();
		switch (dir)
		{
		case direction::left:
			if (b == nullptr)
			{
				next->dir = direction::down;
				next->t = this;
				next->rt = r;
				next->r = rb;
				next->rb = rb != nullptr ? rb->b : nullptr;
				b = next;
			}
			else
			{
				next->dir = direction::left;
				next->r = this;
				next->rb = b;
				next->b = lb;
				next->lb = lb != nullptr ? lb->l : nullptr;
				l = next;
			}
			break;
		case direction::right:
			if (t == nullptr)
			{
				next->dir = direction::up;
				next->b = this;
				next->lb = l;
				next->l = lt;
				next->lt = lt != nullptr ? lt->t : nullptr;
				t = next;
			}
			else
			{
				next->dir = direction::right;
				next->l = this;
				next->lt = t;
				next->t = rt;
				next->rt = rt != nullptr ? rt->r : nullptr;
				r = next;
			}
			break;
		case direction::up:
			if (l == nullptr)
			{
				next->dir = direction::left;
				next->r = this;
				next->rb = b;
				next->b = lb;
				next->lb = lb != nullptr ? lb->l : nullptr;
				l = next;
			}
			else
			{
				next->dir = direction::up;
				next->b = this;
				next->lb = l;
				next->l = lt;
				next->lt = lt != nullptr ? lt->t : nullptr;
				t = next;
			}
			break;
		case direction::down:
			if (r == nullptr)
			{
				next->dir = direction::right;
				next->l = this;
				next->lt = t;
				next->t = rt;
				next->rt = rt != nullptr ? rt->r : nullptr;
				r = next;
			}
			else
			{
				next->dir = direction::down;
				next->t = this;
				next->rt = r;
				next->r = rb;
				next->rb = rb != nullptr ? rb->b : nullptr;
				b = next;
			}
			break;
		default:
			throw std::logic_error("cell is missing direction");
			return nullptr;
		}

		next->calc_value();
		return next;
	}
	void calc_value()
	{
		value =
		  ( l != nullptr ?  l->value : 0)
		+ ( r != nullptr ?  r->value : 0)
		+ ( t != nullptr ?  t->value : 0)
		+ ( b != nullptr ?  b->value : 0)
		+ (lt != nullptr ? lt->value : 0)
		+ (rt != nullptr ? rt->value : 0)
		+ (lb != nullptr ? lb->value : 0)
		+ (rb != nullptr ? rb->value : 0);
	}
};

int main(int argc, const char *argv[])
{
	config c = proc(argc, argv, 3);

	cout << "Data: " << c.input << endl;
	cout << "Result:" << (c.puzzle == 1 ? puzzle_a(c.input) : puzzle_b(c.input)) << endl;
	return 0;
}

int puzzle_a(const string &input)
{
	// The pattern of the sprial follows the equation `cell_count = 1 + !(n * 8)`
	// This means every ring's size is `n * 8` and the total number of cells are a simple factorial
	int sum = 1;
	int locale = atoi(input.c_str());
	int curr = 0;
	int count = 0;
	while (sum < locale)
	{
		sum += (curr += 8);
		count++;
	}

	int side = curr / 4;
	locale = (locale - (sum - curr)) % side;
	int halfSide = side >> 1;
	cout << (strx::format("sum:{0} curr:{1} count:{2} side:{3} halfSide:{4}") % sum % curr % count % side % halfSide).str()
		 << endl;

	if (locale < halfSide)
		return count + (halfSide - locale);
	return count + (locale - halfSide);
}

int puzzle_b(const string &input)
{
	vector<cell*> cells;
	int locale = atoi(input.c_str());
	cell *centre = new cell();
	centre->value = 1;
	centre->dir = direction::down;
	cell *curr = centre;
	cells.push_back(curr);
	while(curr->value < locale) {
		curr = curr->make_next();
		cells.push_back(curr);
	}
	int result = curr->value;
	for(auto p_cell : cells) {
		delete p_cell;
	}
	cells.clear();
	return result;
}

/*

http://adventofcode.com/2017/day/3

--- Day 3: Spiral Memory ---
You come across an experimental new kind of memory stored on an infinite two-dimensional grid.
Each square on the grid is allocated in a spiral pattern starting at a location marked 1 and then counting up while spiraling outward. For example, the first few squares are allocated like this:
17  16  15  14  13
18   5   4   3  12
19   6   1   2  11
20   7   8   9  10
21  22  23---> ...
While this is very space-efficient (no squares are skipped), requested data must be carried back to square 1 (the location of the only access port for this memory system) by programs that can only move up, down, left, or right. They always take the shortest path: the Manhattan Distance between the location of the data and square 1.
For example:
Data from square 1 is carried 0 steps, since it's at the access port.
Data from square 12 is carried 3 steps, such as: down, left, left.
Data from square 23 is carried only 2 steps: up twice.
Data from square 1024 must be carried 31 steps.
How many steps are required to carry the data from the square identified in your puzzle input all the way to the access port?

--- Part Two ---
As a stress test on the system, the programs here clear the grid and then store the value 1 in square 1. Then, in the same allocation order as shown above, they store the sum of the values in all adjacent squares, including diagonals.
So, the first few squares' values are chosen as follows:
Square 1 starts with the value 1.
Square 2 has only one adjacent filled square (with value 1), so it also stores 1.
Square 3 has both of the above squares as neighbors and stores the sum of their values, 2.
Square 4 has all three of the aforementioned squares as neighbors and stores the sum of their values, 4.
Square 5 only has the first and fourth squares as neighbors, so it gets the value 5.
Once a square is written, its value does not change. Therefore, the first few squares would receive the following values:
147  142  133  122   59
304    5    4    2   57
330   10    1    1   54
351   11   23   25   26
362  747  806--->   ...
What is the first value written that is larger than your puzzle input?

*/