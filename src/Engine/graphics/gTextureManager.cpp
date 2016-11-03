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