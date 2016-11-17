#ifndef GENTITYMANAGER_H
#define GENTITYMANAGER_H

#include <vector>
#include <queue>

#include "Types.h"

#include "gLocationComponent.h"
#include "gAnimationComponent.h"
#include "gActionComponent.h"
#include "gMouseDownEventComponent.h"
#include "gMouseUpEventComponent.h"
#include "gRenderableComponent.h"

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

	bool IsInsideEntity(Entity entity, GPoint point);
	void LocalPoint(Entity entity, GPoint& point, GPoint& localPoint);

public:
	template<typename C, typename... Args>
	C* AddComponentsToEntity(Entity entity, Args&& ... args);

	template<typename C>
	C* GetComponent(Entity entity);

	template<typename C>
	bool DoesHaveComponent(Entity entity);

private:
	GEntityManager();
	~GEntityManager();
	
	uint32 GetComponentCount();

private:
	uint32 defaulEntityCount;

	queue<Entity>                   mAvailableEntities;

	vector<vector<GBaseComponent*>> mComponents; // заменить на другой контейнер
	vector<Entity>                  mActiveEntities;
};

template<typename C, typename... Args>
C*   GEntityManager::AddComponentsToEntity(Entity entity, Args&& ... args)
{
	uint32 index = GComponent<C>::GetComponentId();
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
	uint32 index = GComponent<C>::GetComponentId();
	return static_cast<C*>(mComponents[index][entity]);
}

template<typename C>
bool GEntityManager::DoesHaveComponent(Entity entity)
{
	uint32 index = GComponent<C>::GetComponentId();
	return mComponents[index][entity] != nullptr;
}

#endif

