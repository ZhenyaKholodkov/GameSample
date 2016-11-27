#ifndef GENTITYMANAGER_H
#define GENTITYMANAGER_H

#include <vector>
#include <queue>

#include "Types.h"
#include "GDefines.h"

#include "gPool.h"

#include "gLocationComponent.h"
#include "gAnimationComponent.h"
#include "gActionComponent.h"
#include "gMouseDownEventComponent.h"
#include "gMouseUpEventComponent.h"
#include "gMouseMoveEventComponent.h"
#include "gRenderableComponent.h"
#include "gMoveableComponent.h"

#include "gRenderSystem.h"
#include "gAnimationSystem.h"

class GEntityManager
{
public:
	static GEntityManager* Instance();

	Entity CreateEntity();
	void   DestroyEntity(Entity entity);

	GBaseComponent* GetComponent(Entity entity, uint32 index);

	vector<Entity>::const_iterator GetActiveEntitiesBegin() { return mActiveEntities.begin(); }
	vector<Entity>::const_iterator GetActiveEntitiesEnd() { return mActiveEntities.end(); }

	bool IsInsideEntity(Entity entity, GCursor point);
	void LocalPoint(Entity entity, GCursor& point, GCursor& localPoint);

public:
	template<typename C, typename... Args>
	C* AddComponentsToEntity(Entity entity, Args&& ... args);

	template<typename C>
	C* GetComponent(Entity entity);

	template<typename C>
	bool DoesHaveComponent(Entity entity);

	template<typename C>
	typename GPool<C>::GPoolIterator GetBeginComponent();
	template<typename C>
	typename GPool<C>::GPoolIterator GetEndComponent();

	template<typename C>
	typename GPool<C>::GPoolPairIterator GetBeginPairComponent();
	template<typename C>
	typename GPool<C>::GPoolPairIterator GetEndPairComponent();

private:
	GEntityManager();
	~GEntityManager();
	
	uint32 GetComponentCount();

private:
	queue<Entity>                   mAvailableEntities;

	vector<vector<GBaseComponent*>> mComponents; // заменить на другой контейнер
	vector<Entity>                  mActiveEntities;

	std::vector<GBasePool*> mComponentPools;
};

template<typename C, typename... Args>
C*   GEntityManager::AddComponentsToEntity(Entity entity, Args&& ... args)
{
	uint32 index = GComponent<C>::GetComponentId();
	/////////////////////////////////////////////////////////////////////////
	GPool<C>* componentPool = static_cast<GPool<C>*>(mComponentPools[index]);
	if (componentPool == nullptr)
	{
		componentPool = new GPool<C>(150);
		mComponentPools[index] = static_cast<GBasePool*>(componentPool);
	}
	C* new_component = componentPool->create(entity, std::forward<Args>(args) ...);
	//////////////////////////////////////////////////////////////////////////
	/*C* new_component = new C(std::forward<Args>(args) ...);
	if (!mComponents[index][entity])
	{
		mComponents[index][entity] = static_cast<GBaseComponent*>(new_component);
	}*/
	return new_component;
}

template<typename C>
C* GEntityManager::GetComponent (Entity entity)
{
	uint32 index = GComponent<C>::GetComponentId();
	GPool<C>* mComponentPool = static_cast<GPool<C>*>(mComponentPools[index]);
	if (mComponentPool == nullptr)
		return nullptr;
	return mComponentPool->get(entity);
	//return static_cast<C*>(mComponents[index][entity]);
}

template<typename C>
bool GEntityManager::DoesHaveComponent(Entity entity)
{
	uint32 index = GComponent<C>::GetComponentId();
	GPool<C>* mComponentPool = static_cast<GPool<C>*>(mComponentPools[index]);
	if (mComponentPool == nullptr)
		return nullptr;
	return mComponentPool->doesContainComponent(entity);
	//return mComponents[index][entity] != nullptr;
}


template<typename C>
typename GPool<C>::GPoolIterator GEntityManager::GetBeginComponent()
{
	typedef typename GPool<C>::GPoolIterator GPoolIterator;
	uint32 index = GComponent<C>::GetComponentId();
	GPool<C>* mComponentPool = static_cast<GPool<C>*>(mComponentPools[index]);
	return mComponentPool == nullptr ? GPoolIterator(nullptr) : mComponentPool->begin();
}

template<typename C>
typename GPool<C>::GPoolIterator GEntityManager::GetEndComponent()
{
	typedef typename GPool<C>::GPoolIterator GPoolIterator;
	uint32 index = GComponent<C>::GetComponentId();
	GPool<C>* mComponentPool = static_cast<GPool<C>*>(mComponentPools[index]);
	return mComponentPool == nullptr ? GPoolIterator(nullptr) : mComponentPool->end();
}

template<typename C>
typename GPool<C>::GPoolPairIterator GEntityManager::GetBeginPairComponent()
{
	typedef typename GPool<C>::GPoolPairIterator GPoolPairIterator;
	uint32 index = GComponent<C>::GetComponentId();
	GPool<C>* mComponentPool = static_cast<GPool<C>*>(mComponentPools[index]);
	return mComponentPool == nullptr ? GPoolPairIterator(nullptr) : mComponentPool->beginPair();
}
template<typename C>
typename GPool<C>::GPoolPairIterator GEntityManager::GetEndPairComponent()
{
	typedef typename GPool<C>::GPoolPairIterator GPoolPairIterator;
	uint32 index = GComponent<C>::GetComponentId();
	GPool<C>* mComponentPool = static_cast<GPool<C>*>(mComponentPools[index]);
	return mComponentPool == nullptr ? GPoolPairIterator(nullptr) : mComponentPool->endPair();
}
#endif

