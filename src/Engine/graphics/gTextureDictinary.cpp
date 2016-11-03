#include "gTextureDictinary.h"


GTextureDictinary* GTextureDictinary::Instance()
{
	static GTextureDictinary* instance = new GTextureDictinary();
	return instance;
}

GTextureDictinary::GTextureDictinary()
	: mTableSize(1000)
{
	mTable = new GTextureObject*[mTableSize];
	for (uint32 i = 0; i < mTableSize; i++)
	{
		mTable[i] = nullptr;
	}
}

GTextureDictinary::~GTextureDictinary()
{
	destroy();
}

GTextureObject* GTextureDictinary::find(const char* key)
{
	GTextureObject* texture = mTable[getTableID(key)];
	
	while (texture)
	{
		if (texture->mKey == key)
		{
			break;
		}
		texture = texture->mSameHash;
	}
	return texture;
}

void GTextureDictinary::insert(GTextureObject* texture)
{
	texture->mNext = mChain;
	texture->mPrev = nullptr;
	if (mChain)
		mChain->mPrev = texture;
	mChain = texture;

	uint32 id = getTableID(texture->mKey);
	texture->mSameHash = mTable[id];
	mTable[id] = texture;
}

void GTextureDictinary::remove(GTextureObject* texture)
{
	if (texture->mNext)
		texture->mNext->mPrev = texture->mPrev;

	if (texture->mPrev)
		texture->mPrev->mNext = texture->mNext;
	else if (texture == mChain)
		mChain = texture->mNext;

	GTextureObject* curTexture = mTable[getTableID(texture->mKey)];
	while (curTexture)
	{
		if (curTexture == texture)
		{
			curTexture = texture->mSameHash;
			break;
		}
		texture = texture->mSameHash;
	}
}

uint32 GTextureDictinary::getTableID(const char* key)
{
	uint32 hash = (uint32)(((size_t)key) >> 2);
	return hash % mTableSize;
}

void GTextureDictinary::destroy()
{

}