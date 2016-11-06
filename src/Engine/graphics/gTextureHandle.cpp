#include "gTextureHandle.h"

GTextureHandle::GTextureHandle():
	mTexture(nullptr)
{
	
}

GTextureHandle::GTextureHandle(const char* texturePath)
{
	mTexture = GTextureManager::Instance()->LoadTexture(texturePath);
	lock();
}

GTextureHandle::GTextureHandle(const GTextureHandle& handle)
{

}

void GTextureHandle::Set(const char* texturePath)
{
	if (mTexture != nullptr) 
	{
		unlock();
	}

	mTexture = GTextureManager::Instance()->LoadTexture(texturePath);
	lock();
}

GTextureHandle::~GTextureHandle()
{
	unlock();
}

void GTextureHandle::lock()
{
	if (mTexture == nullptr)
		return;

	mTexture->mRefCounter++;
}

void GTextureHandle::unlock()
{
	if (IsNull())
		return;

	if (--mTexture->mRefCounter == 0)
	{
		// free
		GTextureManager::Instance()->unloadTexture(mTexture);
	}

	mTexture = nullptr;
}