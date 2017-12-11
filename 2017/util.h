#pragma once
#ifndef _UTIL_H
#define _UTIL_H

#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <vector>
#include <memory>
#include <unordered_set>
#include <functional>
#include <algorithm>
#include "../stringx/include/stringx.hpp"

std::string get_file(const char *filename)
{
    std::ifstream ifs(filename);
    std::string content((std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()));
    ifs.close();
    return content;
}

struct config
{
    int puzzle;
    std::string input;
};

config proc(int argc, const char **argv, int day)
{
    if (argc != 2 && argc != 4)
        exit(-1);
    std::string puzzle = argv[1];
    if (puzzle != "-1" && puzzle != "-2")
        exit(-1);
    int p = puzzle[1] - '0';
    if (argc == 2)
        return {p, get_file(("data/day{0}.txt"_f % day % strx::endf).c_str())};

    std::string dataFlag = argv[2];
    return {p,
            dataFlag == "-d"
                ? std::string(argv[3])
                : dataFlag == "-f"
                      ? get_file(argv[3])
                      : get_file(("data/day{0}.txt"_f % day % strx::endf).c_str())};
}

#endif
