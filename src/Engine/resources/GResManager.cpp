#include "GResManager.h"
#include "Utils.h"
#include "gRenderManager.h"
#include "tinyxml.h"

#include <stdio.h>
#include <stdarg.h>

//////////////!!!GResManager - realiztion!!!///////////
GResManager::GResManager() : mMaxFileId(0)
{
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
	GSprite* sprite = static_cast<GSprite*>(GResourceDictionary::Instance()->find(key));

	if (!sprite)
		return nullptr;

	if (!sprite->IsLoaded())
		sprite->load();

	return sprite;
}

bool GResManager::LoadResources(const char* pathToConfig)
{
	TiXmlDocument confDocument;
	if (!confDocument.LoadFile(pathToConfig))
	{
		return false;
	}
	if (confDocument.Error())
	{
		return false;
	}

	TiXmlElement* root = confDocument.RootElement();
	if (!root)
	{
		return false;
	}

	TiXmlElement* atlasTextureData = root->FirstChildElement("TextureAtlas");
	while (atlasTextureData)
	{
		GTextureAtlas* atlas = new GTextureAtlas(atlasTextureData->Attribute("imagePath"));
		atlasTextureData->QueryIntAttribute("width", &atlas->mWidth);
		atlasTextureData->QueryIntAttribute("height", &atlas->mHeight);
		GResourceDictionary::Instance()->insert(static_cast<GResource*>(atlas));

		TiXmlElement* spriteData = atlasTextureData->FirstChildElement("sprite");
		while (spriteData)
		{
			GSprite* sprite = new GSprite(spriteData->Attribute("n"));

			sprite->mTextureKey = atlas->mKey;
			spriteData->QueryIntAttribute("x", &sprite->mXPos);
			spriteData->QueryIntAttribute("y", &sprite->mYPos);
			spriteData->QueryIntAttribute("w", &sprite->mWidth);
			spriteData->QueryIntAttribute("h", &sprite->mHeight);

			float pivotX = 0.0f;
			float pivotY = 0.0f; 
			spriteData->QueryFloatAttribute("pX", &pivotX);
			spriteData->QueryFloatAttribute("pY", &pivotY);
			sprite->mPivotX = pivotX * sprite->mWidth;
			sprite->mPivotY = pivotY * sprite->mHeight;

			GResourceDictionary::Instance()->insert((GResource*)(sprite));

			spriteData = spriteData->NextSiblingElement();
		}
		atlasTextureData = atlasTextureData->NextSiblingElement();
	}
	return true;
}

//////////////!!!GResManager - end!!!///////////