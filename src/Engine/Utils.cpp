
#include "Utils.h"


bool sgInitTable = true;
uint8   sgHashTable[256];

void initTolowerTable()
{
	for (uint32 i = 0; i < 256; i++) 
	{
		uint32 c = dTolower(i);
		sgHashTable[i] = c * c;
	}

	sgInitTable = false;
}

uint32 getHashString(const char* str)
{
	if (sgInitTable)
		initTolowerTable();

	uint32 ret = 0;
	char c;
	while ((c = *str++) != 0) 
	{
		ret <<= 1;
		ret ^= sgHashTable[c];
	}
	return ret;
}