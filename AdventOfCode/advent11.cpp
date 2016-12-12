#include "advent.h"
#include <regex>
#include <array>
#include <sstream>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <set>
#include <queue>

std::map<std::string, unsigned> floors = { {"first", 0}, {"second", 1}, {"third", 2}, {"fourth",4} };
std::map<std::string, unsigned> elems = { { "strontium", 0 },{ "curium", 1 },{ "plutonium", 2 },{ "thulium", 3 }, {"ruthenium",4}, {"hydrogen", 5}, {"lithium",6}, {"elerium",7}, {"dilithium",8}};
enum device_type {microchip, generator, num_devices};
enum dir {up, down, num_dirs};
enum { num_floors = 4};

typedef std::pair<device_type, unsigned> device;
typedef std::array<std::set<device>, num_floors> dev_list;
typedef std::pair<dir, std::vector<device>> move;

struct state {
	unsigned e;
	dev_list d;
	unsigned num_elem;
	unsigned steps;	
	unsigned dist;
	std::set<device> current() const {
		return d[e];
	}

	bool valid_state() const {
		const std::set<device> & l = d[e];
		// iterate over all microchips
		auto it = std::find_if(l.begin(), l.end(), [](const device &d) {
			return d.first == microchip;
		});

		for (; it != l.end(); ++it) {
			bool matching_gen = std::find_if(l.begin(), l.end(), [it](const device & d) {
				return d.first == generator && it->second == d.second;
			}) != l.end();

			bool non_matching_gen = std::any_of(l.begin(), l.end(), [it](const device & d) {
				return d.first == generator && it->second != d.second;
			});

			if (non_matching_gen && !matching_gen)
				return false;
		}

		return true;
	}
};

void alg(const state & s, std::vector<move> & moves, std::vector<state> & states, std::vector<unsigned> & scores);

std::vector<std::vector<unsigned >> comb(unsigned N, unsigned K)
{
	if (N < 2) return{ {} };
	std::vector<std::vector<unsigned>> c;
	std::string bitmask(K, 1); // K leading 1's
	bitmask.resize(N, 0); // N-K trailing 0's

// print integers and permute bitmask
	do {
		std::vector<unsigned> combination;
		for (int i = 0; i < N; ++i) // [0..N-1] integers
		{
			if (bitmask[i]) combination.push_back(i);
		}
		c.push_back(combination);
	} while (std::prev_permutation(bitmask.begin(), bitmask.end()));
	return c;
}

bool repeated_move(const move & last, const move & current) {
	return last.first != current.first && last.second == current.second;
}

bool repeated_state(std::vector<state> & states, const state & s) {
	bool repeated = false;
	
	for (const state & o : states) {
		if (s.steps >= o.steps && o.d == s.d) {				
			return true;
		}
	}	
	return repeated;
}

bool finished(const state & s) {
	if (s.e != 3)
		return false;

	unsigned c = 0;
	unsigned g = 0;
	for (auto d : s.d[3]) {
		if (d.first == microchip)
			c++;
		else
			g++;
	}	
	return c == s.num_elem && g == s.num_elem;
}

state apply_move(const state & s, const move & m) {
	//make a copy
	state new_state(s);		
	auto & cur = new_state.d[s.e];

	if (s.e == 0 && m.first == down) {
		std::cout << "not a valid move!" << std::endl;
		return new_state;
	}

	new_state.steps++;
	new_state.e = m.first == down ? new_state.e - 1 : new_state.e + 1;

	for (auto & dev : m.second) {
		// remove from current floor
		cur.erase(dev);
		
		// add to new floor
		new_state.d[new_state.e].emplace(dev);
	}

	return new_state;
}

void alg(const state & s, std::vector<move> & moves, std::vector<state> & states, std::vector<unsigned> & scores) {
	//std::cout << "step: " << s.steps << std::endl;
	if (finished(s))
	{
		std::cout << "found : " << moves.size() << std::endl;
		scores.push_back(s.steps);		
		//return true;
	}  else {
		if (!s.valid_state()) {
			//std::cout << "alg() - invalid" << std::endl;
			return;
		}

		std::set<device> & cur = s.current();
		std::vector<device> vec(cur.begin(), cur.end());

		for (int _d = up; _d < num_dirs; _d++) {
			dir d = (dir)_d;
			if (s.e == 3 && d == up)
				continue;
			if (s.e == 0 && d == down)
				continue;

			if (s.e == 1 && d == down && s.d[0].empty())
				continue;
			else if (s.e == 2 && d == down && s.d[0].empty() && s.d[1].empty())
				continue;		

			std::vector<move> next_moves;

			if (d == up) {
				if (cur.size() > 1) {
					auto v = comb(cur.size(), 2);
					for (auto c : v) {
						move m{ (dir)d, std::vector<device>{ vec[c[0]], vec[c[1]]} };
						next_moves.push_back(m);
					}
				}

				// onesies
				for (auto dev : cur) {
					move m{ (dir)d, std::vector<device>{ dev } };
					next_moves.push_back(m);
				}
			}
			else {
				// onesies
				for (auto dev : cur) {
					move m{ (dir)d, std::vector<device>{ dev } };
					next_moves.push_back(m);
				}
				if (cur.size() > 1) {
					auto v = comb(cur.size(), 2);
					for (auto c : v) {
						move m{ (dir)d, std::vector<device>{ vec[c[0]], vec[c[1]]} };
						next_moves.push_back(m);
					}
				}
			}


			for (auto & m : next_moves) {
				state new_state(apply_move(s, m));
				if (new_state.valid_state() && (moves.empty() || (!repeated_move(moves.back(), m) && !repeated_state(states, new_state)))) {
					moves.push_back(m);
					states.push_back(new_state);					
					alg(new_state, moves, states, scores);
					moves.pop_back();
					break;
				}
			}
		}		
	}
	return;
}

template <>
std::string process<11>(std::istream & is, bool part2) {
	std::regex floor_regex("(first|second|third|fourth)");
	std::regex elems_regex("(strontium|curium|plutonium|thulium|ruthenium|hydrogen|lithium|dilithium|elerium)(-compatible micro(c)hip| (g)enerator)");
	std::smatch m;
	state dlist;
	unsigned elem_count = 0;

	for (std::string line; std::getline(is, line); ) {
		unsigned floor = floors.size();
		unsigned elem = elems.size();
		device_type dev = num_devices;

		if (std::regex_search(line, m, floor_regex)) {
			floor = floors[m.str(1)];
		}
		auto words_begin = std::sregex_iterator(line.begin(), line.end(), elems_regex);
		auto words_end = std::sregex_iterator();
		for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
			elem = elems[i->str(1)];
			dev = i->str(3) == "c" ? microchip : generator;
			if (dev == microchip) elem_count++;
			if (floor < floors.size() && elem < elems.size())
				dlist.d[floor].emplace(std::make_pair(dev, elem));
		}		

		if (part2) {
			dlist.d[0].emplace(std::make_pair(microchip, 7));
			dlist.d[0].emplace(std::make_pair(generator, 7));
			dlist.d[0].emplace(std::make_pair(microchip, 8));
			dlist.d[0].emplace(std::make_pair(generator, 8));
		}
	}

	// start at 0
	dlist.num_elem = elem_count;
	dlist.e = 0;
	dlist.steps = 0;

	std::vector<state> states;
	states.push_back(dlist);
	std::vector<move> moves;
	std::vector<unsigned> scores;
	alg(dlist, moves, states, scores);
	//std::cout << "advent11: " <<  << std::endl;	
	
	return std::to_string(*std::min_element(scores.begin(), scores.end()));
}

template <>
void solve<11>() {
	std::cout << "advent11: " << process<11>(std::ifstream("advent11.txt"), false) << std::endl;
	std::cout << "advent11.part2: " << process<11>(std::ifstream("advent11.txt"), true) << std::endl;
}

template <>
void test<11>() {
	std::string input =
		"The first floor contains a hydrogen-compatible microchip and a lithium-compatible microchip.\n"
		"The second floor contains a hydrogen generator.\n"
		"The third floor contains a lithium generator.\n"
		"The fourth floor contains nothing relevant.";

	assert(process<11>(std::istringstream(input), false) == "11");
}