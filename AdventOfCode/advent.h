#pragma once

#include <istream>
#include <assert.h>
#include <string>

template <unsigned N>
std::string process(std::istream & is, bool part2);

template <unsigned N>
std::string process(std::istream && is, bool part2) {
	return process<N>(is, part2);
}

template <unsigned N>
void test();

template <unsigned N>
void solve();
