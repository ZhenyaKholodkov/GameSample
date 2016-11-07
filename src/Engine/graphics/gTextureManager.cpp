#include "gTextureManager.h"
#include "gIOFile.h"
#include "gPngLoader.h"


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
	GTextureDictinary* dictionary = GTextureDictinary::Instance();
	GTextureObject* texture = dictionary->find(name);
	if (texture)
		return texture;

	char path[256];
	strcpy(path, "data/resources/scene1/");
	strcat(path, name);

	unsigned char* data;
	ulong32 width;
	ulong32 height;
	LoadPngImage(path, &data, &width, &height);

	texture = new GTextureObject();
	texture->mKey = name;
	texture->mGLTextureId = IGRender::Instance()->LoadTexture(data, width, height);
	texture->mWidth = width;
	texture->mHeight = height;

	dictionary->insert(texture);

	return texture;
}


void GTextureManager::unloadTexture(GTextureObject* texture)
{
	GTextureDictinary* dictionary = GTextureDictinary::Instance();
	dictionary->remove(texture);

	SAFE_DELETE(texture);
}