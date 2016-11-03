#include "GResManager.h"
#include "Utils.h"
#include "Engine\OpenGL\IGRender.h"

#include <stdio.h>
#include <stdarg.h>

//////////////!!!ResDictionary - realiztion!!!///////////
ResDictionary::ResDictionary():
	mEntryCount(0),
    mTableSize(DefaultTableSize)
{
	mTable = new GResFile *[mTableSize];
	int32 i;
	for (i = 0; i < mTableSize; i++)
		mTable[i] = NULL;
}

ResDictionary::~ResDictionary()
{
	delete[] mTable;
}

void ResDictionary::insert(GResFile* resFile)
{

}
int32 ResDictionary::hash(const char* path, const char* file)
{
	return ((uint32)((((size_t)path) >> 2) + (((size_t)file) >> 2))) % mTableSize;
}

GResFile* ResDictionary::find(const char* name, const char* path)
{
	return NULL;
}
//////////////!!!ResDictionary - end!!!///////////

///////////// !!!Texture !!//////////////////
Texture::Texture(): mTextureId(-1), mWeight(0), mHeight(0),
mData(NULL), mDataSize(0)
{
}

Texture::~Texture()
{
	if (mData)
	{
		delete mData;
	}
}

//////////////!!!GResManager - realiztion!!!///////////
GResManager::GResManager() : mMaxFileId(0)
{
	mResources.resize(DefaultResourceSize);
}

GResManager::~GResManager()
{
	mResources.clear();
}

GResManager* GResManager::Instance()
{
	static GResManager instance;
	return &instance;
}

void GResManager::Add(GResFile* resFile)
{
	bool foundEmptySlot = false;
	for (int i = mMaxFileId; i < mResources.size(); i++)
	{
		if (!mResources[i])
		{
			foundEmptySlot = true;
			mResources[i] = resFile;
			resFile->mFileId = i;
			break;
		}
	}

	if (!foundEmptySlot)
	{
		mResources.push_back(resFile);
		resFile->mFileId = (mResources.size() - 1);
	}
}

void GResManager::setResDirectory(const char* dir)
{
	mResDir = dir; 
}

GResFile* GResManager::CreateRes(const char* name)
{
	return NULL;
}

bool GResManager::LoadResource(int id)
{
	GResFile* resource = mResources[id];
	std::string path = resource->mPath + "/" + resource->mName;
	if (resource->mType == RES_FILE_TEXTURE)
	{
		Texture* texture = reinterpret_cast<Texture*>(resource);
		LoadPngImage(path.c_str(), &texture->mData, &texture->mWeight, &texture->mHeight);
		LoadTextureToVRAM(id);
	}
	return true;
}

bool GResManager::LoadTextureToVRAM(int id)
{
	GResFile* resource = mResources[id];
	Texture* texture = reinterpret_cast<Texture*>(resource);
	GThreadSafeErrors err;
	texture->mTextureId = IGRender::Instance()->createTexture(IGRender::TPT_ABGR_8888,
		texture->mData, texture->mWeight, texture->mHeight, id, err);
	return true;
}

bool GResManager::LoadImage(const char* path, unsigned char** data, uint32* dataSize)
{
	FILE* file = fopen(path, "rb");

	size_t fLen = ftell(file);

	*data = new unsigned char[fLen];
	*dataSize = (uint32)fread(data, 1, fLen, file);

	fclose(file);
	return true;
}

//////////////!!!GResManager - end!!!///////////