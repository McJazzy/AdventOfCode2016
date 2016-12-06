#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <unordered_set>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include <cmath>
#include "advent1.h"
#include <boost/functional/hash.hpp>

typedef boost::char_separator<char> sep_type;

void update_dir(enum dir & d, bool r) {
	d = r ? (dir)((d + 1) % num_dirs) : (dir)((d + 3) % num_dirs);	
}

unsigned int manhattan(int x, int y) {
	return std::abs(x) + std::abs(y);
}

template <typename T>
void rotate(std::vector<T> & v, bool right) {
	if (!right)
		std::rotate(v.rbegin(), v.rbegin() + 1, v.rend());
	else
		std::rotate(v.begin(), v.begin() + 1, v.end());
}

unsigned int process(const std::string & input, dir d = north, bool twice = false) {
	std::vector<std::string> dirs;
	boost::split(dirs, input, boost::is_any_of(","));
	
	typedef std::pair <int, int> point;
	std::unordered_set<point, boost::hash<point>> visited;

	int x = 0; int y = 0;

	std::vector<std::function<std::tuple<int, int>(int, int, int)>> funcs =
	{
		[](int x, int y, int len) { return std::make_tuple(x,y + len); }, // north
		[](int x, int y, int len) { return std::make_tuple(x + len,y); }, // east
		[](int x, int y, int len) { return std::make_tuple(x,y - len); }, // south
		[](int x, int y, int len) { return std::make_tuple(x - len,y); }, // west
	};

	visited.insert(point(x, y));

	for (auto s : dirs) {
		boost::trim(s);
		update_dir(d, s.at(0) == 'R');
		int length = std::stoi(s.substr(1));		
		rotate(funcs, s.at(0) == 'R');
		for (int i = 0; i < length; i++) {
			std::tie(x, y) = funcs[0](x, y, 1);
			if (twice) {
				point p(x, y);
				if (visited.find(p) != visited.end())
					return manhattan(x, y);
				else
					visited.insert(p);
			}
		}
	}
	return manhattan(x, y);
}

void advent1(){ 
	std::ifstream ifs("advent1.txt");
	
	std::string line;
	std::string f;	

	while (std::getline(ifs, line)) {
		f += line;
	}
	unsigned int dist = process(f);

	unsigned int dist2 = process(f, north, true);

	std::cout << f << std::endl <<  "advent1: " << dist << std::endl;
	std::cout << "advent1.part2: " << dist2 << std::endl;
}

void test_advent1() {
	/*assert(process("R5, L5, R5, R3") == 12);
	assert(process("R2, R2, R2") == 2);
	assert(process("R2, L3, R5") == 10);*/

	assert(process("R8, R4, R4, R8", north, true) == 4);
}