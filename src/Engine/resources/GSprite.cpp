#include "GSprite.h"

GSprite::GSprite(const char* path):
	GResource(path, ResourceType::TYPE_SPRITE),
	mXPos(0),
    mYPos(0),
    mWidth(0),
    mHeight(0),
    mPivotX(0.0f),
    mPivotY(0.0f)
{

}

GSprite::~GSprite()
{

}

void GSprite::load()
{
	mTextureHandle.Set(mTextureKey);
}