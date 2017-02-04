#pragma once
#include "advent.h"
#include <assert.h>
#include <valarray>
#include <deque>
#include <vector>
#include <iostream>
#include <string>
#include <list>
#include <functional>
#include <numeric>

struct pos { int x; int y; 
pos operator+(const pos & other) { return{ x + other.x, y + other.y }; }
};
struct state { pos p; unsigned dist; };

template <int N>
std::string process(int find_x, int find_y, int num, bool part2) {
	int height = find_y * 2; 
	int width = find_x * 2; 		

	std::valarray<bool> maze(width * height);
	std::valarray<int> visited(false, width * height);
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			unsigned hash = (3 + x + 2 * y) * x + (y + 1) * y + num;			
			maze[x + y * width] = (__builtin_popcount (hash) % 2) != 0;
		}		
	}
	
	std::vector<pos> dirs = { { -1,0 },{ 0,-1 },{ 1,0 },{ 0,1 } };
		
	state start = { { 1,1 }, 0 };
	std::list<state> to_visit;	
	to_visit.push_back(start);

	while (!to_visit.empty()) {		
		state current = to_visit.front();		
		to_visit.pop_front();		

		if (current.p.x == find_x && current.p.y == find_y && !part2)
			return std::to_string(current.dist);

		visited[current.p.x + current.p.y * width] = current.dist;

		for (auto p : dirs) {
			pos n = current.p + p; //new pos
			if (n.x >= 0 && n.x < width && n.y >= 0 && n.y < height) {
				if (!visited[n.x + n.y * width] && !maze[n.x + n.y * width]) {
					to_visit.push_back({ n, current.dist + 1 });
				}
			}
		}
	}

	if (part2) {
		int num = 0;
		for (int n : visited) {
			if (n <= 50 && n > 0)
				num++;
		}		
		return std::to_string(num);
	}
}

template <>
void solve<13>() {
	std::cout << "advent13: " << process<13>(31, 39, 1358, false) << std::endl;
	std::cout << "advent13.part2: " << process<13>(31, 39, 1358, true) << std::endl;
}

template <>
void test<13>() {
	assert(process<13>(7, 4, 10, false) == "11");
}

