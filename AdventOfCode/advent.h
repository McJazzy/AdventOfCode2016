#ifndef __ADVENT_H
#define __ADVENT_H

#include <istream>
#include <assert.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstdlib>

template<unsigned N>
std::string process(std::istream & is, bool part2);

template<unsigned N>
std::string process(std::istream && is, bool part2) {
	return process<N>(is, part2);
}

template<unsigned N>
void test();

template<unsigned N>
void solve();

// fix for __MINGW32__
#ifdef __MINGW32__
#  include <sstream>
namespace std{
inline int stoi(const std::string& s) {
    return atoi(s.c_str());
}
template<class T>
inline std::string to_string(const T& x) {
    std::ostringstream stm;
    stm << x;
    return stm.str();
}
}
#endif // __MINGW32__

#endif //__ADVENT_H
