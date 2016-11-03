#include "gIOFile.h"

bool loadImage(const char* path, unsigned char** data, uint32* dataSize)
{
	FILE* file = fopen(path, "rb");

	size_t fLen = ftell(file);

	*data = new unsigned char[fLen];
	*dataSize = (uint32)fread(data, 1, fLen, file);

	fclose(file);
	return true;
}