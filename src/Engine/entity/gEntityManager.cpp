#include "gEntityManager.h"


GEntityManager* GEntityManager::Instance()
{
	static GEntityManager* instance = new GEntityManager();
	return instance;
}

GEntityManager::GEntityManager():
	defaulEntityCount(100)
{
	mComponents.resize(GetComponentCount());
	for (int index = 0; index < mComponents.size(); ++index)
	{
		mComponents[index].resize(defaulEntityCount);
	}
	for (uint32 index = 0; index < defaulEntityCount; ++index)
	{
		mAvailableEntities.push(index);
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
	Entity newEntity = mAvailableEntities.front();
	mAvailableEntities.pop();
	mActiveEntities.push_back(newEntity);

	return newEntity;
}

void GEntityManager::DestroyEntity(Entity entity)
{
	for (int index = 0; index < mComponents.size(); ++index)
	{
		SAFE_DELETE(mComponents[index][entity]);
	}
	mAvailableEntities.push(entity);
	for (std::vector<Entity>::const_iterator iter = mActiveEntities.begin(); iter != mActiveEntities.end(); iter++)
	{
		if ((*iter) == entity)
			mActiveEntities.erase(iter);
	}
}

GBaseComponent* GEntityManager::GetComponent(Entity entity, uint32 index)
{
	return mComponents[index][entity];
}

uint32 GEntityManager::GetComponentCount()
{
	return GBaseComponent::s_component_counter;
}

bool GEntityManager::IsInsideEntity(Entity entity, GPoint point)
{
	if (!DoesHaveComponent<GLocationComponent>(entity) ||
		!DoesHaveComponent<GRenderableComponent>(entity))
	{
		return false;
	}
	GPoint localPoint;       //localized relativly the entity
	LocalPoint(entity, point, localPoint);

	uint32 index = GComponent<GRenderableComponent>::GetComponentId();
	GRenderableComponent* renderable = static_cast<GRenderableComponent*>(mComponents[index][entity]);
	return renderable->IsPiontInsideWH(localPoint);
}

void GEntityManager::LocalPoint(Entity entity, GPoint& point, GPoint& localPoint)
{
	uint32 index = GComponent<GLocationComponent>::GetComponentId();
	GLocationComponent* location = static_cast<GLocationComponent*>(mComponents[index][entity]);

	float ang = 0;                          // градусы в радианы
	float c = cosf(ang), s = sinf(ang);
	localPoint.x = ( c*(point.x - location->getX()) + s*(point.y - location->getY()));
	localPoint.y = (-s*(point.x - location->getX()) + c*(point.y - location->getY()));
}