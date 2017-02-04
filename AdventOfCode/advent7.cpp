#include <vector>
#include <set>
#include <algorithm>
#include "advent.h"


bool has_abba(std::string & s) {
	bool abba = false;
	for (unsigned int i = 0; i < s.length() - 3; i++) {
		if (s[i] == s[i + 3] && s[i + 1] == s[i + 2] && s[i] != s[i+1]) {
			abba = true;
			break;
		}		
	}
	return abba;
}

void collect_abas(std::string & s, std::set <std::string> & v) {
	for (unsigned i = 0; i < s.length() - 2; i++) {
		if (s[i] == s[i + 2] && s[i] != s[i + 1])
			v.emplace(s.substr(i, 3));
	}
}

bool aba_match_bab(const std::set<std::string> & babs, const std::set<std::string> & abas) {
	return count_if(babs.begin(), babs.end(), [&abas](const std::string & e) {
		std::string inv = e; inv[0] = e[1]; inv[2] = e[1]; inv[1] = e[0];
		return std::find(abas.begin(), abas.end(), inv) != abas.end();
	}) > 0;	
}

template <>
std::string process<7>(std::istream & is, bool part2) {
	unsigned int sum = 0;
	std::string line;
	while (std::getline(is, line)) {
		std::vector<std::string> res;

		std::istringstream sub(line);
		for (std::string p; std::getline(sub, p); sub.ignore(1, '['), sub.ignore(1,']'))
			res.push_back(p);

		if (!part2) {
			bool regular_abba = false;
			bool hyper_abba = false;
			for (unsigned i = 0; i < res.size(); i++) {
				if (i % 2 == 0 && has_abba(res[i])) regular_abba = true;
				else if (has_abba(res[i])) {
					hyper_abba = true; break;
				}
			}
			if (regular_abba && !hyper_abba)
				sum++;
		}
		else {			
			std::set<std::string> abas;
			std::set<std::string> babs;
			for (unsigned i = 0; i < res.size(); i += 2)
				collect_abas(res[i], abas);
			
			for (unsigned i = 1; i < res.size(); i += 2)
				collect_abas(res[i], babs);
			
			sum += aba_match_bab(abas, babs);						
		}
	}
	return std::to_string(sum);
}

template<>
void test<7>() {
	assert(process<7>(std::istringstream("abba[mnop]qrst"), false) == "1");
	assert(process<7>(std::istringstream("abcd[bddb]xyyx"), false) == "0");
	assert(process<7>(std::istringstream("aaaa[qwer]tyui"), false) == "0");
	assert(process<7>(std::istringstream("ioxxoj[asdfgh]zxcvbn"), false) == "1");
	assert(process<7>(std::istringstream("aaa[zzz]xyz[baaaaaaaaaaaaaaaabab]aaaaaaaaaba"), true) == "1");
	assert(process<7>(std::istringstream("xyx[xyx]xyx"), true) == "0");
	assert(process<7>(std::istringstream("aaa[kek]eke"), true) == "1");
	assert(process<7>(std::istringstream("zazbz[bzb]cdb"), true) == "1");
}

template<>
void solve<7>() {
	std::cout << "advent7: " << process<7>(std::ifstream("advent7.txt"), false) << std::endl;
	std::cout << "advent7.part2: " << process<7>(std::ifstream("advent7.txt"), true) << std::endl;
}
