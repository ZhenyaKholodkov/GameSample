#ifndef GENTITYMANAGER_H
#define GENTITYMANAGER_H

#include <vector>
#include <queue>

#include "Types.h"
#include "GDefines.h"

#include "gPool.h"

#include "gLocationComponent.h"
#include "gAnimationComponent.h"
#include "gMouseDownEventComponent.h"
#include "gMouseUpEventComponent.h"
#include "gMouseMoveEventComponent.h"
#include "gRenderableComponent.h"
#include "gMoveableComponent.h"
#include "gChildComponent.h"
#include "gParentComponent.h"

#include "gRenderSystem.h"
#include "gAnimationSystem.h"

class GEntityManager
{
public:
	static GEntityManager* Instance();

	Entity CreateEntity();
	void   DestroyEntity(Entity entity);
	
	bool IsInsideEntity(Entity entity, GCursor point);
	void LocalPoint(Entity entity, GCursor& point, GCursor& localPoint);

	void setChildParentRelations(Entity parent, Entity child);
	void removeParent(Entity child);

public:
	template<typename C, typename... Args>
	C* AddComponentsToEntity(Entity entity, Args&& ... args);

	template<typename C>
	C* GetComponent(Entity entity);

	template<typename C>
	bool DoesHaveComponent(Entity entity);

	template<typename C>
	typename GComponentPool<C>::GPoolIterator GetBeginComponent();
	template<typename C>
	typename GComponentPool<C>::GPoolIterator GetEndComponent();

	template<typename C>
	typename GComponentPool<C>::GPoolPairIterator GetBeginPairComponent();
	template<typename C>
	typename GComponentPool<C>::GPoolPairIterator GetEndPairComponent();

private:
	GEntityManager();
	~GEntityManager();
	
	uint32 GetComponentCount();
private:
	queue<Entity>                   mAvailableEntities;

	std::vector<GBasePool*>        mComponentPools;
};

template<typename C, typename... Args>
C*   GEntityManager::AddComponentsToEntity(Entity entity, Args&& ... args)
{
	uint32 index = GComponent<C>::GetComponentId();

	GComponentPool<C>* componentPool = static_cast<GComponentPool<C>*>(mComponentPools[index]);
	if (componentPool == nullptr)
	{
		componentPool = new GComponentPool<C>(150);
		mComponentPools[index] = static_cast<GBasePool*>(componentPool);
	}
	C* new_component = componentPool->create(entity, std::forward<Args>(args) ...);
	return new_component;
}

template<typename C>
C* GEntityManager::GetComponent (Entity entity)
{
	uint32 index = GComponent<C>::GetComponentId();
	GComponentPool<C>* mComponentPool = static_cast<GComponentPool<C>*>(mComponentPools[index]);
	if (mComponentPool == nullptr)
		return nullptr;
	return mComponentPool->get(entity);
	//return static_cast<C*>(mComponents[index][entity]);
}

template<typename C>
bool GEntityManager::DoesHaveComponent(Entity entity)
{
	uint32 index = GComponent<C>::GetComponentId();
	GComponentPool<C>* mComponentPool = static_cast<GComponentPool<C>*>(mComponentPools[index]);
	if (mComponentPool == nullptr)
		return nullptr;
	return mComponentPool->doesContainComponent(entity);
	//return mComponents[index][entity] != nullptr;
}


template<typename C>
typename GComponentPool<C>::GPoolIterator GEntityManager::GetBeginComponent()
{
	typedef typename GComponentPool<C>::GPoolIterator GPoolIterator;
	uint32 index = GComponent<C>::GetComponentId();
	GComponentPool<C>* mComponentPool = static_cast<GComponentPool<C>*>(mComponentPools[index]);
	return mComponentPool == nullptr ? GPoolIterator(nullptr) : mComponentPool->begin();
}

template<typename C>
typename GComponentPool<C>::GPoolIterator GEntityManager::GetEndComponent()
{
	typedef typename GComponentPool<C>::GPoolIterator GPoolIterator;
	uint32 index = GComponent<C>::GetComponentId();
	GComponentPool<C>* mComponentPool = static_cast<GComponentPool<C>*>(mComponentPools[index]);
	return mComponentPool == nullptr ? GPoolIterator(nullptr) : mComponentPool->end();
}

template<typename C>
typename GComponentPool<C>::GPoolPairIterator GEntityManager::GetBeginPairComponent()
{
	typedef typename GComponentPool<C>::GPoolPairIterator GPoolPairIterator;
	uint32 index = GComponent<C>::GetComponentId();
	GComponentPool<C>* mComponentPool = static_cast<GComponentPool<C>*>(mComponentPools[index]);
	return mComponentPool == nullptr ? GPoolPairIterator(nullptr) : mComponentPool->beginPair();
}
template<typename C>
typename GComponentPool<C>::GPoolPairIterator GEntityManager::GetEndPairComponent()
{
	typedef typename GComponentPool<C>::GPoolPairIterator GPoolPairIterator;
	uint32 index = GComponent<C>::GetComponentId();
	GComponentPool<C>* mComponentPool = static_cast<GComponentPool<C>*>(mComponentPools[index]);
	return mComponentPool == nullptr ? GPoolPairIterator(nullptr) : mComponentPool->endPair();
}
#endif

