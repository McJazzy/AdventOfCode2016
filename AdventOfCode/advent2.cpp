#include "advent.h"
#include <map>
#include <functional>
#include <math.h>
#include <algorithm>

template<class T>
const T& clamp(const T& x, const T& upper, const T& lower) {
	return std::min(upper, std::max(x, lower));
}

std::string process(std::string input, bool part2 = false) {
	std::stringstream ss(input);
	std::string line;
	typedef std::pair<int, int> point;

	int x, y;
	if (part2) {
		x = 0; y = 2;
	}
	else {
		x = 1; y = 1;
	}
	
	std::string res = "";

	std::map<point, char> translation = {
		{ {0 , 0}, '1'},
		{ {0 , 1}, '4'},
		{ {0, 2}, '7'},
		{ { 1, 2 }, '2' },
		{ { 1, 2 }, '5' },
		{ { 1, 2 }, '8' },
		{ { 2, 2 }, '3' },
		{ { 2, 2 }, '6' },
		{ { 2, 2 }, '9' }
	};

//	typedef std::pair<
	std::map<point, char> translation2 = {
		{ { 2, 0 }, '1'},
		{ { 1, 1 }, '2'},
		{ { 2, 1 }, '3' },
		{ { 2, 1 }, '4' },
		{ { 0, 2 }, '5' },
		{ { 1, 2 }, '6' },
		{ { 2, 2 }, '7' },
		{ { 3, 2 }, '8' },
		{ { 4, 2 }, '9' },
		{ { 1, 3 }, 'A' },
		{ { 2, 3 }, 'B' },
		{ { 3, 3 }, 'C' },
		{ { 2, 4 }, 'D' },
	};

	auto clamp1 = [](point p) {return std::make_pair(clamp(0, 2, p.first), clamp(0, 2, p.second)); };
	auto clamp2 = [](point p, bool y) {
		if (y)
			return std::make_pair(p.first, clamp(abs(2 - p.first), 4 - abs(2 - p.first), p.second));
		else 
			return std::make_pair(clamp(abs(2 - p.second), 4 - abs(2 - p.second), p.first), p.second);		
	};

	std::map<char, std::function<std::pair<int, int>(int, int)>> move = {
		{'U', [](int x, int y) { return std::make_pair(x, y-1); } },
		{'L', [](int x, int y) { return std::make_pair(x-1, y); } },
		{ 'D', [](int x, int y) { return std::make_pair(x, y+1); } },
		{ 'R', [](int x, int y) { return std::make_pair(x+1, y); } },
	};

	while (std::getline(ss, line)) {		
		for (auto c : line) {
			if (!part2)
				std::tie(x, y) = clamp1(move[c](x, y));
			else
				std::tie(x, y) = clamp2(move[c](x, y), (c == 'U' || c == 'D'));
		}
		if (!part2)
			res += std::to_string(y * 3 + x + 1);
		else
			res += translation2[std::pair<int, int>(x, y)];		
	}
	return res;
}

template<>
void solve<2>() {
	std::ifstream f("advent2.txt");
	std::string s = std::string((std::istreambuf_iterator<char>(f)),(std::istreambuf_iterator<char>()));
	std::cout << "advent2: " << process(s) << std::endl;
	std::cout << "advent2.part2: " << process(s, true) << std::endl;
}

template<>
void test<2>() {
	assert(process("ULL\nRRDDD\nLURDL\nUUUUD") == "1985");
	assert(process("ULL\nRRDDD\nLURDL\nUUUUD", true) == "5DB3");
}
