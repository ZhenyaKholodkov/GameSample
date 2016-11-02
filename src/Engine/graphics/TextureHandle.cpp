#include "TextureHandle.h"

TextureHandle::TextureHandle():
	mTexture(nullptr)
{
	
}

TextureHandle::TextureHandle(const char* texturePath)
{

}

TextureHandle::TextureHandle(const TextureHandle& handle)
{

}

TextureHandle::~TextureHandle()
{

}

void TextureHandle::lock()
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