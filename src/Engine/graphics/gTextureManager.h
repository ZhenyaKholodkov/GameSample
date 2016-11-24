#ifndef GTEXTUREMANAGER_H
#include "GTextureObject.h"
#include "GTextureDictinary.h"

#include "gRenderManager.h"

class GTextureManager
{
public:
	static GTextureManager* Instance();

	GTextureObject* LoadTexture(const char* name);
	void unloadTexture(GTextureObject* texture);
private:
	GTextureManager();
	~GTextureManager();

private:
};
#define GTEXTUREMANAGER_H
#endif //TEXTUREMANAGER_H