#include "util.h"

using namespace std;
using namespace strx;
using namespace linq;

int puzzle_a(const string &input);
int puzzle_b(const string &input);

int main(int argc, const char *argv[])
{
	config c = proc(argc, argv, __FILE__);

	cout << "Data: " << c.input << endl;
	cout << "\nResult:" << (c.puzzle == 1 ? puzzle_a(c.input) : puzzle_b(c.input)) << endl;
	return 0;
}

int puzzle_a(const string &input)
{
}

int puzzle_b(const string &input)
{
}

/*
http://adventofcode.com/2017/day/



*/