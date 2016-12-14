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

// micro - gen
typedef std::pair < dir, std::vector<unsigned>> move;
typedef std::vector<unsigned> dev_list;

inline uint64_t hash(uint64_t a, uint64_t b)
{
	return (uint64_t)1 << (a << 4) << (b << 2);
}
inline unsigned first(unsigned p) { return (p & 0xff00) >> 8; }
inline unsigned second(unsigned p) { return p & 0xff; }
inline unsigned mp(unsigned first, unsigned second) { return first << 8 | second; }
inline void set_first(unsigned v, unsigned & prev)  { prev = ((v << 8) | (prev & 0x00FF)); }
inline void set_second(unsigned v, unsigned & prev) { prev = (prev & 0xFF00) | v; }

struct state {
	unsigned elev;
	dev_list d;
	unsigned steps;	
	unsigned dist;
	
	void add_elem(unsigned m, unsigned g) {		
			d.push_back(mp(m,g));		
	}

	void calc_dist() {
		//calculate distance to all unit on 4th floor
		dist = steps;
		for (unsigned dev : d)
			dist += 10 * (2 * (num_floors - 1) - first(dev) - second(dev)) * (2 * (num_floors - 1) - first(dev) - second(dev));
	}

	bool equals(const state &  other) const {
		if (elev != other.elev)
			return false;
				
		// compare 2 unsorted vectors
		uint64_t this_vec = 0, other_vec = 0;
		for (unsigned i = 0; i < d.size(); i++) {
			this_vec += hash(first(d[i]), second(d[i]));
			other_vec += hash(first(other.d[i]), second(other.d[i]));
		}
		return this_vec == other_vec;
	}

	unsigned dev_on_floor(unsigned n) const {
		unsigned sum = 0;
		for (auto dev : d) {
			sum += first(dev) == n;
			sum += second(dev) == n;
		}
		return sum;
	}
};



bool valid_state(const dev_list & d) {
	bool prot = true;
	for (int i = 0; i < d.size(); ++i) {
		unsigned dev = d[i];
		unsigned mchip_floor = first(dev);
		if (first(dev) != second(dev)) {
			for (int j = 0; j < d.size(); j++) {
				if (second(d[j]) == mchip_floor)
					return false;
			}
		}
	}
	return prot;
}

// N= 3 returns (0,1)(1,2)(0,2)
const std::vector<std::pair<unsigned, unsigned>> & comb2(unsigned N)
{
	static bool init;
	static std::array<std::vector<std::pair<unsigned, unsigned>>,16> arr;

	if (!init) {
		for (unsigned num = 2; num < 16; num++) {
			std::vector<std::vector<unsigned>> c;
			std::string bitmask(2, 1); // K leading 1's
			bitmask.resize(num, 0); // N-K trailing 0's								  
			do {
				std::vector<unsigned> combination;
				for (int i = 0; i < num; ++i) // [0..N-1] integers
				{
					if (bitmask[i]) combination.push_back(i);
				}
				arr[num].push_back(std::make_pair(combination.at(0), combination.at(1)));
				
			} while (std::prev_permutation(bitmask.begin(), bitmask.end()));
		}		
		init = true;
	}
	
	return arr[N];	
}

bool repeated_state(std::vector<state> & states, const state & s) {
	bool repeated = false;

	for (auto it = states.rbegin(); it != states.rend(); ++it) {
		if (s.equals(*it)) {				
			return true;
		}
	}	
	return repeated;
}

bool finished(const state & s) {	
	if (s.elev != num_floors - 1)
		return false;
	
	for (int i = 0; i < s.d.size(); ++i)
	{
		if (first(s.d[i]) != num_floors - 1 || second(s.d[i]) != num_floors - 1)
			return false;
	}
	return true;
}

state apply_move(const state & s, move & m) {	
	int count = 0;
	state new_state(s);
	new_state.steps++;
	new_state.elev = m.first == down ? new_state.elev - 1 : new_state.elev + 1;

	for (unsigned idx : m.second) {
		count = 0;
		for (unsigned i = 0; i < s.d.size(); ++i) {
			if (first(s.d[i]) == s.elev) {
				count++;
				if (idx == count - 1) {
					set_first((m.first == up) ? first(s.d[i]) + 1 : first(s.d[i]) - 1, new_state.d[i]);
					break;
				}				
			}
			if (second(s.d[i]) == s.elev) {
				count++;
				if (idx == count - 1) {
					set_second((m.first == up) ? second(s.d[i]) + 1 : second(s.d[i]) - 1, new_state.d[i]);
					break;
				}				
			}
		}
	}

	new_state.calc_dist();
	return new_state;
}

unsigned alg(state & init) {

	auto cmp = [](const state & s1, const state & s2) {return s1.dist > s2.dist; };
	std::priority_queue<state, std::vector<state>, decltype(cmp)> to_visit(cmp);	

	init.calc_dist();

	to_visit.push(init);
	std::vector<state> visited;
	std::vector<unsigned> scores;
	bool found = false;

	while (!to_visit.empty() && !found) {
		const state s = to_visit.top();
		to_visit.pop();
		visited.push_back(s);

		if (finished(s))
		{
			std::cout << "steps: " << s.steps << " visited nodes: " << visited.size() << std::endl;
			scores.push_back(s.steps);
			found = true;
		}
		else {
			std::vector<move> next_moves;
			for (int _d = up; _d < num_dirs; _d++) {
				dir d = (dir)_d;
				if (s.elev == 3 && d == up)
					continue;
				if (s.elev == 0 && d == down)
					continue;
				
				if (s.elev == 1 && d == down && !s.dev_on_floor(0))
					continue;
				else if (s.elev == 2 && d == down && !s.dev_on_floor(0) && !s.dev_on_floor(1))
					continue;
				
				if (d == up && s.dev_on_floor(s.elev) > 1) {
					auto v = comb2(s.dev_on_floor(s.elev));
					for (auto c : v) {
						move m{ (dir)d, std::vector<unsigned>{c.first, c.second} };
						next_moves.push_back(m);
					}
				}				
				
				// onesies
				for (unsigned i = 0; i < s.dev_on_floor(s.elev); ++i) {
					move m{ (dir)d, std::vector<unsigned>{ i } };
					next_moves.push_back(m);
				}									
			}
					
			bool twosee = false;
			for (auto & m : next_moves) {			
				if (twosee && m.second.size() == 1)
					continue;

				state new_state = apply_move(s, m);
				if (valid_state(new_state.d) && !repeated_state(visited, new_state)) {				
					if (m.first == up && m.second.size() == 2)
						twosee = true;
					to_visit.push(new_state);								
				}
			}
		}
	}
	return *std::min_element(scores.begin(), scores.end());
}

template <>
std::string process<11>(std::istream & is, bool part2) {
	std::regex floor_regex("(\\w+) floor");
	std::regex elems_regex("(\\w+)(-compatible micro(c)hip| (g)enerator)");
	std::smatch m;
	state init;
	unsigned elem_count = 0;
	std::map<unsigned, std::pair<unsigned, unsigned>> pairs;

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
			if (dev == microchip)
				pairs[elem].first = floor;
			else
				pairs[elem].second = floor;
		}
	}

	if (part2) {
		pairs[5] = { 0 , 0 };
		pairs[6] = { 0 , 0 };
	}

	for (auto pp : pairs) {
		init.add_elem(pp.second.first, pp.second.second);
	}

	// start at 0	
	init.elev = 0;
	init.steps = 0;	
		
	unsigned min_score = alg(init);
	
	return std::to_string(min_score);
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