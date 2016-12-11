
#include "Utils.h"

#include <stdarg.h>

// get hash of specified string
ulong32 get_hash_jbd2(const char* str)
{
	ulong32 hash = 5381;
	int c;

	while (c = *str++) {
		hash = ((hash << 5) + hash) + c;  /* hash * 33 + c */
	}
	return hash;
}

int randInt(int minVal, int maxVal)
{
	int diff = (maxVal - minVal + 1);
	if (!diff) return 0;
	return (minVal + (rand() % diff));
}

FILE* GLog::mFile = 0;

void GLog::printLog(const char* msg)
{
	if (mFile == 0)
	{
		mFile = fopen("logs.log", "w");
	}
	printf("%s", msg);
	fwrite(msg, 1, strlen(msg), mFile);
}

void GLog::finish()
{
	if (mFile != 0)
	{
		fclose(mFile);
		mFile = 0;
	}
}