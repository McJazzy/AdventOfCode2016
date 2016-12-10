#include "advent.h"
#include <string>
#include <sstream>
#include <assert.h>
#include <regex>
#include <valarray>
#include <numeric>
#include <iostream>
#include <fstream>

template <>
std::string process<8>(std::istream & is, bool part2) {
	unsigned x{ 50 }, y{ 6 };
	std::valarray<int> r(x * y);
	std::regex rect("(r)ect (\\d+)x(\\d+)"), col("r(o)tate column x=(\\d+) by (\\d+)"),
		row("ro(t)ate row y=(\\d+) by (\\d+)");
	std::smatch m;
	std::string line;
	while (std::getline(is, line)) {
		if (std::regex_match(line,  m, rect) || std::regex_match(line, m, col) || std::regex_match(line, m, row)) {
			unsigned a = std::stoi(m.str(2)), b = std::stoi(m.str(3));
			char op = static_cast<char>(m.str(1).at(0));
			if (op == 'r') {
				std::gslice s(0, { b,a }, { x, 1 });
				r[s] = 1;
			} else if (op == 'o') {
				std::slice s{ a, y, x };
				r[s] = std::valarray<int>(r[s]).cshift(-(int)b);
			} else if (op == 't') {
				std::slice s{ a * x, x, 1 };
				r[s] = std::valarray<int>(r[s]).cshift(-(int)b);
			}
		}
	}

	if (part2) {
		for (unsigned j = 0; j < y; j++) {
			for (unsigned i = 0; i < x; i++) {
				if (r[j*x + i]) std::cout << "#";
				else std::cout << " ";
			}
			std::cout << std::endl;
		}
	}
	
	return std::to_string(r.sum());
}

template <>
void test<8>() {
	std::string s =
		"rect 3x2\n"
		"rotate column x=1 by 1\n"
		"rotate row y=0 by 4\n"
		"rotate column x=1 by 1";

	assert(process<8>(std::istringstream(s), false) == "6");
}

template <>
void solve<8>() {
	std::cout << "advent8: " << process<8>(std::ifstream("advent8.txt"), false) << std::endl;
}



