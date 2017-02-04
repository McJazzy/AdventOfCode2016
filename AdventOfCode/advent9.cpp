#include "advent.h"
#include <iostream>
#include <fstream>
#include <assert.h>
#include <sstream>
#include <vector>
#include <string>
#include <regex>
#include <string>

const static std::regex r("\\((\\d+)x(\\d+)\\)");

long long decode(const std::string & s, bool part2) {
	std::smatch m;
	if (!std::regex_search(s, m, r))
		return s.length();
	long long consume = std::stoi(m.str(1)), times = std::stoi(m.str(2));
	std::string inner = s.substr(m.position() + m.length(), consume);
	std::string rest = s.substr(m.position() + m.length() + consume);
	long long inner_length = decode(inner, part2);
	long long consumed = !part2 ? consume : inner_length;
	return m.position() + consumed * times + decode(rest, part2);
}

long long decode(std::string && s, bool part2) {
	return decode(s, part2);
}

template <>
std::string process<9>(std::istream & is, bool part2) {
	std::vector<std::pair<unsigned, unsigned>> dec;
	long long length = 0;
	for (std::string line; std::getline(is, line);) {		
		length += decode(line, part2);
	}
	return std::to_string(length);
}

template <>
void test<9>() {
	assert(process<9>(std::istringstream("ADVENT"), false) == "6");
	assert(process<9>(std::istringstream("A(1x5)BC"), false) == "7");
	assert(process<9>(std::istringstream("(3x3)XYZ"), false) == "9");
	assert(process<9>(std::istringstream("A(2x2)BCD(2x2)EFG"), false) == "11");
	assert(process<9>(std::istringstream("(6x1)(1x3)A"), false) == "6");
	assert(process<9>(std::istringstream("X(8x2)(3x3)ABCY"), false) == "18");
	assert(process<9>(std::istringstream("(25x3)(3x3)ABC(2x3)XY(5x2)PQRSTX(18x9)(3x2)TWO(5x7)SEVEN"), true) == "445");
	assert(process<9>(std::istringstream("(27x12)(20x12)(13x14)(7x10)(1x12)A"), true) == "241920");	
	assert(process<9>(std::istringstream("X(8x2)(3x3)ABCY"), true) == "20");
}

template <>
void solve<9>() {
	std::cout << "advent9: " << process<9>(std::ifstream("advent9.txt"), false) << std::endl;
	std::cout << "advent9.part2: " << process<9>(std::ifstream("advent9.txt"), true) << std::endl;
}
