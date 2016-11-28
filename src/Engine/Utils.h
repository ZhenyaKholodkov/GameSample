#ifndef UTILS_H
#define UTILS_H

#include "Types.h"


inline unsigned int rgbaToInt(unsigned int aR, unsigned int aG, unsigned int aB, unsigned int aA = 255)
{
	return (aR << 16) | (aG << 8) | (aB) | (aA << 24);
}

int randInt(int minVal, int maxVal);

ulong32 get_hash_jbd2(const char* str);

#endif // !UTILS_H
