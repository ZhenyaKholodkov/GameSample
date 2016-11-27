#ifndef GRESOURCE_H
#define GRESOURCE_H

#include "gTextureHandle.h"
#include "Types.h"

enum ResourceType
{
	TYPE_TEXTURE_ATLAS = BIT(1),
	TYPE_SPRITE = BIT(2)
};

class GResource
{
	friend class GResManager;
	friend class GResourceDictionary;
public :
	GResource(const char* path, ResourceType type);
	virtual ~GResource();

private:
	GResource*   mNext;
	GResource*   mPrev;
	GResource*   mHashNext;

protected:
	ResourceType mType;
	char*  mKey;
};
#endif //GRESOURCE_H