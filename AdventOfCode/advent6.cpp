#include "advent6.h"
#include <string>
#include <array>
#include <sstream>
#include <fstream>
#include <istream>
#include <iostream>
#include <assert.h>
#include <vector>
#include <map>

#pragma warning(disable:4996)

std::string process(std::istream & is, bool part2 = false) {
	std::string line;
	std::array<std::map<char, int>, 8> freq;
	std::string res;
	while (std::getline(is, line)) {
		for (unsigned i(0); i < line.size(); i++)
			freq[i][line[i]]++;
	}
	for (auto & f : freq) {		
		if(!f.empty())
			res += std::max_element(f.begin(), f.end(), [part2](const auto & lhs, const auto & rhs) {
				return part2 ? lhs.second > rhs.second : lhs.second < rhs.second; })->first;
	}
	return res.substr(0,line.size());
}

void advent6() {
	std::ifstream ifs("advent6.txt");	
	std::cout << "advent6: " << process(ifs) << std::endl;
	std::ifstream ifs2("advent6.txt");	
	std::cout << "advent6.part2: " << process(ifs2, true) << std::endl;
}


void test_advent6() {
	std::string input(
	"eedadn\n"
	"drvtee\n"
	"eandsr\n"
	"raavrd\n"
	"atevrs\n"
	"tsrnev\n"
	"sdttsa\n"
	"rasrtv\n"
	"nssdts\n"
	"ntnada\n"
	"svetve\n"
	"tesnvt\n"
	"vntsnd\n"
	"vrdear\n"
	"dvrsen\n"
	"enarar");

	std::istringstream is;
	is.str(input);
	assert(process(is) == "easter");
	is.clear();
	is.str(input);
	assert(process(is, true) == "advent");
}