#include "advent.h"
#include "lib/md5.h"
#include <list>
#include <array>
#include <string>
#include <vector>

struct pos {
	int x;
	int y;
	pos operator+(const pos & other) {
		return {x + other.x, y + other.y};
	}
	bool operator==(const pos & other) {
		return x == other.x && y == other.y;
	}
};
struct state {
	std::string path;
	pos p;
};

template<>
std::string process<17>(std::istream & is, bool part2) {
	std::list<state> s, found;
	std::string input;
	is >> input;
	const pos orig { 0, 0 }, end { 3, 3 };
	state orig_s;
	orig_s.path = "";
	orig_s.p = orig;
	std::vector<state> dirs = { { "U", { 0, 1 } }, { "D", { 0, -1 } },
			{ "L", {-1, 0 } }, { "R", { 1, 0 } } };

	for (s.push_back(orig_s); s.size() != 0 && !found.size(); s.pop_front()) {
		auto & curr = s.front();
		std::copy(curr.path.begin(), curr.path.end(), input.end());
		std::string md5_hash = MD5(input).hexdigest();
		for (int i = 0; i < 4; ++i) {
			if (md5_hash[i] > 'a') {
				state n{ curr.path + dirs[i].path ,curr.p + dirs[i].p};
				bool in = n.p.x > orig.x && n.p.x < end.x && n.p.y > orig.y && n.p.y < end.y;
				if (n.p == end)
					found.push_back(n);
				else if (in)
					s.push_back(n);
			}
		}
	}
	return found.back().path;
}

template<>
void solve<17>() {
	std::cout << "advent17: "
			<< process<17>(std::stringstream("hhhxzeay"), false) << std::endl;
}

template<>
void test<17>() {
	assert(process<17>(std::stringstream("ihgpwlah"), false) == "DDRRRD");
	assert(process<17>(std::stringstream("kglvqrro"), false) == "DDUDRLRRUDRD");
	assert(
			process<17>(std::stringstream("ulqzkmiv"), false)
					== "DRURDRUDDLLDLUURRDULRLDUUDDDRR");
}
