#ifndef GENTITYMANAGER_H
#define GENTITYMANAGER_H

#include <vector>
#include <queue>
#include <memory>
#include <functional>
#include <any>

#include "Types.h"
#include "GDefines.h"
#include "GExeptions.h"

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

class GEntityManager
{
public:
	static GEntityManager* instance();
	Entity createEntity();                  // creates and return new Entity. 
	void   destroyEntity(Entity entity);    // destoryes the entity with compnents.
	
	bool isInsideEntity(Entity entity, GCursor point) const;                   // check if the mouse cursor inside the entity
	void getLocalPoint(Entity entity, GCursor& point, GCursor& localPoint) const; // localized point in entity coordinates

	void setChildParentRelations(Entity parent, Entity child);          // sets child-parent relationships between entites. Creates the ChildComponent and ParentComponent behind the scene.
	void removeParent(Entity child);                                    // removes parent for entity

public:
	GEntityManager();
	~GEntityManager();

	template<typename C, typename... Args>
	C* addComponentsToEntity(Entity entity, Args&& ... args);           //Creates Component C for entity. If it is first component of that type, The ComponentPool will be created.

	template<typename C>
	C* getComponent(Entity entity) const;                                     // returns pointer on the component C for an entity
	
	template<typename C>
	bool doesHaveComponent(Entity entity) const;                              //checks if entity contains the component C

	template<typename C>
	const GComponentPool<C>* getComponentPool() const;

	template<typename C>
	typename GComponentPool<C>::GPoolIterator getBeginComponent() const;      // returns the iterator pointed on the first compnent in the pool
	template<typename C>
	typename GComponentPool<C>::GPoolIterator getEndComponent() const;        // returns the iterator pointed on the lst compnent in the pool

	template<typename C>
	typename GComponentPool<C>::GPoolPairIterator getBeginPairComponent() const;  // returns the iterator with pair of Entity-Component pointed on the first compnent in the pool
	template<typename C>
	typename GComponentPool<C>::GPoolPairIterator getEndPairComponent() const;    // returns the iterator with pair of Entity-Component pointed on the last compnent in the pool

private:
	uint32 getComponentCount();
private:
	queue<Entity>                mAvailableEntities;   // the queue of available entities 

	std::vector<std::any>        mComponentPools;       // pools of components. Each Component class has the static identifier that determines the index in this vector for the component pool.

	const uint32 DEFUALT_POOL_SIZE = 10;  
};

template<class C>
const GComponentPool<C>* GEntityManager::getComponentPool() const
{
	uint32 index = GComponent<C>::getComponentId();
	if (!mComponentPools[index].has_value())
	{
		return nullptr;
	}
	else if (mComponentPools[index].type() != typeid(GComponentPool<C>))
	{
		throw GPoolCastException();
	}
	return std::any_cast<const GComponentPool<C>>(&mComponentPools[index]);
}

template<class C, typename... Args>
C*   GEntityManager::addComponentsToEntity(Entity entity, Args&& ... args)
{
	uint32 index = GComponent<C>::getComponentId();

	if (!mComponentPools[index].has_value())
	{
		mComponentPools[index] = GComponentPool<C>(DEFUALT_POOL_SIZE);
	}
	if (mComponentPools[index].type() != typeid(GComponentPool<C>))
	{
		throw GPoolCastException();
	}
	GComponentPool<C>* componentPool = std::any_cast<GComponentPool<C>>(&mComponentPools[index]);

	auto new_component = componentPool->create(entity, std::forward<Args>(args) ...);
	return new_component;
}

template<typename C>
C* GEntityManager::getComponent (Entity entity) const
{
	if (entity == -1)
		return nullptr;

	auto componentPool = getComponentPool<C>();
	return componentPool->getComponent(entity);
}

template<typename C>
bool GEntityManager::doesHaveComponent(Entity entity) const
{
	auto componentPool = getComponentPool<C>();
	return componentPool->doesContainComponent(entity);
}

template<typename C>
typename GComponentPool<C>::GPoolIterator GEntityManager::getBeginComponent() const
{
	typedef typename GComponentPool<C>::GPoolIterator GPoolIterator;
	auto componentPool = getComponentPool<C>();
	return !componentPool  ? GPoolPairIterator(nullptr, 0) : componentPool->begin();
}

template<typename C>
typename GComponentPool<C>::GPoolIterator GEntityManager::getEndComponent() const
{
	typedef typename GComponentPool<C>::GPoolIterator GPoolIterator;
	auto componentPool = getComponentPool<C>();
	return !componentPool  ? GPoolPairIterator(nullptr, 0) : componentPool->end();
}

template<typename C>
typename GComponentPool<C>::GPoolPairIterator GEntityManager::getBeginPairComponent() const
{
	typedef typename GComponentPool<C>::GPoolPairIterator GPoolPairIterator;
	auto componentPool = getComponentPool<C>();
	return !componentPool  ? GPoolPairIterator(nullptr, 0) : componentPool->begin();
}

template<typename C>
typename GComponentPool<C>::GPoolPairIterator GEntityManager::getEndPairComponent() const
{
	typedef typename GComponentPool<C>::GPoolPairIterator GPoolPairIterator;
	auto componentPool = getComponentPool<C>();
	return !componentPool  ? GPoolPairIterator(nullptr, 0) : componentPool->end();
}
#endif

