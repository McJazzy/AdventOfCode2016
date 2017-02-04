/*
 * util.cpp
 *
 *  Created on: Feb 4, 2017
 *      Author: mjasp
 */
#include "util.h"

namespace util {

char * itoa(int val, char *b, int base) {
	char const digit[] = "0123456789abcdef";
	char* p = b;
	int shifter = val;
	do
		++p;
	while (shifter /= base);
	*p = '\0';
	do
		*--p = digit[val % base];
	while (val /= base);
	return b;
}

}

