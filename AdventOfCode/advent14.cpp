#pragma warning(disable:4996)

#include "advent.h"
#include <assert.h>
#include "md5.h"
#include <istream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

int find_triplet(const std::string & hex) {
	for (int i = 0; i < hex.length() - 2; i++)
		if (hex[i] == hex[i + 1] && hex[i] == hex[i + 2])
			return i+1;
	return 0;
}

int find_quintet(const std::string & hex, char c) {
	for (int i = 0; i < hex.length() - 4; i++)
		if (hex[i] == c && hex[i+1] == c && c == hex[i + 2] && c == hex[i + 3] && c == hex[i + 4])
			return i+1;
	return 0;
}

std::string gen_md5(std::string & s, int i, bool part2) {
	MD5 md5(s + std::to_string(i));
	std::string tmp = md5.hexdigest();
	if (part2) {		
		for (int i = 0; i < 2016; i++)
			tmp = MD5(tmp).hexdigest();
	}
	return tmp;
}

template<>
std::string process<14>(std::istream & is, bool part2) {	
	std::string line; std::getline(is, line);
	std::vector<std::string> memo;
	unsigned sum = 1;
	bool	 triplet_found = false;
	char	 triplet_char;
	unsigned search_index = 0;
	
	while (sum < 64 + 1) {
		for (int i = search_index; i < 50000; ++i) {
			if (memo.size() <= i) {				
				memo.push_back(gen_md5(line, i, part2));
			}

			const std::string & hex = memo[i];

			if (!triplet_found) {
				if (int c = find_triplet(hex)) {
					search_index = i;
					triplet_char = hex[c];
					triplet_found = true;
				}
			}
			else {				
				if (i <= search_index + 1000) {
					if (find_quintet(hex, triplet_char)) {						
						search_index++, sum++, triplet_found = false;						
						break;
					}
				}
				else {
					search_index++, triplet_found = false;
					break;
				}
			}
		}
	}

	return std::to_string(search_index - 1);
}

template <> 
void solve<14>() {
	std::cout << "advent14: " << process<14>(std::istringstream("qzyelonm"), false) << std::endl;
	std::cout << "advent14.part2: " << process<14>(std::istringstream("qzyelonm"), true) << std::endl;
}

template <>
void test<14>() {
	assert(process<14>(std::istringstream("abc"), false) == "22728");
	assert(process<14>(std::istringstream("abc"), true) == "22551");
}