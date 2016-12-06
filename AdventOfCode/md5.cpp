/* MD5
converted to C++ class by Frank Thilo (thilo@unix-ag.org)
for bzflag (http://www.bzflag.org)

based on:

md5.h and md5.c
reference implemantion of RFC 1321

Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.

License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.

License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.

*/

#pragma warning(disable:4996)
/* interface header */
#include "md5.h"

/* system implementation headers */
#include <cstdio>


// Constants for MD5Transform routine.
#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

///////////////////////////////////////////////

// F, G, H and I are basic MD5 functions.
inline MD5::uint4 MD5::F(uint4 x, uint4 y, uint4 z) {
	return x&y | ~x&z;
}

inline MD5::uint4 MD5::G(uint4 x, uint4 y, uint4 z) {
	return x&z | y&~z;
}

inline MD5::uint4 MD5::H(uint4 x, uint4 y, uint4 z) {
	return x^y^z;
}

inline MD5::uint4 MD5::I(uint4 x, uint4 y, uint4 z) {
	return y ^ (x | ~z);
}

// rotate_left rotates x left n bits.
inline MD5::uint4 MD5::rotate_left(uint4 x, int n) {
	return (x << n) | (x >> (32 - n));
}

// FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
// Rotation is separate from addition to prevent recomputation.
inline void MD5::FF(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac) {
	a = rotate_left(a + F(b, c, d) + x + ac, s) + b;
}

inline void MD5::GG(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac) {
	a = rotate_left(a + G(b, c, d) + x + ac, s) + b;
}

inline void MD5::HH(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac) {
	a = rotate_left(a + H(b, c, d) + x + ac, s) + b;
}

inline void MD5::II(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac) {
	a = rotate_left(a + I(b, c, d) + x + ac, s) + b;
}

//////////////////////////////////////////////

// default ctor, just initailize
MD5::MD5()
{
	init();
}

//////////////////////////////////////////////

// nifty shortcut ctor, compute MD5 for string and finalize it right away
MD5::MD5(const std::string &text)
{
	init();
	update(text.c_str(), text.length());
	finalize();
}

//////////////////////////////

void MD5::init()
{
	finalized = false;

	count[0] = 0;
	count[1] = 0;

	// load magic initialization constants.
	state[0] = 0x67452301;
	state[1] = 0xefcdab89;
	state[2] = 0x98badcfe;
	state[3] = 0x10325476;
}

//////////////////////////////

// decodes input (unsigned char) into output (uint4). Assumes len is a multiple of 4.
void MD5::decode(uint4 output[], const uint1 input[], size_type len)
{
	for (unsigned int i = 0, j = 0; j < len; i++, j += 4)
		output[i] = ((uint4)input[j]) | (((uint4)input[j + 1]) << 8) |
		(((uint4)input[j + 2]) << 16) | (((uint4)input[j + 3]) << 24);
}

//////////////////////////////

// encodes input (uint4) into output (unsigned char). Assumes len is
// a multiple of 4.
void MD5::encode(uint1 output[], const uint4 input[], size_type len)
{
	for (size_type i = 0, j = 0; j < len; i++, j += 4) {
		output[j] = input[i] & 0xff;
		output[j + 1] = (input[i] >> 8) & 0xff;
		output[j + 2] = (input[i] >> 16) & 0xff;
		output[j + 3] = (input[i] >> 24) & 0xff;
	}
}

//////////////////////////////

// apply MD5 algo on a block
void MD5::transform(const uint1 block[blocksize])
{
#define LOADSCHEDULE(i)  \
		schedule[i] = (uint32_t)block[i * 4 + 0] <<  0  \
		            | (uint32_t)block[i * 4 + 1] <<  8  \
		            | (uint32_t)block[i * 4 + 2] << 16  \
		            | (uint32_t)block[i * 4 + 3] << 24;



	//uint4 a = state[0], b = state[1], c = state[2], d = state[3], x[16];
	uint32_t schedule[16];
	LOADSCHEDULE(0)
		LOADSCHEDULE(1)
		LOADSCHEDULE(2)
		LOADSCHEDULE(3)
		LOADSCHEDULE(4)
		LOADSCHEDULE(5)
		LOADSCHEDULE(6)
		LOADSCHEDULE(7)
		LOADSCHEDULE(8)
		LOADSCHEDULE(9)
		LOADSCHEDULE(10)
		LOADSCHEDULE(11)
		LOADSCHEDULE(12)
		LOADSCHEDULE(13)
		LOADSCHEDULE(14)
		LOADSCHEDULE(15)

#define ROTL32(x, n)  (((0U + (x)) << (n)) | ((x) >> (32 - (n))))  // Assumes that x is uint32_t and 0 < n < 32
#define ROUND0(a, b, c, d, k, s, t)  ROUND_TAIL(a, b, d ^ (b & (c ^ d)), k, s, t)
#define ROUND1(a, b, c, d, k, s, t)  ROUND_TAIL(a, b, c ^ (d & (b ^ c)), k, s, t)
#define ROUND2(a, b, c, d, k, s, t)  ROUND_TAIL(a, b, b ^ c ^ d        , k, s, t)
#define ROUND3(a, b, c, d, k, s, t)  ROUND_TAIL(a, b, c ^ (b | ~d)     , k, s, t)
#define ROUND_TAIL(a, b, expr, k, s, t)    \
		a = 0U + a + (expr) + UINT32_C(t) + schedule[k];  \
		a = 0U + b + ROTL32(a, s);

		uint32_t a = state[0];
	uint32_t b = state[1];
	uint32_t c = state[2];
	uint32_t d = state[3];

	ROUND0(a, b, c, d, 0, 7, 0xD76AA478)
		ROUND0(d, a, b, c, 1, 12, 0xE8C7B756)
		ROUND0(c, d, a, b, 2, 17, 0x242070DB)
		ROUND0(b, c, d, a, 3, 22, 0xC1BDCEEE)
		ROUND0(a, b, c, d, 4, 7, 0xF57C0FAF)
		ROUND0(d, a, b, c, 5, 12, 0x4787C62A)
		ROUND0(c, d, a, b, 6, 17, 0xA8304613)
		ROUND0(b, c, d, a, 7, 22, 0xFD469501)
		ROUND0(a, b, c, d, 8, 7, 0x698098D8)
		ROUND0(d, a, b, c, 9, 12, 0x8B44F7AF)
		ROUND0(c, d, a, b, 10, 17, 0xFFFF5BB1)
		ROUND0(b, c, d, a, 11, 22, 0x895CD7BE)
		ROUND0(a, b, c, d, 12, 7, 0x6B901122)
		ROUND0(d, a, b, c, 13, 12, 0xFD987193)
		ROUND0(c, d, a, b, 14, 17, 0xA679438E)
		ROUND0(b, c, d, a, 15, 22, 0x49B40821)
		ROUND1(a, b, c, d, 1, 5, 0xF61E2562)
		ROUND1(d, a, b, c, 6, 9, 0xC040B340)
		ROUND1(c, d, a, b, 11, 14, 0x265E5A51)
		ROUND1(b, c, d, a, 0, 20, 0xE9B6C7AA)
		ROUND1(a, b, c, d, 5, 5, 0xD62F105D)
		ROUND1(d, a, b, c, 10, 9, 0x02441453)
		ROUND1(c, d, a, b, 15, 14, 0xD8A1E681)
		ROUND1(b, c, d, a, 4, 20, 0xE7D3FBC8)
		ROUND1(a, b, c, d, 9, 5, 0x21E1CDE6)
		ROUND1(d, a, b, c, 14, 9, 0xC33707D6)
		ROUND1(c, d, a, b, 3, 14, 0xF4D50D87)
		ROUND1(b, c, d, a, 8, 20, 0x455A14ED)
		ROUND1(a, b, c, d, 13, 5, 0xA9E3E905)
		ROUND1(d, a, b, c, 2, 9, 0xFCEFA3F8)
		ROUND1(c, d, a, b, 7, 14, 0x676F02D9)
		ROUND1(b, c, d, a, 12, 20, 0x8D2A4C8A)
		ROUND2(a, b, c, d, 5, 4, 0xFFFA3942)
		ROUND2(d, a, b, c, 8, 11, 0x8771F681)
		ROUND2(c, d, a, b, 11, 16, 0x6D9D6122)
		ROUND2(b, c, d, a, 14, 23, 0xFDE5380C)
		ROUND2(a, b, c, d, 1, 4, 0xA4BEEA44)
		ROUND2(d, a, b, c, 4, 11, 0x4BDECFA9)
		ROUND2(c, d, a, b, 7, 16, 0xF6BB4B60)
		ROUND2(b, c, d, a, 10, 23, 0xBEBFBC70)
		ROUND2(a, b, c, d, 13, 4, 0x289B7EC6)
		ROUND2(d, a, b, c, 0, 11, 0xEAA127FA)
		ROUND2(c, d, a, b, 3, 16, 0xD4EF3085)
		ROUND2(b, c, d, a, 6, 23, 0x04881D05)
		ROUND2(a, b, c, d, 9, 4, 0xD9D4D039)
		ROUND2(d, a, b, c, 12, 11, 0xE6DB99E5)
		ROUND2(c, d, a, b, 15, 16, 0x1FA27CF8)
		ROUND2(b, c, d, a, 2, 23, 0xC4AC5665)
		ROUND3(a, b, c, d, 0, 6, 0xF4292244)
		ROUND3(d, a, b, c, 7, 10, 0x432AFF97)
		ROUND3(c, d, a, b, 14, 15, 0xAB9423A7)
		ROUND3(b, c, d, a, 5, 21, 0xFC93A039)
		ROUND3(a, b, c, d, 12, 6, 0x655B59C3)
		ROUND3(d, a, b, c, 3, 10, 0x8F0CCC92)
		ROUND3(c, d, a, b, 10, 15, 0xFFEFF47D)
		ROUND3(b, c, d, a, 1, 21, 0x85845DD1)
		ROUND3(a, b, c, d, 8, 6, 0x6FA87E4F)
		ROUND3(d, a, b, c, 15, 10, 0xFE2CE6E0)
		ROUND3(c, d, a, b, 6, 15, 0xA3014314)
		ROUND3(b, c, d, a, 13, 21, 0x4E0811A1)
		ROUND3(a, b, c, d, 4, 6, 0xF7537E82)
		ROUND3(d, a, b, c, 11, 10, 0xBD3AF235)
		ROUND3(c, d, a, b, 2, 15, 0x2AD7D2BB)
		ROUND3(b, c, d, a, 9, 21, 0xEB86D391)

		state[0] = 0U + state[0] + a;
	state[1] = 0U + state[1] + b;
	state[2] = 0U + state[2] + c;
	state[3] = 0U + state[3] + d;

	// Zeroize sensitive information.
	// memset(x, 0, sizeof x);
}

//////////////////////////////

// MD5 block update operation. Continues an MD5 message-digest
// operation, processing another message block
void MD5::update(const unsigned char input[], size_type length)
{
	// compute number of bytes mod 64
	size_type index = count[0] / 8 % blocksize;

	// Update number of bits
	if ((count[0] += (length << 3)) < (length << 3))
		count[1]++;
	count[1] += (length >> 29);

	// number of bytes we need to fill in buffer
	size_type firstpart = 64 - index;

	size_type i;

	// transform as many times as possible.
	if (length >= firstpart)
	{
		// fill buffer first, transform
		memcpy(&buffer[index], input, firstpart);
		transform(buffer);

		// transform chunks of blocksize (64 bytes)
		for (i = firstpart; i + blocksize <= length; i += blocksize)
			transform(&input[i]);

		index = 0;
	}
	else
		i = 0;

	// buffer remaining input
	memcpy(&buffer[index], &input[i], length - i);
}

//////////////////////////////

// for convenience provide a verson with signed char
void MD5::update(const char input[], size_type length)
{
	update((const unsigned char*)input, length);
}

//////////////////////////////

// MD5 finalization. Ends an MD5 message-digest operation, writing the
// the message digest and zeroizing the context.
MD5& MD5::finalize()
{
	static unsigned char padding[64] = {
		0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};

	if (!finalized) {
		// Save number of bits
		unsigned char bits[8];
		encode(bits, count, 8);

		// pad out to 56 mod 64.
		size_type index = count[0] / 8 % 64;
		size_type padLen = (index < 56) ? (56 - index) : (120 - index);
		update(padding, padLen);

		// Append length (before padding)
		update(bits, 8);

		// Store state in digest
		encode(digest, state, 16);

		// Zeroize sensitive information.
	//	memset(buffer, 0, sizeof buffer);
	//	memset(count, 0, sizeof count);

		finalized = true;
	}

	return *this;
}

//////////////////////////////

// return hex representation of digest as string
std::string MD5::hexdigest() const
{
	if (!finalized)
		return "";

	char buf[33];
	for (int i = 0; i<16; i++)
		sprintf(buf + i * 2, "%02x", digest[i]);
	buf[32] = 0;

	return std::string(buf);
}

bool MD5::digest_starts_5zeroes()
{
	return (digest[0] == 0 && digest[1] == 0 && ((digest[2] >> 4) == 0));
}

//////////////////////////////

std::ostream& operator<<(std::ostream& out, MD5 md5)
{
	return out << md5.hexdigest();
}

//////////////////////////////

std::string md5(const std::string str)
{
	MD5 md5 = MD5(str);

	return md5.hexdigest();
}