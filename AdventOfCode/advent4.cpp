#include <regex>
#include <map>
#include <algorithm>

#include "advent.h"

template<>
std::string process<4>(std::istream & is, bool part2) {

	unsigned sum = 0;
	for (std::string line; std::getline(is, line);) {
		std::regex s("([a-z-]+)-(\\d+)\\[([a-z]+)\\]");
		std::sregex_iterator next(line.begin(), line.end(), s);
		std::smatch m = *next;
		unsigned int code = std::stoi(m.str(2));
		bool match = true;

		if (part2) {
			std::string decipher = m.str(1);
			for (char & c : decipher)
				c = (c == '-') ? ' ' : char(((c - 'a' + code) % 26) + 'a');
			match = decipher.find("north") != std::string::npos;
			return std::to_string(code);
		} else {
			std::vector<std::string> parts;
			std::istringstream is(m.str(1));
			for (std::string p; std::getline(is, p); is.ignore(1, '-'))
				parts.push_back(p);
			typedef std::pair<char, int> kv;
			std::map<char, int> freq;

			for (auto i : parts) {
				for (auto j : i) {
					freq[j]++;
				}
			}

			std::vector<kv> ordered;
			std::transform(freq.begin(), freq.end(),
					std::back_inserter(ordered), [](kv const & p) {return p;});
			std::sort(ordered.begin(), ordered.end(),
					[](kv &left, kv &right) {return left.second > right.second;});

			for (unsigned i = 0; i < m.str(3).length(); ++i) {
				if (m.str(3)[i] != ordered[i].first) {
					match = false;
					break;
				}
			}

			if (match)
				sum += code;
		}
	}
	return std::to_string(sum);
}

template<>
void solve<4>() {
	std::ifstream f("advent4.txt");

	std::cout << "advent4: " << process<4>(std::ifstream("advent4.txt"), false);
	std::cout << "advent4.part2 " << process<4>(std::ifstream("advent4.txt"), true);
}

template<>
void test<4>() {
	assert(process<4>(std::stringstream("aaaaa-bbb-z-y-x-123[abxyz]"), false) == "123");
	assert(process<4>(std::stringstream("a-b-c-d-e-f-g-h-987[abcde]"), false) == "987");
	assert(process<4>(std::stringstream("not-a-real-room-404[oarel]"), false) == "404" );
	assert(process<4>(std::stringstream("totally-real-room-200[decoy]"), false) == "0");
	assert(process<4>(std::stringstream("qzmt-zixmtkozy-ivhz-343[aaaaa]"), false) == "0");
}
