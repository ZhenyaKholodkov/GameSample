#include "gSystemManager.h"


GSystemManager::GSystemManager()
{
	mSystems.resize(2);
	for (auto system : mSystems)
	{
		system = nullptr;
	}
}

GSystemManager::~GSystemManager()
{
	for (auto system : mSystems)
	{
		SAFE_DELETE(system);
	}
}

GSystemManager* GSystemManager::Instatnce()
{
	static GSystemManager*  instance= new GSystemManager();
	return instance;
}


void GSystemManager::update(int dt)
{
	for (auto system : mSystems)
	{
		if (system)
		{
			system->update(dt);
		}
	}
}