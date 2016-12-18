#include "advent.h"
#include <string>
#include <assert.h>
#include <sstream>
#include <istream>
#include <iostream>

std::string apply_checksum(std::string & c) {
	std::string n;
	for (auto it = c.begin(); it != c.end(); it += 2) {
		n.push_back('0' + (*it == *(it + 1)));
	}
	return n;
}

std::string process(std::istream & is, unsigned len, bool part2) {		
	std::string in; in.reserve(len * 2);
	is >> in;
	while (in.size() < len) {
		in.push_back('0');
		for (auto it = in.rbegin() + 1; it != in.rend(); ++it) 
			in.push_back(*it ^ 0x1);		
	}
	in.resize(len);
	std::string checksum = in;
	while (checksum.size() % 2 == 0) {
		checksum = apply_checksum(checksum);
	}
	return checksum;
}

template<>
void solve<16>() {
	std::cout << "advent16: " << process(std::stringstream("01111001100111011"), 272, false) << std::endl;
	std::cout << "advent16.part2 "  << process(std::stringstream("01111001100111011"), 35651584, true) << std::endl;
}

template<>
void test<16>() {
	assert(process(std::stringstream("10000"), 20, false) == "01100");
	
}