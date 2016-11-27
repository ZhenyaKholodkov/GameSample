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

class GResManager
{
public:

	enum { DefaultResourceSize = 100 };

	static GResManager* Instance();

	bool LoadResource(int id);
	bool LoadResources(const char* pathToConfig);
	GSprite* GetSprite(const char* key);

	void setResDirectory(const char* dir);

private:
	GResManager();
	~GResManager();
	bool LoadImage(const char* path, unsigned char** data, uint32* dataSize);

	const char*     mResDir;
	int             mMaxFileId;
};

#endif