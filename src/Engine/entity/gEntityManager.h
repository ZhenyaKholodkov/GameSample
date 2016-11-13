#ifndef GENTITYMANAGER_H
#define GENTITYMANAGER_H

#include <vector>

#include "Types.h"

#include "gLocationComponent.h"
#include "gRenderableComponent.h"
#include "gAnimationComponent.h"

#include "gRenderSystem.h"
#include "gAnimationSystem.h"

class GEntityManager
{
public:
	static GEntityManager* Instance();

	Entity CreateEntity();

	template<typename C, typename... Args>
	C* AddComponentsToEntity(Entity entity, Args&& ... args);

	template<typename C>
	C* GetComponent(Entity entity);

	GBaseComponent* GetComponent(Entity entity, uint32 index);

	vector<Entity>::const_iterator GetActiveEntitiesBegin() 
	{
		bool d = true;
		return mActiveEntities.begin(); 
	}
	vector<Entity>::const_iterator GetActiveEntitiesEnd() { return mActiveEntities.end(); }



private:
	GEntityManager();
	~GEntityManager();
	
private:
	Entity mFreeEntity;

	uint32 defaulEntityCount;

	vector<vector<GBaseComponent*>> mComponents; // заменить на другой контейнер
	vector<Entity>                  mActiveEntities;
};

template<typename C, typename... Args>
C*   GEntityManager::AddComponentsToEntity(Entity entity, Args&& ... args)
{
	if (mFreeEntity <= entity)
	{
		return nullptr;
	}

	uint32 index = GComponent<C>::getComponentIndex();
	C* new_component = new C(std::forward<Args>(args) ...);
	if (!mComponents[index][entity])
	{
		mComponents[index][entity] = static_cast<GBaseComponent*>(new_component);
	}
	return new_component;
}

template<typename C>
C* GEntityManager::GetComponent(Entity entity)
{
	uint32 index = GComponent<C>::getComponentIndex();
	return dynamic_cast<C*>(mComponents[index][entity]);
}


#endif

