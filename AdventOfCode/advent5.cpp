#include <string>
#include <assert.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <thread>
#include <string>
#include <cstdlib>

#include "md5.h"
namespace adv5 {
	char buffer[64];

	std::string process(std::string id, bool part2 = false) {		
		MD5 m;
		unsigned int val = 0;
		strcpy(buffer, id.c_str());
		unsigned int len = strlen(id.c_str());		
		std::string result("________");	
		for (unsigned int i = 0; val <8; i++) {
			itoa(i, buffer + len, 10);
			m.init();
			m.update(buffer, strlen(buffer));
			m.finalize();
			if (m.digest_starts_5zeroes()) {
				if (!part2) {										
					result[val] = m.hexdigest()[5];
					std::cout << i << " " << m.hexdigest() << " pw: " << result << std::endl;
					val++;
				}
				else {
					if (isdigit(m.hexdigest()[5])) {
						char dig = m.hexdigest()[5];
						int pos = (int)(dig - '0');
						if (pos < 8 && result[pos] == '_') {									
							std::cout << i << " " << m.hexdigest() << " pw: " << result << std::endl;
							result[pos] = (char)m.hexdigest()[6];							
							val++;
						}
					}
				}
			}
		}
		return std::string(std::begin(result), std::end(result));
	}
}

void advent5() {
	std::cout << "advent5: " << adv5::process("ojvtpuvg") << std::endl;
	std::cout << "advent5.part2: " << adv5::process("ojvtpuvg", true) << std::endl;
}

void test_advent5() {
	std::cout << adv5::process("abc") << std::endl;
	std::cout << adv5::process("abc", true) << std::endl; 	
}
