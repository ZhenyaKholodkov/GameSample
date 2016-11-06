#ifndef GTEXTUREATLAS_H
#define GTEXTUREATLAS_H

#include "gResource.h"

class GTextureAtlas : GResource
{
	friend class GResManager;
public :
	GTextureAtlas(const char* path);
	~GTextureAtlas();

private:
	int mWidth;
	int mHeight;
};
#endif //GTEXTUREATLAS_H