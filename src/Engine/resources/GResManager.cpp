#include "GResManager.h"
#include "Utils.h"
#include "gRenderManager.h"
#include "tinyxml.h"

#include <stdio.h>
#include <stdarg.h>

//////////////!!!GResManager - realiztion!!!///////////
GResManager::GResManager() : mMaxFileId(0)
{
	mResourceDictionary.reset(new GResourceDictionary());
	mTextureManager.reset(new GTextureManager());
}

GResManager::~GResManager()
{
}

GResManager* GResManager::Instance()
{
	static GResManager instance;
	return &instance;
}

void GResManager::setResDirectory(const char* dir)
{
	mResDir = dir; 
}

bool GResManager::LoadImage(const char* path, unsigned char** data, uint32* dataSize)
{
	FILE* file = fopen(path, "rb");

	size_t fLen = ftell(file);

	*data = new unsigned char[fLen];
	*dataSize = (uint32)fread(data, 1, fLen, file);

	fclose(file);
	return true;
}

GSprite* GResManager::GetSprite(const char* key)
{
	GSprite* sprite = static_cast<GSprite*>(mResourceDictionary->find(key));

	if (!sprite)
		return nullptr;

	if (!sprite->IsLoaded())
		sprite->load();

	return sprite;
}

bool GResManager::LoadResources(const char* pathToConfig)
{
	GLog::printLog("GResManager::LoadResources()\n");
	TiXmlDocument confDocument;
	if (!confDocument.LoadFile(pathToConfig))
	{
		GLog::printLog("!confDocument.LoadFile(pathToConfig)\n");
		return false;
	}
	GLog::printLog("GResManager::LoadResources()1\n");
	if (confDocument.Error())
	{
		return false;
	}

	GLog::printLog("GResManager::LoadResources()2\n");
	TiXmlElement* root = confDocument.RootElement();
	if (!root)
	{
		return false;
	}

	GLog::printLog("GResManager::LoadResources() opened\n");
	TiXmlElement* atlasTextureData = root->FirstChildElement("TextureAtlas");
	while (atlasTextureData)
	{
		GTextureAtlas* atlas = new GTextureAtlas(atlasTextureData->Attribute("imagePath"));
		atlasTextureData->QueryIntAttribute("width", &atlas->mWidth);
		atlasTextureData->QueryIntAttribute("height", &atlas->mHeight);
		mResourceDictionary->insert(static_cast<GResource*>(atlas));

		TiXmlElement* spriteData = atlasTextureData->FirstChildElement("sprite");
		while (spriteData)
		{
			GSprite* sprite = new GSprite(spriteData->Attribute("n"), mTextureManager);

			sprite->mTextureKey = atlas->mKey;
			spriteData->QueryIntAttribute("x", &sprite->mRect.mXPos);
			spriteData->QueryIntAttribute("y", &sprite->mRect.mYPos);
			spriteData->QueryIntAttribute("w", &sprite->mRect.mWidth);
			spriteData->QueryIntAttribute("h", &sprite->mRect.mHeight);

			float pivotX = 0.0f;
			float pivotY = 0.0f; 
			spriteData->QueryFloatAttribute("pX", &pivotX);
			spriteData->QueryFloatAttribute("pY", &pivotY);
			sprite->mRect.mPivotX = pivotX * sprite->mRect.mWidth;
			sprite->mRect.mPivotY = pivotY * sprite->mRect.mHeight;

			mResourceDictionary->insert((GResource*)(sprite));

			spriteData = spriteData->NextSiblingElement();
		}
		atlasTextureData = atlasTextureData->NextSiblingElement();
	}
	GLog::printLog("GResManager::LoadResources() end\n");
	return true;
}

//////////////!!!GResManager - end!!!///////////