#include "util.h"
#include <list>

#define TEST false


using namespace std;
using namespace strx;
using namespace linq;

#if TEST
char programs[] = {'a', 'b', 'c', 'd', 'e'};
#else
char programs[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p'};
#endif

struct token
{
    char type;
    string a, b;
};
token split(const string &str)
{
    token t;
    t.type = str[0];
    t.a = "";
    t.b = "";
    bool first = true;
    for (size_t c = 1, size = str.size(); c < size; c++)
    {
        if (str[c] == '/')
        {
            first = false;
            continue;
        }
        if (first)
            t.a += str[c];
        else
            t.b += str[c];
    }
    return move(t);
}

struct command
{
    virtual void operator()() = 0;
};

struct pswap : public command
{
    uint8_t a, b;
    virtual void operator()()
    {
        char store = programs[a];
        programs[a] = programs[b];
        programs[b] = store;
    }
    pswap(uint8_t a, uint8_t b) : a(a), b(b) {}
};
struct spin : public command
{
    uint8_t count;
    virtual void operator()()
    {
        char buff[sizeof(programs)];
        memcpy(buff, programs + (sizeof(programs) - count), count);
        memcpy(buff + count, programs, sizeof(programs) - count);
        memcpy(programs, buff, sizeof(programs));
    }
    spin(uint8_t count) : count(count) {}
};
struct partner : public pswap
{
    virtual void operator()()
    {
        char ta = a, tb = b;
        for (size_t c = 0; c < sizeof(programs); c++)
        {
            if (programs[c] == ta)
                a = c;
            if (programs[c] == tb)
                b = c;
        }
        pswap::operator()();
        a = ta;
        b = tb;
    }
    partner(uint8_t a, uint8_t b) : pswap(a, b) {}
};

void puzzle_a(const vector<command *> &commands)
{
    for (size_t c = 0, size = commands.size(); c < size; c++)
    {
        (*(commands[c]))();
    }
}

void puzzle_b(const vector<command *> &commands)
{
    #if TEST
    const size_t target = 2;
    const char *base = "abcde";
    #else
    const size_t target = 1000000000;
    const char *base = "abcdefghijklmnop";
    #endif

    vector<string> vals;
    string val;
    val.resize(sizeof(programs), '_');
    vals.push_back(base);
    cout << base << endl;
    while (true)
    {
        puzzle_a(commands);
        memcpy((void *)val.data(), programs, sizeof(programs));
        if (val == base)
            break;
        vals.push_back(val);
        cout << val << endl;
    }
    cout << "Cycle Size: " << vals.size() << endl;
    memcpy(programs, vals[target % vals.size()].data(), sizeof(programs));
}

int main(int argc, const char *argv[])
{
    config c = proc(argc, argv, __FILE__);
    
    #if TEST
    vector<string> tokens = split("s1,x3/4,pe/b", ',');
    #else
    vector<string> tokens = split(c.input, ',');
    #endif

    vector<command *> commands;
    token t;
    for (size_t c = 0, size = tokens.size(); c < size; c++)
    {
        t = split(tokens[c]);
        switch (t.type)
        {
        case 's':
            commands.push_back(new spin((uint8_t)atoi(t.a.c_str())));
            break;
        case 'p':
            commands.push_back(new partner(t.a[0], t.b[0]));
            break;
        case 'x':
            commands.push_back(new pswap(atoi(t.a.c_str()), atoi(t.b.c_str())));
            break;
        }
    }

    if (c.puzzle == 1)
        puzzle_a(commands);
    else
        puzzle_b(commands);

    char result[sizeof(programs) + 1];
    memset(result, 0, sizeof(result));
    memcpy(result, programs, sizeof(programs));
    cout << "\nResult:" << result << endl;
    return 0;
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

--- Part Two ---

Now that you're starting to get a feel for the dance moves, you turn your attention to the dance as a whole.

Keeping the positions they ended up in from their previous dance, the programs perform it again and again: including the first dance, a total of one billion (1000000000) times.

In the example above, their second dance would begin with the order baedc, and use the same dance moves:

    s1, a spin of size 1: cbaed.
    x3/4, swapping the last two programs: cbade.
    pe/b, swapping programs e and b: ceadb.

In what order are the programs standing after their billion dances?

*/