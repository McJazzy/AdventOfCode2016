#include "advent.h"
#include <vector>
#include <tuple>
#include <set>
#include <cmath>
#include <functional>
#include <algorithm>

enum dir {
	north, east, south, west, num_dirs
};

unsigned int manhattan(int x, int y) {
	return std::abs(x) + std::abs(y);
}

template<typename T>
void rot(std::vector<T> & v, bool right) {
	if (!right)
		std::rotate(v.rbegin(), v.rbegin() + 1, v.rend());
	else
		std::rotate(v.begin(), v.begin() + 1, v.end());
}

template<>
std::string process<1>(std::istream & is, bool part2) {
	char d;
	std::vector<std::function<std::tuple<int, int>(int, int, int)> > funcs = {
			[](int x, int y, int len) {return std::make_tuple(x,y + len);}, // north
			[](int x, int y, int len) {return std::make_tuple(x + len,y);}, // east
			[](int x, int y, int len) {return std::make_tuple(x,y - len);}, // south
			[](int x, int y, int len) {return std::make_tuple(x - len,y);}, // west
			};

	typedef std::pair<int, int> point;
	std::set<point> visited;

	int x = 0;
	int y = 0;
	for (std::string line; std::getline(is, line);) {
		std::stringstream ss(line);
		for (int dist; ss >> d >> dist; ss.ignore(1, ',')) {
			rot(funcs, d == 'R');
			for (int i = 0; i < dist; i++) {
				std::tie(x, y) = funcs[0](x, y, 1);
				if (part2 && !visited.emplace(x, y).second) {
					return std::to_string(manhattan(x, y));
				}
			}
		}
	}

	return std::to_string(manhattan(x, y));
}

template<>
void solve<1>() {
	std::ifstream ifs("advent1.txt");
	std::cout << "advent1: " << process<1>(ifs, false) << std::endl;
	std::cout << "advent1.part2: "
			<< process<1>(std::ifstream("advent1.txt"), true) << std::endl;
}

template<>
void test<1>() {
	assert(process<1>(std::istringstream("R5, L5, R5, R3"), false) == "12");
	assert(process<1>(std::istringstream("R2, R2, R2"), false) == "2");
	assert(process<1>(std::istringstream("R2, L3, R5"), false) == "10");
	assert(process<1>(std::istringstream("R8, R4, R4, R8"), true) == "4");
}
