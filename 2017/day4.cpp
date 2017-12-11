#include "util.h"

using namespace std;
using namespace strx;

void puzzle_a(const string &input);
void puzzle_b(const string &input);

int main(int argc, const char *argv[])
{
    config c = proc(argc, argv, 4);

    cout << "Data: " << c.input << '\n';
    if (c.puzzle == 1)
        puzzle_a(c.input);
    else
        puzzle_b(c.input);
    return 0;
}

void puzzle_a(const string &input)
{
    vector<string> lines = split(input, '\n');

    int good = 0;
    int bad = 0;
    unordered_set<string> set;
    for (auto line : lines)
    {
        auto words = split(line, ' ');
        if (words.size() <= 1)
        {
            bad++;
            continue;
        }
        set.clear();
        for (auto word : words)
        {
            if (set.find(word) != set.end())
                break;
            set.insert(word);
        }
        if (set.size() == words.size())
            good++;
        else
            bad++;
    }
    cout << " bad: " << bad << '\n';
    cout << "good: " << good << endl;
}

void puzzle_b(const string &input)
{
}

/*

--- Day 4: High-Entropy Passphrases ---
A new system policy has been put in place that requires all accounts to use a passphrase instead of simply a password. A passphrase consists of a series of words (lowercase letters) separated by spaces.
To ensure security, a valid passphrase must contain no duplicate words.
For example:
aa bb cc dd ee is valid.
aa bb cc dd aa is not valid - the word aa appears more than once.
aa bb cc dd aaa is valid - aa and aaa count as different words.
The system's full passphrase list is available as your puzzle input. How many passphrases are valid?

--- Part Two ---
For added security, yet another system policy has been put in place. Now, a valid passphrase must contain no two words that are anagrams of each other - that is, a passphrase is invalid if any word's letters can be rearranged to form any other word in the passphrase.
For example:
abcde fghij is a valid passphrase.
abcde xyz ecdab is not valid - the letters from the third word can be rearranged to form the first word.
a ab abc abd abf abj is a valid passphrase, because all letters need to be used when forming another word.
iiii oiii ooii oooi oooo is valid.
oiii ioii iioi iiio is not valid - any of these words can be rearranged to form any other word.
Under this new system policy, how many passphrases are valid?

*/