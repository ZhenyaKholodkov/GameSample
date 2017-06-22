#ifndef GTEXTUREMANAGER_H
#include "GTextureObject.h"
#include "GTextureDictinary.h"

#include "gRenderManager.h"

class GTextureManager
{
	friend class GResManager;
public:
	~GTextureManager();
	GTextureObject* LoadTexture(const char* name);  // return the texture and loads it if needed.
	void unloadTexture(GTextureObject* texture);
private:
	GTextureManager();

private:
	std::unique_ptr<GTextureDictinary> mTextureDictionary;
};
#define GTEXTUREMANAGER_H
#endif //TEXTUREMANAGER_H