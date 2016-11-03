#include "gTextureHandle.h"

GTextureHandle::GTextureHandle():
	mTexture(nullptr)
{
	
}

GTextureHandle::GTextureHandle(const char* texturePath)
{

}

GTextureHandle::GTextureHandle(const GTextureHandle& handle)
{

}

GTextureHandle::~GTextureHandle()
{

}

void GTextureHandle::lock()
{
	if (mTexture == nullptr)
		return;

	mTexture->mRefCounter++;
}

void TextureHandle::unlock()
{
	if (IsNull())
		return;

	if (--mTexture->mRefCounter == 0)
	{
		// free
	}

	mTexture = nullptr;
}