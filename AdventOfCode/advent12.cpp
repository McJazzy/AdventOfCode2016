#include "advent.h"
#include <istream>
#include <sstream>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <regex>
#include <map>
#include <array>
#include <functional>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>

struct instr { std::string op; std::string op1; std::string op2; };

template <unsigned N>
std::string process<12>(std::istream & is, bool part2) {
	std::array<int, 4> regs = { 0, 0, part2, 0 };
	std::vector<instr> instrs;	
	unsigned pc = 0;
	std::vector<std::string> results;
	
	for (std::string line; std::getline(is, line); ) {
		boost::algorithm::split(results, line, boost::is_any_of(" "));
		if (results.size() == 2)
			instrs.push_back({ results[0], results[1], "0" });
		else
			instrs.push_back({ results[0], results[1], results[2] });
	}

	auto eval = [&](std::string op) 
	{
		if (isalpha(op.at(0))) return regs[op.at(0) - 'a'];
		else return std::stoi(op);
	};

	std::map<std::string, std::function<void(std::string, std::string)>> apply = {
		{ "cpy", [&](std::string op1, std::string op2) {++pc; regs[op2.at(0) - 'a'] = eval(op1);} },
		{ "inc", [&](std::string op1, std::string op2) {pc++; ++regs[op1.at(0) - 'a']; } },
		{ "dec", [&](std::string op1, std::string op2) {pc++; --regs[op1.at(0) - 'a']; } },
		{ "jnz", [&](std::string op1, std::string op2) {if (eval(op1) != 0) pc += eval(op2); else pc++; }}
	};

	while (pc < instrs.size()) {
		auto instr = instrs.at(pc);
		apply[instr.op](instr.op1, instr.op2);
	}
	return std::to_string(regs[0]);
}

template <>
void solve<12>() {
	std::cout << "advent12: " << process<12>(std::ifstream("advent12.txt"), false) << std::endl;
	std::cout << "advent12.part2: " << process<12>(std::ifstream("advent12.txt"), true) << std::endl;
}

template <>
void test<12>() {
	std::string inp =
		"cpy 41 a\n"
		"inc a\n"
		"inc a\n"
		"dec a\n"
		"jnz a 2\n"
		"dec a";

	assert(process<12>(std::istringstream(inp), false) == "42");
}