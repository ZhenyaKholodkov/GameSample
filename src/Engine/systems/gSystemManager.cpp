#include "gSystemManager.h"

GSystemManager* GSystemManager::instance()
{
	static GSystemManager instance;
	return &instance;
}

GSystemManager::GSystemManager()
{
	mSystems.resize(getSystemCount());
	for (auto system : mSystems)
	{
		system = nullptr;
	}
}

GSystemManager::~GSystemManager()
{
}

uint32 GSystemManager::getSystemCount() const
{
	return GBaseSystem::s_system_counter;
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