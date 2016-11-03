#include "gTextureManager.h"


GTextureManager* GTextureManager::Instance()
{
	static GTextureManager* instance = new GTextureManager();
	return instance;
}


GTextureManager::GTextureManager()
{

}

GTextureManager::~GTextureManager()
{

}

GTextureObject* GTextureManager::LoadTexture(const char* name)
{
	GTextureObject* texture = GTextureDictinary::Instance()->find(name);
	if (texture)
		return texture;

}