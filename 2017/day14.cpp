#include "util.h"
#include <bitset>

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

union sector {
    uint8_t data[16];
    uint64_t blocks[2];
    sector()
    {
        blocks[0] = 0;
        blocks[1] = 0;
    }
};
void print_row(const sector &sec)
{
    for (int c = 0; c < 64; c++)
    {
        cout << (char)(((sec.blocks[0] >> (63 - c)) & 1) + '0');
    }
    for (int c = 0; c < 64; c++)
    {
        cout << (char)(((sec.blocks[1] >> (63 - c)) & 1) + '0');
    }
    cout << endl;
}

sector generate_hash(const string &data);
const char HEX[17] = "0123456789abcdef";
int puzzle_a(const string &input)
{
    sector sec;
    int count = 0;
    for (int c = 0; c < 128; c++)
    {
        sec = generate_hash("{0}-{1}"_f % input % c % endf);
        // Reverse byte order for endian-ness when read as uint64_t
        std::reverse(&(sec.data[0]), &(sec.data[8]));
        std::reverse(&(sec.data[8]), &(sec.data[16]));
        printf("%s-%d: %lx %lx\n", input.c_str(), c, sec.blocks[0], sec.blocks[1]);
        int bits = 0;
        for (int i = 0; i < 64; i++)
        {
            bits += ((sec.blocks[0] >> i) & 1) + ((sec.blocks[1] >> i) & 1);
        }
        count += bits;
        cout << bits << ": ";
        print_row(sec);
    }
    return count;
}

int puzzle_b(const string &input)
{
}

const int RING_SIZE = 256;
const int SPARSE_HASH_ITERATION_COUNT = 64;
void reverse(uint8_t ring[RING_SIZE], int index, uint8_t length)
{
    if (length == 1)
        return;
    // half length, ignoring odd numbers as their middle number does not change
    uint8_t halfLength = length >> 1;
    uint8_t temp = 0;
    int a = 0, b = 0;
    for (int c = 0; c < halfLength; c++)
    {
        a = (index + c) % RING_SIZE;
        b = (index + (length - (c + 1))) % RING_SIZE;
        temp = ring[a];
        ring[a] = ring[b];
        ring[b] = temp;
    }
}
sector generate_hash(const string &data)
{
    auto lengths = from(data.c_str(), data.size()).select<uint8_t>([](auto item) -> uint8_t { return (uint8_t)item; });
    lengths.push_back((uint8_t)17); // DC1
    lengths.push_back((uint8_t)31); // US
    lengths.push_back((uint8_t)73); // I
    lengths.push_back((uint8_t)47); // /
    lengths.push_back((uint8_t)23); // ETB
    uint8_t ring[RING_SIZE];
    for (int c = 0; c < RING_SIZE; c++)
        ring[c] = (uint8_t)c;
    // Generate the sparse hash by iterating the lengths SPARSE_HASH_ITERATION_COUNT times
    for (int iterations = 0, index = 0, skip = 0; iterations < SPARSE_HASH_ITERATION_COUNT; iterations++)
    {
        // Perform ring twists based on the list of lengths
        for (size_t c = 0, size = lengths.size(); c < size; c++, skip++)
        {
            // Perform twist
            reverse(ring, index, lengths[c]);
            // Update index position
            index = (index + (int)lengths[c] + skip) % RING_SIZE;
        }
    }
    // Generate dense hash
    sector sec;
    const int dense_size = 16;
    for (int section = 0, offset = 0, part;
         section < dense_size; section++)
    {
        offset = section * dense_size;
        sec.data[section] = ring[offset];
        for (part = 1; part < dense_size; part++)
        {
            sec.data[section] ^= ring[offset + part];
        }
    }
    return std::move(sec);
}

/*
http://adventofcode.com/2017/day/14

--- Day 14: Disk Defragmentation ---
Suddenly, a scheduled job activates the system's disk defragmenter. Were the situation different, you might sit and watch it for a while, but today, you just don't have that kind of time. It's soaking up valuable system resources that are needed elsewhere, and so the only option is to help it finish its task as soon as possible.
The disk in question consists of a 128x128 grid; each square of the grid is either free or used. On this disk, the state of the grid is tracked by the bits in a sequence of knot hashes.
A total of 128 knot hashes are calculated, each corresponding to a single row in the grid; each hash contains 128 bits which correspond to individual grid squares. Each bit of a hash indicates whether that square is free (0) or used (1).
The hash inputs are a key string (your puzzle input), a dash, and a number from 0 to 127 corresponding to the row. For example, if your key string were flqrgnkx, then the first row would be given by the bits of the knot hash of flqrgnkx-0, the second row from the bits of the knot hash of flqrgnkx-1, and so on until the last row, flqrgnkx-127.
The output of a knot hash is traditionally represented by 32 hexadecimal digits; each of these digits correspond to 4 bits, for a total of 4 * 32 = 128 bits. To convert to bits, turn each hexadecimal digit to its equivalent binary value, high-bit first: 0 becomes 0000, 1 becomes 0001, e becomes 1110, f becomes 1111, and so on; a hash that begins with a0c2017... in hexadecimal would begin with 10100000110000100000000101110000... in binary.
Continuing this process, the first 8 rows and columns for key flqrgnkx appear as follows, using # to denote used squares, and . to denote free ones:
    ##.#.#..-->
    .#.#.#.#   
    ....#.#.   
    #.#.##.#   
    .##.#...   
    ##..#..#   
    .#...#..   
    ##.#.##.-->
    |      |   
    V      V   
In this example, 8108 squares are used across the entire 128x128 grid.
Given your actual key string, how many squares are used?

*/