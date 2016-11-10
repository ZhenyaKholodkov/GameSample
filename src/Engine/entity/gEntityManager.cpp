#include "gEntityManager.h"

using namespace std;


GEntityManager* GEntityManager::Instance()
{
	static GEntityManager* instance = new GEntityManager();
	return instance;
}

GEntityManager::GEntityManager():
	mFreeEntity(0),
	defaulEntityCount(100)
{
	mComponents.reserve(2);
	for (auto componentPool : mComponents)
	{
		componentPool.reserve(defaulEntityCount);
		for (auto component : componentPool)
		{
			component = nullptr;
		}
	}

	mSystems.reserve(2);
	for (auto system : mSystems)
	{
		system = nullptr;
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

	for (auto system : mSystems)
	{
		SAFE_DELETE(system);
	}
}

Entity GEntityManager::CreateEntity()
{
	return mFreeEntity++;
}

template<typename C, typename ... Args>
void   GEntityManager::AddComponentsToEntity(Entity entity, Args && ... args)
{
	uint32 index = GComponent<C>::getComponentIndex();
	C* c = new C(std::forward(args) ...);
	/*if (!mComponents[index][entity])
	{
		mComponents[index][entity] = new C(std::forward(srgs) ...);
	}*/
}