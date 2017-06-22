#include "gTextureManager.h"
#include "gIOFile.h"
#include "gPngLoader.h"


GTextureManager::GTextureManager()
{
	mTextureDictionary.reset(new GTextureDictinary());
}

GTextureManager::~GTextureManager()
{

}

GTextureObject* GTextureManager::LoadTexture(const char* name)
{
	GTextureObject* texture = mTextureDictionary->find(name);
	if (texture)
		return texture;

	char path[256];
	strcpy(path, "resources/textures/");
	strcat(path, name);

	unsigned char* data;
	ulong32 width;
	ulong32 height;
	LoadPngImage(path, &data, &width, &height);

	texture = new GTextureObject(name);
	texture->mGLTextureId = GRenderManager::Instance()->loadTexture(data, width, height);
	texture->mWidth = width;
	texture->mHeight = height;

	mTextureDictionary->insert(texture);

	free(data);
	return texture;
}


void GTextureManager::unloadTexture(GTextureObject* texture)
{
	GRenderManager::Instance()->unloadTexture(texture->mGLTextureId);
	mTextureDictionary->remove(texture);
	SAFE_DELETE(texture);
}