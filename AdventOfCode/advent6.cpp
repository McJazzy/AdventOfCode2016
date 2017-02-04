#include "advent.h"

#include <array>
#include <vector>
#include <map>
#include <algorithm>

template<>
std::string process<6>(std::istream & is, bool part2) {
	std::array<std::map<char, int>, 8> freq;
	std::string res;
	std::string line;
	for (;std::getline(is, line);) {
		for (unsigned i(0); i < line.size(); i++)
			freq[i][line[i]]++;
	}
	for (auto & f : freq) {
		if (!f.empty())
			res +=
					std::max_element(f.begin(), f.end(),
							[part2](const auto & lhs, const auto & rhs) {
								return part2 ? lhs.second > rhs.second : lhs.second < rhs.second;})->first;
	}
	return res.substr(0, line.size());
}

template<>
void solve<6>() {
	std::ifstream ifs("advent6.txt");
	std::cout << "advent6: " << process<6>(ifs, false) << std::endl;
	std::ifstream ifs2("advent6.txt");
	std::cout << "advent6.part2: " << process<6>(ifs2, true) << std::endl;
}

template<>
void test<6>() {
	std::string input("eedadn\n"
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

	assert(process<6>(std::istringstream(input), false) == "easter");
	assert(process<6>(std::istringstream(input), true) == "advent");
}
