#ifndef GENTITYMANAGER_H
#define GENTITYMANAGER_H

#include <vector>
#include <stack>
#include <memory>
#include <functional>
#include <any>
#include <cassert>
#include <bitset>
#include <functional>
#include <utility>
#include <new>

#include "Types.h"
#include "GDefines.h"
#include "GExeptions.h"

#include "gPool.h"
#include "gComponentMask.h"

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


class GEntityManager : public std::enable_shared_from_this<GEntityManager>
{
	friend class Iterator;
	friend class GGameWindow;
	public:
		template <typename T> struct identity { typedef T type; };


		class Iterator : std::iterator<std::forward_iterator_tag, Entity>
		{
			friend class GEntityManager;
		public:
			bool         operator==(const Iterator& right) const { return mCurrentEntity == right.mCurrentEntity; }
			bool         operator!=(const Iterator& right) const { return mCurrentEntity != right.mCurrentEntity; }
			Entity       operator*()       { return mCurrentEntity; }
			const Entity operator*() const { return mCurrentEntity; }
			Iterator&    operator++()      { nextExistedEntity(); return *this; }

			Iterator&    operator=(const Iterator& right)
			{
				mManager = right.mManager;
				mMask = right.mMask;
				mCapacity = right.mManager->mComponentIndexes.size();
				mCurrentEntity = right.mCurrentEntity;
				return *this; 
			}
		private:
			Iterator(std::shared_ptr<const GEntityManager> manager, ComponentMask mask, uint32 index) :mManager(manager), mMask(mask), mCapacity(manager->mComponentIndexes.size()), mCurrentEntity(index)
			{
				if (predicate())
				{
					nextExistedEntity();
				}
			}
			Iterator(const Iterator& iter) :
				mManager(iter.mManager), mMask(iter.mMask), mCapacity(iter.mManager->mComponentIndexes.size()), mCurrentEntity(iter.mCurrentEntity)
			{}
			~Iterator() {}


			void nextExistedEntity()
			{
				if (mCurrentEntity < mCapacity)
				{
					++mCurrentEntity;
					if (predicate())
					{
						nextExistedEntity();
					}
				}
			}

			inline bool predicate()
			{
				return mCurrentEntity < mCapacity && (!mManager->mComponentMasks.size() || mCurrentEntity == INVALID_ENTITY || !(mManager->getMaskFor(mCurrentEntity).contains(mMask)));
			}
		private:
			std::shared_ptr<const GEntityManager> mManager;
			Entity mCurrentEntity;
			size_t mCapacity;
			ComponentMask mMask;
		};

		template <typename ... Components>
		class View
		{
			friend class GEntityManager;
		public:
			View(std::shared_ptr<const GEntityManager> manager, ComponentMask mask) : mManager(manager), mMask(mask) {}
			View(View& view) : mManager(view.mManager), mMask(view.mMask) {}

			GEntityManager::Iterator begin() const { return GEntityManager::Iterator(mManager, mMask, 0); }
			GEntityManager::Iterator end()   const { const GEntityManager::Iterator iter(mManager, mMask, (mManager->mComponentIndexes.size() - 1)); return iter; }

			void each(typename identity<std::function<void(Entity entity, Components&...)>>::type f)
			{
				for (Entity it : *this)
				{
					f(it, *(mManager->getComponent<Components>(it))...);  //f(it, *(it.component<Components>())...);
				}
			}
		private:
			std::shared_ptr<const GEntityManager> mManager;
			ComponentMask mMask;
		};
public:
	//static GEntityManager* instance();
	Entity createEntity();                  // creates and return new Entity. 
	void   destroyEntity(Entity entity);    // destoryes the entity with compnents.
	
	bool isInsideEntity(Entity entity, GCursor point) const;                   // check if the mouse cursor inside the entity
	void getLocalPoint(Entity entity, GCursor& point, GCursor& localPoint) const; // localized point in entity coordinates

	void setChildParentRelations(Entity parent, Entity child);          // sets child-parent relationships between entites. Creates the ChildComponent and ParentComponent behind the scene.
	void removeParent(Entity child);                                    // removes parent for entity

private:
	GEntityManager();
public:
	~GEntityManager();
public:
	template<typename C, typename... Args>
	C* addComponentsToEntity(Entity entity, Args&& ... args);           //Creates Component C for entity. If it is first component of that type, The ComponentPool will be created.

	template<typename C>
	C* getComponent(Entity entity) const;                                    // returns pointer on the component C for an entity
	
	template<typename C>
	bool doesHaveComponent(Entity entity) const;                              //checks if entity contains the component C

	template<typename C>
	std::shared_ptr<const GBasePool> GEntityManager::getComponentPool() const;

	template<typename... Components>
	GEntityManager::View<Components...> getEntitiesWithComponents() const;

	template <typename... Components>
	void each(typename GEntityManager::identity<std::function<void(Entity entity, Components&...)>>::type f);

	template<typename... Components>
	ComponentMask getComponentMasks() const;
	/*template<typename C1, typename C2, typename... Components>
	ComponentMask getComponentMask() const;*/
	template<typename C>
	ComponentMask getComponentMask() const;

	const ComponentMask& getMaskFor(Entity entity) const { return mComponentMasks[entity]; }
	/*template<typename C>
	typename GComponentPool<C>::GPoolIterator getBeginComponent() const;      // returns the iterator pointed on the first compnent in the pool
	template<typename C>
	typename GComponentPool<C>::GPoolIterator getEndComponent() const;        // returns the iterator pointed on the lst compnent in the pool

	template<typename C>
	typename GComponentPool<C>::GPoolPairIterator getBeginPairComponent() const;  // returns the iterator with pair of Entity-Component pointed on the first compnent in the pool
	template<typename C>
	typename GComponentPool<C>::GPoolPairIterator getEndPairComponent() const;    // returns the iterator with pair of Entity-Component pointed on the last compnent in the pool
	*/
private:
	uint32 getComponentCount();
private:
	const uint32 DEFUALT_POOL_SIZE = 10;
	std::stack<Entity>           mAvailableEntities;   // the queue of available entities 

	std::vector<std::shared_ptr<GBasePool>>  mComponentPools;       // pools of components. Each Component class has the static identifier that determines the index in this vector for the component pool.
	std::vector<std::vector<size_t>> mComponentIndexes;
	std::vector<ComponentMask>       mComponentMasks;
};


/*template<typename C1, typename C2, typename... Components>
ComponentMask GEntityManager::getComponentMask() const
{
	return getComponentMask<C1> | getComponentMask<C2, Components>();
}*/

template<typename... Components>
ComponentMask GEntityManager::getComponentMasks() const
{
	std::vector<ComponentMask> masks = {getComponentMask<Components>()...};
	ComponentMask mask;
	for (auto m : masks)
	{
		mask += m;
	}
	return mask;
}

template<typename C>
ComponentMask GEntityManager::getComponentMask() const
{
	uint32 index = GComponent<C>::getComponentId();
	ComponentMask mask(index);
	return mask;
}

template<typename... Components>
GEntityManager::View<Components...> GEntityManager::getEntitiesWithComponents() const
{
	return View<Components...>(shared_from_this(), getComponentMasks<Components...>());
}

template <typename ... Components>
void GEntityManager::each(typename GEntityManager::identity<std::function<void(Entity entity, Components&...)>>::type f) 
{
	return getEntitiesWithComponents<Components...>().each(f);
}

template<class C>
std::shared_ptr<const GBasePool> GEntityManager::getComponentPool() const
{
	uint32 index = GComponent<C>::getComponentId();
	return &mComponentPools[index];
}

template<class C, typename... Args>
C*   GEntityManager::addComponentsToEntity(Entity entity, Args&& ... args)
{
	uint32 index = GComponent<C>::getComponentId();

	if (!mComponentPools[index])
	{
		mComponentPools[index].reset(new GComponentPool<C>(DEFUALT_POOL_SIZE));
	}

	int component_index = mComponentPools[index]->create();
	mComponentIndexes[entity][index] = component_index;
	C* component = static_cast<C*>(mComponentPools[index]->getComponent(component_index));
	new (component) C(std::forward<Args>(args)...);
	mComponentMasks[entity] += getComponentMask<C>();
	return component;
}

template<typename C>
C* GEntityManager::getComponent (Entity entity) const
{
	assert(entity != INVALID_ENTITY);

	uint32 index = GComponent<C>::getComponentId();
	if (!mComponentPools[index])
		return nullptr;
	return static_cast<C*>(mComponentPools[index]->getComponent(mComponentIndexes[entity][index]));
}

template<typename C>
bool GEntityManager::doesHaveComponent(Entity entity) const
{
	uint32 id = GComponent<C>::getComponentId();
	return mComponentMasks[entity].contains(id);
}
/*
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
}*/
#endif

