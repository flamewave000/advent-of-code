#pragma once
#ifndef _UTIL_H
#define _UTIL_H

#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <vector>
#include "../stringx/include/stringx.hpp"

inline std::string get_file(const std::string& filename) {
    return get_file(filename.c_str());
}
std::string get_file(const char * filename) {
    std::ifstream ifs(filename);
    std::string content((std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()));
    ifs.close();
    return content;
}

struct config {
    int puzzle;
    std::string input;
};

config proc(int argc, const char** argv, int day) {
    if(argc != 2 && argc != 4) exit(-1);
    std::string puzzle = argv[1];
    if(puzzle != "-1" && puzzle != "-2") exit(-1);
    int p = puzzle[1] - '0';
    if(argc == 2) {
        return { p, get_file(strx::format("data/day{0}.txt") % day % strx::endf) };
    }
    std::string dataFlag = argv[2];
    return {
        puzzle == "-1" ? 1 : 2,
        dataFlag == "-d"
            ? std::string(argv[3])
            : dataFlag == "-f"
                ? get_file(argv[2])
                : get_file(strx::format("data/day{0}.txt") % day % strx::endf)
    };
}

#endif
