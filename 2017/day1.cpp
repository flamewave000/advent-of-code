#include <iostream>
#include "util.h"

using namespace std;

int puzzle_a(const string& input);
int puzzle_b(const string& input);

int main(int argc, const char *argv[])
{
    config c = proc(argc, argv);

    cout << "Performing sum of: " << c.input;
    cout << "\n\nTotal Sum:" << (c.puzzle == 1 ? puzzle_a(c.input) : puzzle_b(c.input)) << endl;
    return 0;
}

int puzzle_a(const string& input) {
    int sum = 0;
    for(size_t c = 0, size = input.size(); c < size; c++) {
        if((c == size - 1 && input[c] == input[0]) || input[c] == input[c+1])
            sum += input[c] - '0';
    }
    return sum;
}

int puzzle_b(const string& input) {
    int sum = 0;
    size_t c = 0;
    size_t size = input.size();
    size_t half = size / 2;
    for(; c < size; c++) {
        if(input[c] == input[(c + half) % size])
            sum += input[c] - '0';
    }
    return sum;
}