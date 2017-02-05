#include "advent.h"
#include <regex>
#include <array>
#include <sstream>
#include <assert.h>
#include <iostream>
#include <fstream>

enum {l, h, options};
enum {num_bots=210};

struct bot {	
	int    value_target[2], value[2];
	bool		value_set[2], output[2];	

	void		set_value(int v) {
		int cmin = value[l];
		value[l] = value_set[l] ? std::min({ v,cmin }) : v;
		value[h] = std::max({ v,cmin });

		value_set[h] = value_set[l];
		value_set[l] = true;
	}

	int	release(bool high) {
		value_set[high] = false;
		return value[high];
	}
};

bool give(int bot_id, int cmp1, int cmp2, std::vector<bot> & bots, std::vector<int> & outputs) {
	bool ret = false;

	bot & b = bots[bot_id];
	if (b.value_set[h] && b.value_set[l]) {		
		int high = b.release(h);
		int low = b.release(l);

		ret = (high == cmp1 && low == cmp2);
						
		for (int i = 0; i < options; i++) {
			auto val = i == l ? low : high;
			if (!b.output[i])
				bots[b.value_target[i]].set_value(val);
			else
				outputs[b.value_target[i]] = val;
		}

		std::cout << "bot " << bot_id << " gives low=" << low << " to " << b.value_target[l] << " and high=" << high << " to " << b.value_target[h] << std::endl;
	}
	return ret;
}

std::string process(std::istream && is, int val1, int val2, bool part2) {
	std::smatch m;
	std::regex v("(v)alue (\\d+) goes to bot (\\d+)"),
		g("(b)ot (\\d+) gives low to (bot|output) (\\d+) and high to (bot|output) (\\d+)");
	std::vector<bot> bots(num_bots);
	std::vector<int> outputs(num_bots);
	
	for (std::string line; std::getline(is, line); ) {
		if (std::regex_search(line, m, v) || std::regex_search(line, m, g)) {
			if (m.str(1) == "v") {			
				bots[std::stoi(m.str(3))].set_value(std::stoi(m.str(2)));
			}
			else if (m.str(1) == "b") {			
				int bot_id = std::stoi(m.str(2));
				bots[bot_id].value_target[h] = std::stoi(m.str(6)); 
				bots[bot_id].value_target[l] = std::stoi(m.str(4));
				bots[bot_id].output[h] = m.str(5) == "output";
				bots[bot_id].output[l] = m.str(3) == "output";
			}
		}
	}
	
	bool comb_seen = false;
	int output_bot = num_bots;
	for (int iter = 0; iter < num_bots; iter++) {
		for (int i = 0; i < bots.size(); i++) {
			comb_seen = give(i, val1, val2, bots, outputs);
			if (comb_seen) {
				output_bot = i;				
			}
		}
	}

	if (part2) return std::to_string(outputs[0] * outputs[1] * outputs[2]);

	return std::to_string(output_bot);
}

template<>
void test<10>() {
	std::string input("value 5 goes to bot 2\n"
		"bot 2 gives low to bot 1 and high to bot 0\n"
		"value 3 goes to bot 1\n"
		"bot 1 gives low to output 1 and high to bot 0\n"
		"bot 0 gives low to output 2 and high to output 0\n"
		"value 2 goes to bot 2");	
	
	assert(process(std::istringstream(input), 5, 2, false) == "2");
	assert(process(std::istringstream(input), 5, 2, true) == "30");
}

template <>
void solve<10>() {
	std::cout << "advent10: " << process(std::ifstream("advent10.txt"), 61, 17, false) << std::endl;
	std::cout << "advent10.part2: " << process(std::ifstream("advent10.txt"), 61, 17, true) << std::endl;
}
