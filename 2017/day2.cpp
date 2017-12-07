#include "util.h"

using namespace std;

int puzzle_a(const string& input);
int puzzle_b(const string& input);

int main(int argc, const char *argv[])
{
    config c = proc(argc, argv);

    cout << "\n\nResult:" << (c.puzzle == 1 ? puzzle_a(c.input) : puzzle_b(c.input)) << endl;
    return 0;
}

int puzzle_a(const string& input) {
    istringstream iss(input);
    string buff;
    unsigned int sum = 0, hi = 0, lo = ~0, val = 0;
    while(getline(iss, buff)) {
        istringstream tokens(buff);
        while(!tokens.eof()) {
            tokens >> val;
            if(val < lo) lo = val;
            if(val > hi) hi = val;
        }
        printf("\nRow[hi:%4d, lo:%4d, %4d] %s", hi, lo, hi-lo, buff.c_str());
        sum += hi - lo;
        hi = val = 0;
        lo = ~0;
    }
    return sum;
}

int puzzle_b(const string& input) {
}