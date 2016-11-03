#ifndef _GRES_MANAGER_H
#define _GRES_MANAGER_H

#include "Types.h"

#include <vector>

extern "C"
{
#define		Byte	z_Byte					// to avoid conflcits in zconf.h
#include	"zlib.h"
#undef		Byte
#include	"png.h"
}
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

	void setResDirectory(const char* dir);

	std::vector<GResFile*> mResources;
private:
	GResManager();
	~GResManager();
	GResFile* CreateRes(const char* name);
	bool LoadTextureToVRAM(int id);
	bool LoadImage(const char* path, unsigned char** data, uint32* dataSize);
	bool LoadPngImage(const char* aFileName, unsigned char** aImage, unsigned long* aWidth, unsigned long* aHeight);

	const char*     mResDir;
	int             mMaxFileId;
	//ResDictionary    mDictinory;
};

#endif