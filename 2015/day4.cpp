#include "util.h"
#include <thread>

using namespace std;
using namespace strx;
using namespace linq;


int main(int argc, const char *argv[]) {
	config cfg = proc(argc, argv, __FILE__);

	cout << "Data: " << cfg.input << endl;

	const char *zero = cfg.puzzle == 1 ? "00000" : "000000";
	const int zero_len = strlen(zero);

	string result = "";
	uint32_t count = UINT32_MAX;
	for (; result.substr(0, zero_len) != zero; count++) {
		result = md5(cfg.input + spec("%d", count, 11));
	}
	printf("\rData: %s%d  Hash: %s", cfg.input.data(), count, result.data());
	printf("\nResult: ('%s', %d)\n", result.data(), count);
	return 0;
}

/*
http://adventofcode.com/2015/day/4

--- Day 4: The Ideal Stocking Stuffer ---

Santa needs help mining some AdventCoins (very similar to bitcoins) to use as gifts for all the economically forward-thinking little girls and boys.

To do this, he needs to find MD5 hashes which, in hexadecimal, start with at least five zeroes. The input to the MD5 hash is some secret key (your puzzle input, given below) followed by a number in decimal. To mine AdventCoins, you must find Santa the lowest positive number (no leading zeroes: 1, 2, 3, ...) that produces such a hash.

For example:

 - If your secret key is `abcdef`, the answer is `609043`, because the MD5 hash of `abcdef609043` starts with five zeroes `(000001dbbfa...)`, and it is the lowest such number to do so.
 - If your secret key is `pqrstuv`, the lowest number it combines with to make an MD5 hash starting with five zeroes is `1048970`; that is, the MD5 hash of `pqrstuv1048970` looks like `000006136ef...`.

Your puzzle answer was 346386.

--- Part Two ---

Now find one that starts with six zeroes.

Your puzzle answer was 9958218.

*/