#include "gTextureManager.h"
#include "gIOFile.h"
#include "gPngLoader.h"


GTextureManager* GTextureManager::Instance()
{
	static GTextureManager instance;
	return &instance;
}


GTextureManager::GTextureManager()
{

}

GTextureManager::~GTextureManager()
{

}

GTextureObject* GTextureManager::LoadTexture(const char* name)
{
	GTextureDictinary* dictionary = GTextureDictinary::Instance();
	GTextureObject* texture = dictionary->find(name);
	if (texture)
		return texture;

	char path[256];
	strcpy(path, "data/resources/textures/");
	strcat(path, name);

	unsigned char* data;
	ulong32 width;
	ulong32 height;
	LoadPngImage(path, &data, &width, &height);

	texture = new GTextureObject(name);
	texture->mGLTextureId = GRenderManager::Instance()->loadTexture(data, width, height);
	texture->mWidth = width;
	texture->mHeight = height;

	dictionary->insert(texture);

	free(data);
	return texture;
}


void GTextureManager::unloadTexture(GTextureObject* texture)
{
	GTextureDictinary* dictionary = GTextureDictinary::Instance();
	dictionary->remove(texture);

	GRenderManager::Instance()->unloadTexture(texture->mGLTextureId);
}