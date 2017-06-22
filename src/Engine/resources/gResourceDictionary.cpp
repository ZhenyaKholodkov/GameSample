#include "gResourceDictionary.h"
#include "Utils.h"


GResourceDictionary::GResourceDictionary()
	: mTableSize(511)
{
	mTable = new GResource*[mTableSize];
	for (uint32 i = 0; i < mTableSize; i++)
	{
		mTable[i] = nullptr;
	}
}

GResourceDictionary::~GResourceDictionary()
{
	for (uint32 i = 0; i < mTableSize; i++)
	{
		SAFE_DELETE(mTable[i]);
	}
	delete[] mTable;
}
 
GResource* GResourceDictionary::find(const char* key)
{
	uint32 id = getTableID(key);
	GResource* resource = mTable[id];

	while (resource)
	{
		if (strcmp(resource->mKey, key) == 0)
		{
			break;
		}
		resource = resource->mHashNext;
	}
	return resource;
}

void GResourceDictionary::insert(GResource* resource)
{
	resource->mNext = mChain;
	resource->mPrev = nullptr;
	if (mChain)
		mChain->mPrev = resource;
	mChain = resource;

	uint32 id = getTableID(resource->mKey);
	resource->mHashNext = mTable[id];
	mTable[id] = resource;
}

void GResourceDictionary::remove(GResource* resource)
{
	if (resource->mNext)
		resource->mNext->mPrev = resource->mPrev;

	if (resource->mPrev)
		resource->mPrev->mNext = resource->mNext;
	else if (resource == mChain)
		mChain = resource->mNext;

	GResource* curResource = mTable[getTableID(resource->mKey)];
	while (curResource)
	{
		if (curResource == resource)
		{
			curResource = resource->mHashNext;
			break;
		}
		resource = resource->mHashNext;
	}
}

uint32 GResourceDictionary::getTableID(const char* key)
{
	ulong32 hash = get_hash_jbd2(key);
	return (uint32)(hash % mTableSize);
}