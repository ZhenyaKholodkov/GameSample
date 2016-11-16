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

