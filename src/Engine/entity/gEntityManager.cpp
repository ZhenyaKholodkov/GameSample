#include "gEntityManager.h"


GEntityManager* GEntityManager::Instance()
{
	static GEntityManager* instance = new GEntityManager();
	return instance;
}

GEntityManager::GEntityManager():
	defaulEntityCount(100)
{
	mComponents.resize(GetComponentCount());
	for (int index = 0; index < mComponents.size(); ++index)
	{
		mComponents[index].resize(defaulEntityCount);
	}
	for (uint32 index = 0; index < defaulEntityCount; ++index)
	{
		mAvailableEntities.push(index);
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
	Entity newEntity = mAvailableEntities.front();
	mAvailableEntities.pop();
	mActiveEntities.push_back(newEntity);
	return newEntity;
}

void GEntityManager::DestroyEntity(Entity entity)
{
	for (int index = 0; index < mComponents.size(); ++index)
	{
		SAFE_DELETE(mComponents[index][entity]);
	}
	mAvailableEntities.push(entity);
}

GBaseComponent* GEntityManager::GetComponent(Entity entity, uint32 index)
{
	return mComponents[index][entity];
}

uint32 GEntityManager::GetComponentCount()
{
	return GBaseComponent::s_component_counter;
}

//template<typename C, typename... Args>
//inline void   GEntityManager::AddComponentsToEntity(Entity entity, Args&& ... args)
//{
//	uint32 index = GComponent<C>::GetComponentId();
//	C* c = new C(std::forward(args) ...);
//	/*if (!mComponents[index][entity])
//	{
//		mComponents[index][entity] = new C(std::forward(srgs) ...);
//	}*/
//}

