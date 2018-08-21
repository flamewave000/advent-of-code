#include "util.h"

using namespace std;
using namespace strx;
using namespace linq;

void puzzle_a(const vector<string> &input);
void puzzle_b(const vector<string> &input);

char programs[16] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p'};

void initialize()
{
    for (int c = 0; c < sizeof(programs); c++)
    {
        programs[c] = (char)((int)'a' + (int)c);
    }
}

void spin(uint8_t count)
{
    char buff[sizeof(programs)];
    memcpy(buff, programs + (sizeof(programs) - count), count);
    memcpy(buff + count, programs, sizeof(programs) - count);
    memcpy(programs, buff, sizeof(programs));
}
inline void pswap(const size_t &a, const size_t &b)
{
    char store = programs[a];
    programs[a] = programs[b];
    programs[b] = store;
}

void partner(char a, char b)
{
    size_t ia, ib;
    for (size_t c = 0; c < sizeof(programs); c++)
    {
        if (programs[c] == a)
            ia = c;
        if (programs[c] == b)
            ib = c;
    }
    pswap(ia, ib);
}


struct token {char type; string a, b;};
token split(const string &str) {
    token t;
    t.type = str[0];
    t.a = "";
    t.b = "";
    bool first = true;
    for(size_t c = 1, size = str.size(); c < size; c++) {
        if(str[c] == '/') {
            first = false;
            continue;
        }
        if(first)
            t.a += str[c];
        else
            t.b += str[c];
    }
    return move(t);
}

int main(int argc, const char *argv[])
{
    config c = proc(argc, argv, __FILE__);
    vector<string> tokens = split(c.input, ',');

    if (c.puzzle == 1)
        puzzle_a(tokens);
    else
        puzzle_b(tokens);

    char result[sizeof(programs) + 1];
    memset(result, 0, sizeof(result));
    memcpy(result, programs, sizeof(programs));
    cout << "\nResult:" << result << endl;
    return 0;
}

void puzzle_a(const vector<string> &input)
{
    initialize();
    token t;
    for (size_t c = 0, size = input.size(); c < size; c++)
    {
        t = split(input[c]);
        switch (t.type)
        {
        case 's':
            spin(atoi(t.a.c_str()));
            break;
        case 'p':
            partner(t.a[0], t.b[0]);
            break;
        case 'x':
            pswap(atoi(t.a.c_str()), atoi(t.b.c_str()));
            break;
        }
    }
}

void puzzle_b(const vector<string> &input)
{
}

/*
http://adventofcode.com/2017/day/16

--- Day 16: Permutation Promenade ---

You come upon a very unusual sight; a group of programs here appear to be dancing.

There are sixteen programs in total, named a through p. They start by standing in a line: a stands in position 0, b stands in position 1, and so on until p, which stands in position 15.

The programs' dance consists of a sequence of dance moves:

    Spin, written sX, makes X programs move from the end to the front, but maintain their order otherwise. (For example, s3 on abcde produces cdeab).
    Exchange, written xA/B, makes the programs at positions A and B swap places.
    Partner, written pA/B, makes the programs named A and B swap places.

For example, with only five programs standing in a line (abcde), they could do the following dance:

    s1, a spin of size 1: eabcd.
    x3/4, swapping the last two programs: eabdc.
    pe/b, swapping programs e and b: baedc.

After finishing their dance, the programs end up in order baedc.

You watch the dance for a while and record their dance moves (your puzzle input). In what order are the programs standing after their dance?

*/