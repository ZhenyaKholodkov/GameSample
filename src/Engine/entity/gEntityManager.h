#ifndef GENTITYMANAGER_H
#define GENTITYMANAGER_H

#include <vector>
#include <stack>
#include <memory>
#include <functional>
#include <any>
#include <cassert>
#include <list>
#include <functional>
#include <utility>
#include <new>
#include <queue>

#include "Utils.h"
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
#include "gRotableComponent.h"
#include "gScalableComponent.h"

class GEntityManager : public std::enable_shared_from_this<GEntityManager>
{
	class EntityPriority;

	friend class Iterator;
	friend class GGameWindow;

	friend inline bool operator== (const GEntityManager::EntityPriority &lhs, const GEntityManager::EntityPriority &rhs);
	friend inline bool operator!= (const GEntityManager::EntityPriority &lhs, const GEntityManager::EntityPriority &rhs);
	friend inline bool operator< (const GEntityManager::EntityPriority &lhs, const GEntityManager::EntityPriority &rhs);
	friend inline bool operator> (const GEntityManager::EntityPriority &lhs, const GEntityManager::EntityPriority &rhs);
	public:
		template <typename T> struct identity { typedef T type; };

		/*class EntityHandler
		{
		public:
			EntityHandler(Entity entity, std::shared_ptr<GEntityManager> manager) {}
			~EntityHandler() {}
		private:
			Entity mEntity;
			std::shared_ptr<GEntityManager> mEntityManager;
		};*/

		class Iterator : std::iterator<std::forward_iterator_tag, std::list<EntityPriority>::const_iterator>
		{
			friend class GEntityManager;
		public:
			bool         operator==(const Iterator& right) const { return mIter == right.mIter; }
			bool         operator!=(const Iterator& right) const { return mIter != right.mIter; }
			Entity       operator*()       { return mIter->mEntity; }
			const Entity operator*() const { return mIter->mEntity; }
			Iterator&    operator++()      { nextExistedEntity(); return *this; }

			Iterator&    operator=(const Iterator& right)
			{
				mManager = right.mManager;
				mMask = right.mMask;
				mIter = right.mIter;
				return *this; 
			}

			void changeIter(Entity entity)
			{
				//if(!isEntityValid(mIter->mEntity))
				if (mManager->mEntityPriorities.size() == 1)
					mIter = mManager->mEntityPriorities.end();
				if(mIter->mEntity == entity && mIter != mManager->mEntityPriorities.begin())
					--mIter;
			}
		private:
			Iterator(std::shared_ptr<GEntityManager> manager, ComponentMask mask, std::list<EntityPriority>::const_iterator iter) :mManager(manager), mMask(mask), mIter(iter)
			{
				mConn = mManager->attachToEntityDestroyed(boost::bind(&Iterator::changeIter, this, _1));
				if (predicate())
				{
					nextExistedEntity();
				}
			}
			Iterator(const Iterator& iter) :
				mManager(iter.mManager), mMask(iter.mMask)
			{}
			~Iterator()
			{
				mConn.disconnect();
			}


			void nextExistedEntity()
			{
				if(mIter != mManager->mEntityPriorities.end())
				{
					++mIter;
					if (predicate())
					{
						nextExistedEntity();
					}
				}
			}

			inline bool predicate() const 
			{
				return mIter != mManager->mEntityPriorities.end() && (!mManager->mComponentMasks.size() || !(mManager->getMaskFor(mIter->mEntity).contains(mMask)));
			}
		private:
			std::shared_ptr<GEntityManager> mManager;
			std::list<EntityPriority>::const_iterator mIter;
			ComponentMask mMask;
			boost::signals2::connection mConn;
		};

		template <typename ... Components>
		class View
		{
			friend class GEntityManager;
		public:
			View(std::shared_ptr<GEntityManager> manager, ComponentMask mask) : mManager(manager), mMask(mask) {}
			View(View& view) : mManager(view.mManager), mMask(view.mMask) {}

			GEntityManager::Iterator begin() const { return GEntityManager::Iterator(mManager, mMask, mManager->mEntityPriorities.begin()); }
			GEntityManager::Iterator end()   const { return GEntityManager::Iterator(mManager, mMask, mManager->mEntityPriorities.end()); }

			void each(typename identity<std::function<void(Entity entity, Components&...)>>::type f)
			{
			//	GEntityManager::Iterator mIter = begin();
			//	boost::signals2::connection conn = mManager->attachToEntityDestroyed(boost::bind(&Iterator::changeIter, &mIter, _1));
			//	while(mIter != end())
				for (Entity it : *this)
				{
					f(it, *(mManager->getComponent<Components>(it))...);  //f(it, *(it.component<Components>())...);
			//		++mIter;
				}
			//	conn.disconnect();
			}
		private:
			std::shared_ptr<GEntityManager> mManager;
			ComponentMask mMask;
			
		};
private:
	struct EntityPriority
	{
		EntityPriority(Entity entity, uint32 priority = 0) : mEntity(entity), mPriority(priority) {}
		Entity mEntity;
		uint32 mPriority;
	};
public:
	Entity createEntity(uint32 priority = 0);                  // creates and return new Entity. 
	void   destroyEntity(Entity entity);    // destoryes the entity with compnents.
	void   destroyAllEntites();
	
	bool isInsideEntity(Entity entity, GCursor point) const;                   // check if the mouse cursor inside the entity
	void getLocalPoint(Entity entity, GCursor& point, GCursor& localPoint) const; // localized point in entity coordinates

	void setChildParentRelations(Entity parent, Entity child);          // sets child-parent relationships between entites. Creates the ChildComponent and ParentComponent behind the scene.
	void removeParent(Entity child);                                    // removes parent for entity
private:
	GEntityManager();
public:
	~GEntityManager();
public:
	Entity createPlainEntity(GSprite* sprite, float xPos, float yPos, uint32 priority = 0);
	Entity createButtonEntity(GSprite* normal, GSprite* move, GSprite* down, float xPos, float yPos, uint32 priority = 0);

	template<typename C, typename... Args>
	C* addComponentsToEntity(Entity entity, Args&& ... args);           //Creates Component C for entity. If it is first component of that type, The ComponentPool will be created.

	template<typename C>
	C* getComponent(Entity entity) const;                                    // returns pointer on the component C for an entity
	
	template<typename C>
	bool doesHaveComponent(Entity entity) const;                              //checks if entity contains the component C

	template<typename C>
	std::shared_ptr<const GBasePool> GEntityManager::getComponentPool() const;

	template<typename... Components>
	GEntityManager::View<Components...> getEntitiesWithComponents();

	template <typename... Components>
	void each(typename GEntityManager::identity<std::function<void(Entity entity, Components&...)>>::type f);

	template<typename... Components>
	ComponentMask getComponentMasks() const;
	template<typename C>
	ComponentMask getComponentMask() const;

	const ComponentMask& getMaskFor(Entity entity) const { return mComponentMasks[entity]; }

	boost::signals2::connection attachToEntityDestroyed(boost::signals2::signal<void(Entity)>::slot_type const& f)
	{
		return signal_EntityToDestroy.connect(f);
	}
private:
	uint32 getComponentCount();
	void   addEntityToPriorities(Entity entity, uint32 priority = 0);

	boost::signals2::signal<void(Entity)> signal_EntityToDestroy;
private:
	const uint32 DEFUALT_POOL_SIZE = 10;
	std::queue<Entity>           mAvailableEntities;   // the queue of available entities 

	std::vector<std::shared_ptr<GBasePool>>  mComponentPools;       // pools of components. Each Component class has the static identifier that determines the index in this vector for the component pool.
	std::vector<std::vector<size_t>> mComponentIndexes;
	std::vector<ComponentMask>       mComponentMasks;
	std::list<EntityPriority>        mEntityPriorities;
};


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
GEntityManager::View<Components...> GEntityManager::getEntitiesWithComponents()
{
	std::shared_ptr<GEntityManager> manager = shared_from_this();
	return View<Components...>(manager, getComponentMasks<Components...>());
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
	if (!doesHaveComponent<C>(entity) || !mComponentPools[index])
		return nullptr;
	return static_cast<C*>(mComponentPools[index]->getComponent(mComponentIndexes[entity][index]));
}

template<typename C>
bool GEntityManager::doesHaveComponent(Entity entity) const
{
	uint32 id = GComponent<C>::getComponentId();
	return mComponentMasks[entity].contains(id);
}


inline bool operator== (const GEntityManager::EntityPriority &lhs, const GEntityManager::EntityPriority &rhs)
{
	return lhs.mPriority == rhs.mPriority;
}
inline bool operator!= (const GEntityManager::EntityPriority &lhs, const GEntityManager::EntityPriority &rhs)
{
	return lhs.mPriority != rhs.mPriority;
}
inline bool operator< (const GEntityManager::EntityPriority &lhs, const GEntityManager::EntityPriority &rhs)
{
	return lhs.mPriority < rhs.mPriority;
}
inline bool operator> (const GEntityManager::EntityPriority &lhs, const GEntityManager::EntityPriority &rhs)
{
	return lhs.mPriority > rhs.mPriority;
}

#endif

