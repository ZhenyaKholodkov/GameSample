#include "GSprite.h"

GSprite::GSprite(const char* path, std::shared_ptr<GTextureManager> manager):
	mTextureHandle(manager),
	GResource(path, ResourceType::TYPE_SPRITE),
	mIsLoaded(false)
{

}

GSprite::~GSprite()
{

}

void GSprite::load()
{
	mTextureHandle.Set(mTextureKey);
	mIsLoaded = true;
}