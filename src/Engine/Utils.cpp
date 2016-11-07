
#include "Utils.h"

ulong32 get_hash_jbd2(const char* str)
{
	ulong32 hash = 5381;
	int c;

	while (c = *str++) {
		hash = ((hash << 5) + hash) + c;  /* hash * 33 + c */
	}
	return hash;
}