#include "gEntityManager.h"


GEntityManager* GEntityManager::Instance()
{
	static GEntityManager* instance = new GEntityManager();
	return instance;
}

GEntityManager::GEntityManager():
	mFreeEntity(0),
	defaulEntityCount(100)
{
	mComponents.resize(GetComponentCount());
	for (int i = 0; i < mComponents.size(); ++i)
	{
		mComponents[i].resize(defaulEntityCount);
	}

}

GEntityManager::~GEntityManager()
{
	for (auto componentPool : mComponents)
	{
		for (auto component : componentPool)
		{
			SAFE_DELETE(component);
		}
	}
}

Entity GEntityManager::CreateEntity()
{
	mActiveEntities.push_back(mFreeEntity);
	return mFreeEntity++;
}

GBaseComponent* GEntityManager::GetComponent(Entity entity, uint32 index)
{
	return mComponents[index][entity];
}

uint32 GEntityManager::GetComponentCount()
{
	return GBaseComponent::s_component_counter;
}

bool GEntityManager::IsInsideEntity(Entity entity, GPoint point)
{
	return false;
}