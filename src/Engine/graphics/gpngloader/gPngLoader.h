#ifndef GPNGLOADER_H
#define GPNGLOADER
#include "png.h"

struct	png_image_raw_data
{
	FILE*	file;
	size_t	size;
	int		offset;
	png_image_raw_data() : file(NULL), size(0), offset(0) {};
};

bool LoadPngImage(const char* aFileName, unsigned char** aImage, unsigned long* aWidth, unsigned long* aHeight);

#endif //GPNGLOADER