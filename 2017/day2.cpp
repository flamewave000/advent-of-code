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
    istringstream iss(input);
    string buff;
    unsigned int sum = 0, val = 0;
    int c = 0;
    vector<int> values;
    while(getline(iss, buff)) {
        istringstream tokens(buff);
        while(!tokens.eof()) {
            tokens >> val;
            c = values.size() - 1;
            for(; c >= 0; c--) {
                if(values[c] % val == 0 || val % values[c] == 0)
                    break;
            }
            // if we found a pair
            if(c >= 0) {
                int div = val > values[c] ? val / values[c] : values[c] / val;
                printf("\nRow[a:%4d, b:%4d, v:%4d] %s", val, values[c], div, buff.c_str());
                sum += div;
                break;
            }
            values.push_back(val);
        }
        values.clear();
    }
    return sum;
}