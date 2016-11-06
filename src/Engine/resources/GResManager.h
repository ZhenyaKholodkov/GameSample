#ifndef _GRES_MANAGER_H
#define _GRES_MANAGER_H

#include "Types.h"

#include <vector>
#include "gResource.h"
#include "GSprite.h"
#include "gTextureAtlas.h"
#include "gResourceDictionary.h"

using namespace std;

enum ResType
{
	RES_FILE_TEXTURE = BIT(0)
};

class GResFile
{
public:

	GResFile() : mFileId(-1), mType(RES_FILE_TEXTURE){}
	~GResFile() {}
protected:
	int         mFileId;
	int         mType;
	std::string  mPath;
	std::string  mName;

	friend class GResManager;
	friend class GGame;
};

class Texture : public GResFile
{
public:
	Texture();
	~Texture();
protected:
	int     mTextureId;

	ulong32 mTextureWeight;
	ulong32 mTextureHeight;

	ulong32 mWeight;
	ulong32 mHeight;

	unsigned char* mData;
	uint32         mDataSize;
	friend class GResManager;
	friend class GGame;
};

class ResDictionary
{
public:
	ResDictionary();
	~ResDictionary();
	void insert(GResFile* resFile);
//	void remove(GResFile* resFile);

	int32 hash(const char* path, const char* file);
	GResFile* find(const char* name, const char* path);
private:
	enum { DefaultTableSize = 1000 };
	GResFile** mTable;
	uint32     mEntryCount;
	uint32     mTableSize;
};

class GResManager
{
public:

	enum { DefaultResourceSize = 100 };

	static GResManager* Instance();

	void  Add(GResFile* resFile);
	bool LoadResource(int id);
	bool LoadResources(const char* pathToConfig);
	GSprite* GetSprite(const char* key);

	void setResDirectory(const char* dir);

	std::vector<GResFile*> mResources;
private:
	GResManager();
	~GResManager();
	GResFile* CreateRes(const char* name);
	bool LoadTextureToVRAM(int id);
	bool LoadImage(const char* path, unsigned char** data, uint32* dataSize);

	const char*     mResDir;
	int             mMaxFileId;
	//ResDictionary    mDictinory;
};

#endif