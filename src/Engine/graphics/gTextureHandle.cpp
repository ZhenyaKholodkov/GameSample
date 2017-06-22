#include "gTextureHandle.h"
#include "gResManager.h"

GTextureHandle::GTextureHandle(std::shared_ptr<GTextureManager> manager):
	mTexture(nullptr),
	mTextureManager(manager)
{
	
}

GTextureHandle::GTextureHandle(const char* texturePath)
{
	mTexture = GResManager::Instance()->mTextureManager->LoadTexture(texturePath);
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

	mTexture = mTextureManager->LoadTexture(texturePath);
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
		if(mTextureManager)
			mTextureManager->unloadTexture(mTexture);
	}

	mTexture = nullptr;
}