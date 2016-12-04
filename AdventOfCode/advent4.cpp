#include <string>
#include <regex>
#include <iostream>
#include <map>
#include <assert.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>

std::pair<bool,unsigned int> process(std::string input, bool part2 = false) {
	std::regex s("([a-z-]+)-(\\d+)\\[([a-z]+)\\]");
	std::sregex_iterator next(input.begin(), input.end(), s);
	std::smatch m = *next;
	unsigned int code = boost::lexical_cast<unsigned int>(m.str(2));	
	bool match = true;

	if (part2) {				
		std::string decipher = m.str(1);
		std::transform(decipher.begin(), decipher.end(),decipher.begin(),
			[code](char c) {if (c == '-') return ' '; else return char(((c - 'a' + code) % 26) + 'a'); });
		match = decipher.find("north") != std::string::npos;		
	}
	else {
		std::vector<std::string> parts;
		boost::split(parts, m.str(1), boost::is_any_of("-"));
		typedef std::pair<char, int> kv;
		std::map<char, int> freq;

		for (auto i : parts) {for (auto j : i) {
				freq[j]++;
		}}

		std::vector<kv> ordered;
		std::transform(freq.begin(), freq.end(), std::back_inserter(ordered), [](kv const & p) {return p; });
		std::sort(ordered.begin(), ordered.end(), [](kv &left, kv &right) {return left.second > right.second; });

		for (int i = 0; i < m.str(3).length(); ++i) {
			if (m.str(3)[i] != ordered[i].first) {
				match = false; break;
			}
		}		
	}
	return std::make_pair(match, code);
}

void advent4() {
	std::ifstream f("advent4.txt");
	std::string line;
	int sum = 0;
	while (std::getline(f, line)) {
		bool match;
		int code;
		std::tie(match, code) = process(line);
		if (match) sum += code;

		std::tie(match, code) = process(line, true);
		if (match)
			std::cout << "advent4.part2: " << code << std::endl;
	}

	std::cout << "advent4: " << sum << std::endl;
}

void test_advent4() {
	assert(process("aaaaa-bbb-z-y-x-123[abxyz]").first == true);
	assert(process("a-b-c-d-e-f-g-h-987[abcde]").first == true);
	assert(process("not-a-real-room-404[oarel]").first == true);
	assert(process("totally-real-room-200[decoy]").first == false);
	assert(process("qzmt-zixmtkozy-ivhz-343[aaaaa]", true).second == 343);
}