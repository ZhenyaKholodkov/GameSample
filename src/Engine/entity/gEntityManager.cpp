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
	mComponents.resize(2);
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

//template<typename C, typename... Args>
//inline void   GEntityManager::AddComponentsToEntity(Entity entity, Args&& ... args)
//{
//	uint32 index = GComponent<C>::getComponentIndex();
//	C* c = new C(std::forward(args) ...);
//	/*if (!mComponents[index][entity])
//	{
//		mComponents[index][entity] = new C(std::forward(srgs) ...);
//	}*/
//}

