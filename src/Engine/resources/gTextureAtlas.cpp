#include "gTextureAtlas.h"

GTextureAtlas::GTextureAtlas(const char* path):
	GResource(path, ResourceType::TYPE_TEXTURE_ATLAS),
	mWidth(0),
    mHeight(0)
{

}

GTextureAtlas::~GTextureAtlas()
{

}