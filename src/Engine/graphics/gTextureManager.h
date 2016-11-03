#ifndef GTEXTUREMANAGER_H
#include "GTextureObject.h"
#include "GTextureDictinary.h"

#include "IGRender.h"

class GTextureManager
{
public:
	GTextureManager* Instance();

	GTextureObject* LoadTexture(const char* name);
private:
	GTextureManager();
	~GTextureManager();

private:
};
#define GTEXTUREMANAGER_H
#endif //TEXTUREMANAGER_H