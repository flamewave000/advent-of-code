#include "util.h"

using namespace std;
using namespace strx;

int puzzle_a(const string &input);
int puzzle_b(const string &input);

int main(int argc, const char *argv[])
{
	config c = proc(argc, argv, ##);

	cout << "Data: " << c.input;
	cout << "\n\nResult:" << (c.puzzle == 1 ? puzzle_a(c.input) : puzzle_b(c.input)) << endl;
	return 0;
}

int puzzle_a(const string &input)
{
}

int puzzle_b(const string &input)
{
}

/*



*/