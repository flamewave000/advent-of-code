#pragma once
#ifndef _UTIL_H
#define _UTIL_H

#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <vector>
#include <stack>
#include <tuple>
#include <memory>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <math.h>
#include <list>
#include "byte.hpp"
#include "c_array.hpp"
#include "md5.hpp"

#ifndef NO_LINQ
#include "../linq/include/linq.hpp"
#endif

#ifndef NO_STRINGX
#include "../stringx/include/stringx.hpp"
#endif

#define guard(cond) if(!(cond))
#define PRINT_LINE() printf("LINE %d\n", __LINE__)
#define PRINT_FLINE() printf("LINE %s:%d\n", __FILE__, __LINE__)

#define PROGRESS(curr, max, skip) if((curr % skip) == 0) { printf("\r%.1f%%", ((float)curr / (float)max) * 100.0f); fflush(stdout); }
#define MPROGRESS(msg, perc) printf("\r%s %.2f", msg, perc)


#define PERF_START() auto __start__ = std::chrono::high_resolution_clock::now()
#define PERF_END() (std::chrono::duration<float, std::milli>(std::chrono::high_resolution_clock::now() - __start__).count())

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

config proc(int argc, const char **argv, std::string file_name)
{
	std::string day = strx::split(file_name, '.')[0];
	if (argc != 2 && argc != 4)
		exit(-1);
	std::string puzzle = argv[1];
	if (puzzle != "-1" && puzzle != "-2")
		exit(-1);
	int p = puzzle[1] - '0';
	if (argc == 2)
		return {p, get_file(("data/{0}.txt"_f % day % strx::endf).c_str())};

	std::string dataFlag = argv[2];
	return {p,
			dataFlag == "-d"
				? std::string(argv[3])
				: dataFlag == "-f"
					  ? get_file(argv[3])
					  : get_file(("data/{0}.txt"_f % day % strx::endf).c_str())};
}

#endif
