#pragma once
#ifndef _UTIL_H
#define _UTIL_H

#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <sstream>

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

config proc(int argc, const char** argv) {
    if(argc < 3 || argc > 4) exit(-1);
    std::string puzzle = argv[1];
    if(puzzle != "-1" && puzzle != "-2") exit(-1);
    return {
        puzzle == "-1" ? 1 : 2,
        std::string(argv[2]) == "-d" ? std::string(argv[3]) : get_file(argv[2])
    };
}

#endif