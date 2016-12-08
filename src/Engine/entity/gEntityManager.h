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
#include "gMoveableAnimationComponent.h"
#include "gCollisionComponent.h"
#include "gChildComponent.h"
#include "gParentComponent.h"
#include "gCounterComponent.h"
#include "gKeyUpEventComponent.h"
#include "gKeyDownEventComponent.h"

#include "gRenderSystem.h"
#include "gAnimationSystem.h"

class GEntityManager
{
public:
	static GEntityManager* Instance();

	Entity CreateEntity();                  // creates and return new Entity. 
	void   DestroyEntity(Entity entity);    // destoryes the entity with compnents.
	
	bool IsInsideEntity(Entity entity, GCursor point);                   // check if the mouse cursor inside the entity
	void LocalPoint(Entity entity, GCursor& point, GCursor& localPoint); // localized point in entity coordinates

	void setChildParentRelations(Entity parent, Entity child);          // sets child-parent relationships between entites. Creates the ChildComponent and ParentComponent behind the scene.
	void removeParent(Entity child);                                    // removes parent for entity

public:
	template<typename C, typename... Args>
	C* AddComponentsToEntity(Entity entity, Args&& ... args);           //Creates Component C for entity. If it is first component of that type, The ComponentPool will be created.

	template<typename C>
	C* GetComponent(Entity entity);                                     // returns pointer on the component C for an entity

	template<typename C>
	bool DoesHaveComponent(Entity entity);                              //checks if entity contains the component C

	template<typename C>
	typename GComponentPool<C>::GPoolIterator GetBeginComponent();      // returns the iterator pointed on the first compnent in the pool
	template<typename C>
	typename GComponentPool<C>::GPoolIterator GetEndComponent();        // returns the iterator pointed on the lst compnent in the pool

	template<typename C>
	typename GComponentPool<C>::GPoolPairIterator GetBeginPairComponent();  // returns the iterator with pair of Entity-Component pointed on the first compnent in the pool
	template<typename C>
	typename GComponentPool<C>::GPoolPairIterator GetEndPairComponent();    // returns the iterator with pair of Entity-Component pointed on the last compnent in the pool

private:
	GEntityManager();
	~GEntityManager();
	
	uint32 GetComponentCount();
private:
	queue<Entity>                   mAvailableEntities;   // the queue of available entities 

	std::vector<GBasePool*>        mComponentPools;       // pools of components. Each Component class has the static identifier that determines the index in this vector for the component pool.

	const uint32 DEFUALT_POOL_SIZE = 10;  
};

template<typename C, typename... Args>
C*   GEntityManager::AddComponentsToEntity(Entity entity, Args&& ... args)
{
	uint32 index = GComponent<C>::GetComponentId();

	GComponentPool<C>* componentPool = static_cast<GComponentPool<C>*>(mComponentPools[index]);
	if (componentPool == nullptr)
	{
		componentPool = new GComponentPool<C>(DEFUALT_POOL_SIZE);
		mComponentPools[index] = static_cast<GBasePool*>(componentPool);
	}
	C* new_component = componentPool->create(entity, std::forward<Args>(args) ...);
	return new_component;
}

template<typename C>
C* GEntityManager::GetComponent (Entity entity)
{
	if (entity == -1)
		return nullptr;

	uint32 index = GComponent<C>::GetComponentId();
	GComponentPool<C>* mComponentPool = static_cast<GComponentPool<C>*>(mComponentPools[index]);
	if (mComponentPool == nullptr)
		return nullptr;
	return mComponentPool->getComponent(entity);
}

template<typename C>
bool GEntityManager::DoesHaveComponent(Entity entity)
{
	uint32 index = GComponent<C>::GetComponentId();
	GComponentPool<C>* mComponentPool = static_cast<GComponentPool<C>*>(mComponentPools[index]);
	if (mComponentPool == nullptr)
		return nullptr;
	return mComponentPool->doesContainComponent(entity);
}


template<typename C>
typename GComponentPool<C>::GPoolIterator GEntityManager::GetBeginComponent()
{
	typedef typename GComponentPool<C>::GPoolIterator GPoolIterator;
	uint32 index = GComponent<C>::GetComponentId();
	GComponentPool<C>* mComponentPool = static_cast<GComponentPool<C>*>(mComponentPools[index]);
	return mComponentPool == nullptr ? GPoolPairIterator(nullptr, 0) : mComponentPool->begin();
}

template<typename C>
typename GComponentPool<C>::GPoolIterator GEntityManager::GetEndComponent()
{
	typedef typename GComponentPool<C>::GPoolIterator GPoolIterator;
	uint32 index = GComponent<C>::GetComponentId();
	GComponentPool<C>* mComponentPool = static_cast<GComponentPool<C>*>(mComponentPools[index]);
	return mComponentPool == nullptr ? GPoolPairIterator(nullptr, 0) : mComponentPool->end();
}

template<typename C>
typename GComponentPool<C>::GPoolPairIterator GEntityManager::GetBeginPairComponent()
{
	typedef typename GComponentPool<C>::GPoolPairIterator GPoolPairIterator;
	uint32 index = GComponent<C>::GetComponentId();
	GComponentPool<C>* mComponentPool = static_cast<GComponentPool<C>*>(mComponentPools[index]);
	return mComponentPool == nullptr ? GPoolPairIterator(nullptr, 0) : mComponentPool->beginPair();
}
template<typename C>
typename GComponentPool<C>::GPoolPairIterator GEntityManager::GetEndPairComponent()
{
	typedef typename GComponentPool<C>::GPoolPairIterator GPoolPairIterator;
	uint32 index = GComponent<C>::GetComponentId();
	GComponentPool<C>* mComponentPool = static_cast<GComponentPool<C>*>(mComponentPools[index]);
	return mComponentPool == nullptr ? GPoolPairIterator(nullptr, 0) : mComponentPool->endPair();
}
#endif

