#include <istream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include <assert.h>
#include <iostream>
#include <set>

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
			v.insert(s.substr(i, 3));
	}
}

bool aba_match_bab(const std::set<std::string> & babs, const std::set<std::string> & abas) {
	return count_if(babs.begin(), babs.end(), [&abas](const std::string & e) {
		std::string inv = e; inv[0] = e[1]; inv[2] = e[1]; inv[1] = e[0];
		return std::find(abas.begin(), abas.end(), inv) != abas.end();
	}) > 0;	
}

unsigned int process(std::istream & is, bool part2 = false) {
	unsigned int sum = 0;
	std::string line;
	while (std::getline(is, line)) {
		std::vector<std::string> res;
		boost::split(res, line, boost::is_any_of("[]"));
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
	return sum;
}

void test_advent7() {	
	assert(process(std::istringstream("abba[mnop]qrst")) == 1);
	assert(process(std::istringstream("abcd[bddb]xyyx")) == 0);
	assert(process(std::istringstream("aaaa[qwer]tyui")) == 0);
	assert(process(std::istringstream("ioxxoj[asdfgh]zxcvbn")) == 1);

	assert(process(std::istringstream("aaa[zzz]xyz[baaaaaaaaaaaaaaaabab]aaaaaaaaaba"), true) == 1);
	assert(process(std::istringstream("xyx[xyx]xyx"), true) == 0);
	assert(process(std::istringstream("aaa[kek]eke"), true) == 1);
	assert(process(std::istringstream("zazbz[bzb]cdb"), true) == 1);
}

void advent7() {		
	std::cout << "advent7: " << process(std::ifstream("advent7.txt")) << std::endl;
	std::cout << "advent7.part2: " << process(std::ifstream("advent7.txt"), true) << std::endl;
}